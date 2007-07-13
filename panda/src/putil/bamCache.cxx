// Filename: bamCache.cxx
// Created by:  drose (09Jun06)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "bamCache.h"
#include "bamCacheIndex.h"
#include "hashVal.h"
#include "datagramInputFile.h"
#include "datagramOutputFile.h"
#include "config_util.h"
#include "bam.h"
#include "typeRegistry.h"
#include "string_utils.h"
#include "configVariableInt.h"
#include "configVariableString.h"

BamCache *BamCache::_global_ptr = NULL;

////////////////////////////////////////////////////////////////////
//     Function: BamCache::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
BamCache::
BamCache() :
  _active(true),
  _read_only(false),
  _index(new BamCacheIndex),
  _index_stale_since(0)
{
  ConfigVariableFilename model_cache_dir
    ("model-cache-dir", Filename(), 
     PRC_DESC("The full path to a directory, local to this computer, in which "
              "model and texture files will be cached on load.  If a directory "
              "name is specified here, files may be loaded from the cache "
              "instead of from their actual pathnames, which may save load time, "
              "especially if you are loading egg files instead of bam files, "
              "or if you are loading models from a shared network drive.  "
              "If this is the empty string, no cache will be used."));
  
  ConfigVariableInt model_cache_flush
    ("model-cache-flush", 30,
     PRC_DESC("This is the amount of time, in seconds, between automatic "
              "flushes of the model-cache index."));

  ConfigVariableBool model_cache_textures
    ("model-cache-textures", true,
     PRC_DESC("If this is set to true, textures will also be cached in the "
              "model cache, as txo files."));

  ConfigVariableInt model_cache_max_kbytes
    ("model-cache-max-kbytes", 1048576,
     PRC_DESC("This is the maximum size of the model cache, in kilobytes."));

  _cache_textures = model_cache_textures;
  _flush_time = model_cache_flush;
  _max_kbytes = model_cache_max_kbytes;

  if (!model_cache_dir.empty()) {
    set_root(model_cache_dir);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
BamCache::
~BamCache() {
  flush_index();
  delete _index;
  _index = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::set_root
//       Access: Published
//  Description: Changes the current root pathname of the cache.  This
//               specifies where the cache files are stored on disk.
//               This should name a directory that is on a disk local
//               to the machine (not on a network-mounted disk), for
//               instance, /tmp/panda-cache or /c/panda-cache.
//
//               If the directory does not already exist, it will be
//               created as a result of this call.
////////////////////////////////////////////////////////////////////
void BamCache::
set_root(const Filename &root) {
  flush_index();
  _root = root;

  // For now, the filename must be a directory.  Maybe eventually we
  // will support writing caches to a Panda multifile (though maybe it
  // would be better to implement this kind of thing at a lower level,
  // via a writable VFS, in which case the specified root filename
  // will still be a "directory").
  if (!root.is_directory()) {
    Filename dirname(_root, Filename("."));
    dirname.make_dir();
  }
  nassertv(root.is_directory());

  delete _index;
  _index = new BamCacheIndex;
  _index_stale_since = 0;
  read_index();
  check_cache_size();
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::lookup
//       Access: Published
//  Description: Looks up a file in the cache.  
//
//               If the file is cacheable, then regardless of whether
//               the file is found in the cache or not, this returns a
//               BamCacheRecord.  On the other hand, if the file
//               cannot be cached, returns NULL.
//
//               If record->has_data() returns true, then the file was
//               found in the cache, and you may call
//               record->extract_data() to get the object.  If
//               record->has_data() returns false, then the file was
//               not found in the cache or the cache was stale; and
//               you should reload the source file (calling
//               record->add_dependent_file() for each file loaded,
//               including the original source file), and then call
//               record->set_data() to record the resulting loaded
//               object; and finally, you should call store() to write
//               the cached record to disk.
////////////////////////////////////////////////////////////////////
PT(BamCacheRecord) BamCache::
lookup(const Filename &source_filename, const string &cache_extension) {
  consider_flush_index();

  if ((cache_extension == "txo")&&(!_cache_textures)) {
    return NULL;
  }
  
  Filename source_pathname(source_filename);
  source_pathname.make_absolute();

  Filename rel_pathname(source_pathname);
  rel_pathname.make_relative_to(_root, false);
  if (rel_pathname.is_local()) {
    // If the source pathname is already within the cache directory,
    // don't cache it further.
    return NULL;
  }

  Filename cache_filename = hash_filename(source_pathname.get_fullpath());
  cache_filename.set_extension(cache_extension);

  return find_and_read_record(source_pathname, cache_filename);
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::store
//       Access: Published
//  Description: Flushes a cache entry to disk.  You must have
//               retrieved the cache record via a prior call to
//               lookup(), and then stored the data via
//               record->set_data().  Returns true on success, false
//               on failure.
////////////////////////////////////////////////////////////////////
bool BamCache::
store(BamCacheRecord *record) {
  nassertr(!record->_cache_pathname.empty(), false);
  nassertr(record->has_data(), false);

  if (_read_only) {
    return false;
  }
  
  consider_flush_index();

#ifndef NDEBUG
  // Ensure that the cache_pathname is within the _root directory tree.
  Filename rel_pathname(record->_cache_pathname);
  rel_pathname.make_relative_to(_root, false);
  nassertr(rel_pathname.is_local(), false);
#endif  // NDEBUG

  record->_recorded_time = time(NULL);

  Filename cache_pathname = Filename::binary_filename(record->_cache_pathname);

  // We actually do the write to a temporary filename first, and then
  // move it into place, so that no one attempts to read the file
  // while it is in the process of being written.
  Filename temp_pathname = cache_pathname;
  temp_pathname.set_extension("tmp");
  temp_pathname.set_binary();

  ofstream temp_file;
  if (!temp_pathname.open_write(temp_file)) {
    util_cat.error()
      << "Could not open cache file: " << temp_pathname << "\n";
    emergency_read_only();
    return false;
  }

  DatagramOutputFile dout;
  if (!dout.open(temp_file)) {
    util_cat.error()
      << "Could not write cache file: " << temp_pathname << "\n";
    temp_pathname.unlink();
    emergency_read_only();
    return false;
  }
  
  if (!dout.write_header(_bam_header)) {
    util_cat.error()
      << "Unable to write to " << temp_pathname << "\n";
    temp_pathname.unlink();
    return false;
  }

  BamWriter writer(&dout, temp_pathname);
  if (!writer.init()) {
    temp_pathname.unlink();
    return false;
  }

  TypeRegistry *type_registry = TypeRegistry::ptr();
  TypeHandle texture_type = type_registry->find_type("Texture");
  if (record->get_data()->is_of_type(texture_type)) {
    // Texture objects write the actual texture image.
    writer.set_file_texture_mode(BTM_rawdata);
  } else {
    // Any other kinds of objects write texture references.
    writer.set_file_texture_mode(BTM_fullpath);
  }

  if (!writer.write_object(record)) {
    temp_pathname.unlink();
    return false;
  }

  if (!writer.write_object(record->get_data())) {
    temp_pathname.unlink();
    return false;
  }

  record->_record_size = temp_file.tellp();
  temp_file.close();

  // Now move the file into place.
  if (!temp_pathname.rename_to(cache_pathname)) {
    cache_pathname.unlink();
    if (!temp_pathname.rename_to(cache_pathname)) {
      util_cat.error()
        << "Unable to rename " << temp_pathname << " to " 
        << cache_pathname << "\n";
      temp_pathname.unlink();
      return false;
    }
  }

  add_to_index(record);

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::emergency_read_only
//       Access: Private
//  Description: Called when an attempt to write to the cache dir
//               has failed, usually for lack of disk space or 
//               because of incorrect file permissions.  Outputs
//               an error and puts the BamCache into read-only
//               mode.
////////////////////////////////////////////////////////////////////
void BamCache::
emergency_read_only() {
  util_cat.error() <<
    "Could not write to the Bam Cache.  Disabling future attempts.\n";
  _read_only = true;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::consider_flush_index
//       Access: Published
//  Description: Flushes the index if enough time has elapsed since
//               the index was last flushed.
////////////////////////////////////////////////////////////////////
void BamCache::
consider_flush_index() {
  if (_index_stale_since != 0) {
    int elapsed = (int)time(NULL) - (int)_index_stale_since;
    if (elapsed > _flush_time) {
      flush_index();
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::flush_index
//       Access: Published
//  Description: Ensures the index is written to disk.
////////////////////////////////////////////////////////////////////
void BamCache::
flush_index() {
  if (_index_stale_since == 0) {
    // Never mind.
    return;
  }

  while (true) {
    if (_read_only) {
      return;
    }

    Filename temp_pathname = Filename::temporary(_root, "index-", ".boo");

    if (!do_write_index(temp_pathname, _index)) {
      emergency_read_only();
      return;
    }
    
    // Now atomically write the name of this index file to the index
    // reference file.
    Filename index_ref_pathname(_root, Filename("index_name.txt"));
    string old_index = _index_ref_contents;
    string new_index = temp_pathname.get_basename() + "\n";
    string orig_index;
    if (index_ref_pathname.atomic_compare_and_exchange_contents(orig_index, old_index, new_index)) {
      // We successfully wrote our version of the index, and no other
      // process beat us to it.  Our index is now the official one.
      // Remove the old index.
      _index_pathname.unlink();
      _index_pathname = temp_pathname;
      _index_ref_contents = new_index;
      _index_stale_since = 0;
      return;
    }

    // Shoot, some other process updated the index while we were
    // trying to update it, and they beat us to it.  We have to merge,
    // and try again.
    temp_pathname.unlink();
    _index_pathname = Filename(_root, Filename(trim(orig_index)));
    _index_ref_contents = orig_index;
    read_index();
  }
  check_cache_size();
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::read_index
//       Access: Private
//  Description: Reads, or re-reads the index file from disk.  If
//               _index_stale_since is nonzero, the index file is read
//               and then merged with our current index.
////////////////////////////////////////////////////////////////////
void BamCache::
read_index() {
  if (!read_index_pathname(_index_pathname, _index_ref_contents)) {
    // Couldn't read the index ref; rebuild the index.
    rebuild_index();
    return;
  }

  while (true) {
    BamCacheIndex *new_index = do_read_index(_index_pathname);
    if (new_index != (BamCacheIndex *)NULL) {
      merge_index(new_index);
      return;
    }

    // We couldn't read the index.  Maybe it's been removed already.
    // See if the index_pathname has changed.
    Filename old_index_pathname = _index_pathname;
    if (!read_index_pathname(_index_pathname, _index_ref_contents)) {
      // Couldn't read the index ref; rebuild the index.
      rebuild_index();
      return;
    }

    if (old_index_pathname == _index_pathname) {
      // Nope, we just couldn't read it.  Delete it and build a new
      // one.
      _index_pathname.unlink();
      rebuild_index();
      flush_index();
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::read_index_pathname
//       Access: Private
//  Description: Atomically reads the current index filename from the
//               index reference file.  The index filename moves
//               around as different processes update the index.
////////////////////////////////////////////////////////////////////
bool BamCache::
read_index_pathname(Filename &index_pathname, string &index_ref_contents) const {
  index_ref_contents.clear();
  Filename index_ref_pathname(_root, Filename("index_name.txt"));
  if (!index_ref_pathname.atomic_read_contents(index_ref_contents)) {
    return false;
  }

  string trimmed = trim(index_ref_contents);
  if (trimmed.empty()) {
    index_pathname = Filename();
  } else {
    index_pathname = Filename(_root, Filename(trimmed));
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::merge_index
//       Access: Private
//  Description: The supplied index file has been updated by some other
//               process.  Merge it with our current index.
//
//               Ownership of the pointer is transferred with this
//               call.  The caller should assume that new_index will
//               be deleted by this method.
////////////////////////////////////////////////////////////////////
void BamCache::
merge_index(BamCacheIndex *new_index) {
  if (_index_stale_since == 0) {
    // If our index isn't stale, just replace it.
    delete _index;
    _index = new_index;
    return;
  }

  BamCacheIndex *old_index = _index;
  old_index->release_records();
  new_index->release_records();
  _index = new BamCacheIndex;

  BamCacheIndex::Records::const_iterator ai = old_index->_records.begin();
  BamCacheIndex::Records::const_iterator bi = new_index->_records.begin();
  
  while (ai != old_index->_records.end() && 
         bi != new_index->_records.end()) {
    if ((*ai).first < (*bi).first) {
      // Here is an entry we have in our index, not present in the new
      // index.
      PT(BamCacheRecord) record = (*ai).second;
      Filename cache_pathname(_root, record->get_cache_filename());
      if (cache_pathname.exists()) {
        // The file exists; keep it.
        _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(record->get_source_pathname(), record));
      }
      ++ai;

    } else if ((*bi).first < (*ai).first) {
      // Here is an entry in the new index, not present in our index.
      PT(BamCacheRecord) record = (*bi).second;
      Filename cache_pathname(_root, record->get_cache_filename());
      if (cache_pathname.exists()) {
        // The file exists; keep it.
        _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(record->get_source_pathname(), record));
      }
      ++bi;

    } else {
      // Here is an entry we have in both.
      PT(BamCacheRecord) a_record = (*ai).second;
      PT(BamCacheRecord) b_record = (*bi).second;
      if (*a_record == *b_record) {
        // They're the same entry.  It doesn't really matter which one
        // we keep.
        _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(a_record->get_source_pathname(), a_record));

      } else {
        // They're different.  Just throw them both away, and re-read
        // the current data from the cache file.

        Filename cache_pathname(_root, a_record->get_cache_filename());

        if (cache_pathname.exists()) {
          PT(BamCacheRecord) record = do_read_record(cache_pathname, false);
          if (record != (BamCacheRecord *)NULL) {
            _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(record->get_source_pathname(), record));
          }
        }
      }

      ++ai;
      ++bi;
    }
  }

  while (ai != old_index->_records.end()) {
    // Here is an entry we have in our index, not present in the new
    // index.
    PT(BamCacheRecord) record = (*ai).second;
    Filename cache_pathname(_root, record->get_cache_filename());
    if (cache_pathname.exists()) {
      // The file exists; keep it.
      _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(record->get_source_pathname(), record));
    }
    ++ai;
  }
   
  while (bi != new_index->_records.end()) {
    // Here is an entry in the new index, not present in our index.
    PT(BamCacheRecord) record = (*bi).second;
    Filename cache_pathname(_root, record->get_cache_filename());
    if (cache_pathname.exists()) {
      // The file exists; keep it.
      _index->_records.insert(_index->_records.end(), BamCacheIndex::Records::value_type(record->get_source_pathname(), record));
    }
    ++bi;
  }

  _index->process_new_records();
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::rebuild_index
//       Access: Private
//  Description: Regenerates the index from scratch by scanning the
//               directory.
////////////////////////////////////////////////////////////////////
void BamCache::
rebuild_index() {
  vector_string contents;
  if (!_root.scan_directory(contents)) {
    util_cat.error()
      << "Unable to read directory " << _root << ", caching disabled.\n";
    set_active(false);
    return;
  }

  delete _index;
  _index = new BamCacheIndex;

  vector_string::const_iterator ci;
  for (ci = contents.begin(); ci != contents.end(); ++ci) {
    Filename filename(*ci);
    if (filename.get_extension() == "bam" ||
        filename.get_extension() == "txo") {
      Filename pathname(_root, filename);

      PT(BamCacheRecord) record = do_read_record(pathname, false);
      if (record == (BamCacheRecord *)NULL) {
        // Well, it was invalid, so blow it away.
        pathname.unlink();

      } else {
        record->_record_access_time = record->_recorded_time;

        bool inserted = _index->_records.insert(BamCacheIndex::Records::value_type(record->get_source_pathname(), record)).second;
        if (!inserted) {
          util_cat.info()
            << "Multiple cache files defining " << record->get_source_pathname() << "\n";
          pathname.unlink();
        }
      }
    }
  }
  _index->process_new_records();

  _index_stale_since = time(NULL);
  check_cache_size();
  flush_index();
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::add_to_index
//       Access: Private
//  Description: Updates the index entry for the indicated record.
//               Note that a copy of the record is made first.
////////////////////////////////////////////////////////////////////
void BamCache::
add_to_index(const BamCacheRecord *record) {
  PT(BamCacheRecord) new_record = record->make_copy();

  if (_index->add_record(new_record)) {
    mark_index_stale();
    check_cache_size();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::remove_from_index
//       Access: Private
//  Description: Removes the index entry for the indicated record, if
//               there is one.
////////////////////////////////////////////////////////////////////
void BamCache::
remove_from_index(const Filename &source_pathname) {
  if (_index->remove_record(source_pathname)) {
    mark_index_stale();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::check_cache_size
//       Access: Private
//  Description: If the cache size has exceeded its specified size
//               limit, removes an old file.
////////////////////////////////////////////////////////////////////
void BamCache::
check_cache_size() {
  if (_index->_cache_size == 0) {
    // 0 means no limit.
    return;
  }

  if (_index->_cache_size / 1024 > _max_kbytes) {
    while (_index->_cache_size / 1024 > _max_kbytes) {
      PT(BamCacheRecord) record = _index->evict_old_file();
      if (record == NULL) {
        // Never mind; the cache is empty.
        break;
      }
      Filename cache_pathname(_root, record->get_cache_filename());
      cache_pathname.unlink();
    }
    mark_index_stale();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::do_read_index
//       Access: Private, Static
//  Description: Reads the index data from the specified filename.
//               Returns a newly-allocated BamCacheIndex object on
//               success, or NULL on failure.
////////////////////////////////////////////////////////////////////
BamCacheIndex *BamCache::
do_read_index(Filename &index_pathname) {
  if (index_pathname.empty()) {
    return NULL;
  }

  index_pathname.set_binary();
  ifstream index_file;
  if (!index_pathname.open_read(index_file)) {
    util_cat.error()
      << "Could not open index file: " << index_pathname << "\n";
    return NULL;
  }

  DatagramInputFile din;
    
  if (!din.open(index_file)) {
    util_cat.debug()
      << "Could not read index file: " << index_pathname << "\n";
    return NULL;
  }
  
  string head;
  if (!din.read_header(head, _bam_header.size())) {
    util_cat.debug()
      << index_pathname << " is not an index file.\n";
    return NULL;
  }
  
  if (head != _bam_header) {
    util_cat.debug()
      << index_pathname << " is not an index file.\n";
    return NULL;
  }
  
  BamReader reader(&din, index_pathname);
  if (!reader.init()) {
    return NULL;
  }

  TypedWritable *object = reader.read_object();

  if (object == (TypedWritable *)NULL) {
    util_cat.error()
      << "Cache index " << index_pathname << " is empty.\n";
    return NULL;

  } else if (!object->is_of_type(BamCacheIndex::get_class_type())) {
    util_cat.error()
      << "Cache index " << index_pathname << " contains a "
      << object->get_type() << ", not a BamCacheIndex.\n";
    return NULL;
  }

  BamCacheIndex *index = DCAST(BamCacheIndex, object);
  if (!reader.resolve()) {
    util_cat.error()
      << "Unable to fully resolve cache index file.\n";
    return NULL;
  }

  return index;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::do_write_index
//       Access: Private, Static
//  Description: Writes the given index data to the specified filename.
////////////////////////////////////////////////////////////////////
bool BamCache::
do_write_index(Filename &index_pathname, const BamCacheIndex *index) {
  index_pathname.set_binary();
  ofstream index_file;
  
  if (!index_pathname.open_write(index_file)) {
    util_cat.error()
      << "Could not open index file: " << index_pathname << "\n";
    return false;
  }
  
  DatagramOutputFile dout;
  if (!dout.open(index_file)) {
    util_cat.error()
      << "Could not write index file: " << index_pathname << "\n";
    index_pathname.unlink();
    return false;
  }

  if (!dout.write_header(_bam_header)) {
    util_cat.error()
      << "Unable to write to " << index_pathname << "\n";
    index_pathname.unlink();
    return false;
  }

  BamWriter writer(&dout, index_pathname);
  if (!writer.init()) {
    index_pathname.unlink();
    return false;
  }

  if (!writer.write_object(index)) {
    index_pathname.unlink();
    return false;
  }

  index_file.close();
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::find_and_read_record
//       Access: Private
//  Description: Looks for the existing cache file that corresponds
//               to the indicated filename.  Normally, this is the
//               specified cache filename exactly; but in the case of
//               a hash collision, it may be a variant of the cache
//               filename.
////////////////////////////////////////////////////////////////////
PT(BamCacheRecord) BamCache::
find_and_read_record(const Filename &source_pathname, 
                     const Filename &cache_filename) {
  int pass = 0;
  while (true) {
    PT(BamCacheRecord) record = 
      read_record(source_pathname, cache_filename, pass);
    if (record != (BamCacheRecord *)NULL) {
      add_to_index(record);
      return record;
    }
    ++pass;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::read_record
//       Access: Private
//  Description: Reads the indicated cache file and returns its
//               associated record if it can be read and it matches
//               the source filename.
////////////////////////////////////////////////////////////////////
PT(BamCacheRecord) BamCache::
read_record(const Filename &source_pathname, 
            const Filename &cache_filename,
            int pass) {
  Filename cache_pathname(_root, cache_filename);
  if (pass != 0) {
    ostringstream strm;
    strm << cache_pathname.get_basename_wo_extension() << "_" << pass;
    cache_pathname.set_basename_wo_extension(strm.str());
  }
  
  if (!cache_pathname.exists()) {
    // There is no such cache file already.  Declare it.
    PT(BamCacheRecord) record =
      new BamCacheRecord(source_pathname, cache_filename);
    record->_cache_pathname = cache_pathname;
    return record;
  }

  PT(BamCacheRecord) record = do_read_record(cache_pathname, true);
  if (record == (BamCacheRecord *)NULL) {
    // Well, it was invalid, so blow it away, and make a new one.
    cache_pathname.unlink();
    remove_from_index(source_pathname);

    PT(BamCacheRecord) record =
      new BamCacheRecord(source_pathname, cache_filename);
    record->_cache_pathname = cache_pathname;
    return record;
  }

  if (record->get_source_pathname() != source_pathname) {
    // This might be just a hash conflict.
    util_cat.debug()
      << "Cache file " << cache_pathname << " references "
      << record->get_source_pathname() << ", not "
      << source_pathname << "\n";
    return NULL;
  }

  if (!record->has_data()) {
    // If we didn't find any data, the caller will have to reload it.
    record->clear_dependent_files();
  }

  record->_cache_pathname = cache_pathname;
  return record;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::do_read_record
//       Access: Private, Static
//  Description: Actually reads a record from the file.
////////////////////////////////////////////////////////////////////
PT(BamCacheRecord) BamCache::
do_read_record(Filename &cache_pathname, bool read_data) {
  cache_pathname.set_binary();
  ifstream cache_file;
  if (!cache_pathname.open_read(cache_file)) {
    util_cat.debug()
      << "Could not open cache file: " << cache_pathname << "\n";
    return NULL;
  }

  DatagramInputFile din;
    
  if (!din.open(cache_file)) {
    util_cat.debug()
      << "Could not read cache file: " << cache_pathname << "\n";
    return NULL;
  }
  
  string head;
  if (!din.read_header(head, _bam_header.size())) {
    util_cat.debug()
      << cache_pathname << " is not a cache file.\n";
    return NULL;
  }
  
  if (head != _bam_header) {
    util_cat.debug()
      << cache_pathname << " is not a cache file.\n";
    return NULL;
  }
  
  BamReader reader(&din, cache_pathname);
  if (!reader.init()) {
    return NULL;
  }
  
  TypedWritable *object = reader.read_object();
  if (object == (TypedWritable *)NULL) {
    util_cat.debug()
      << cache_pathname << " is empty.\n";
    return NULL;
    
  } else if (!object->is_of_type(BamCacheRecord::get_class_type())) {
    util_cat.debug()
      << "Cache file " << cache_pathname << " contains a "
      << object->get_type() << ", not a BamCacheRecord.\n";
    return NULL;
  }
  
  PT(BamCacheRecord) record = DCAST(BamCacheRecord, object);
  if (!reader.resolve()) {
    util_cat.debug()
      << "Unable to fully resolve cache record in " << cache_pathname << "\n";
    return NULL;
  }

  // From this point below, we have validated that the selected
  // filename is indeed a cache record for the indicated source file,
  // and therefore the cache record will be returned.

  // We still need to decide whether the cache record is stale.
  if (read_data && record->dependents_unchanged()) {
    // The cache record doesn't appear to be stale.  Load the cached
    // object.
    object = reader.read_object();

    if (object != (TypedWritable *)NULL) {
      if (!reader.resolve()) {
        util_cat.debug()
          << "Unable to fully resolve cached object in " << cache_pathname << "\n";
        delete object;
      } else {
        // The object is valid.  Store it in the record.
        record->set_data(object, true);
      }
    }
  }
  
  // Also get the file size.
  cache_file.clear();
  cache_file.seekg(0, ios::end);
  record->_record_size = cache_file.tellg();

  // And the last access time is now, duh.
  record->_record_access_time = time(NULL);

  return record;
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::hash_filename
//       Access: Private, Static
//  Description: Returns the appropriate filename to use for a cache
//               file, given the fullpath string to the source
//               filename.
////////////////////////////////////////////////////////////////////
string BamCache::
hash_filename(const string &filename) {
#ifdef HAVE_OPENSSL
  // With OpenSSl, use the MD5 hash of the filename.
  HashVal hv;
  hv.hash_string(filename);
  ostringstream strm;
  hv.output_hex(strm);
  return strm.str();

#else  // HAVE_OPENSSL
  // Without OpenSSL, don't get fancy; just build a simple hash.
  unsigned int hash = 0;
  for (string::const_iterator si = filename.begin(); 
       si != filename.end(); 
       ++si) {
    hash = (hash * 9109) + (unsigned int)(*si);
  }

  ostringstream strm;
  strm << hex << setw(8) << setfill('0') << hash;
  return strm.str();

#endif  // HAVE_OPENSSL
}

////////////////////////////////////////////////////////////////////
//     Function: BamCache::make_global
//       Access: Private, Static
//  Description: Constructs the global BamCache object.
////////////////////////////////////////////////////////////////////
void BamCache::
make_global() {
  _global_ptr = new BamCache;

  if (_global_ptr->get_root().empty()) {
    _global_ptr->set_active(false);
  }
}
