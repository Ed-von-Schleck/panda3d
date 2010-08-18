// Filename: effectPool.cxx
// Created by: Xidram (14Aug10)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#include "effectPool.h"
#include "config_pgraph.h"
#include "mutexHolder.h"
#include "bamCache.h"
#include "bamCacheRecord.h"
#include "virtualFileSystem.h"

EffectPool *EffectPool::_global_ptr = (EffectPool *)NULL;

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::write
//       Access: Published, Static
//  Description: Lists the contents of the effect pool to the
//               indicated output stream.
//               Helps with debugging.
////////////////////////////////////////////////////////////////////
void EffectPool::
write(ostream &out) {
  get_global_ptr()->ns_list_contents(out);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::register_effect_type
//       Access: Public
//  Description: Records a factory function that makes a Effect
//               object of the appropriate type for one or more
//               particular filename extensions.  The string
//               extensions may be a string that contains
//               space-separated list of extensions, case-insensitive.
////////////////////////////////////////////////////////////////////
void EffectPool::
register_effect_type(MakeEffectFunc *func, const string &extensions) {
  MutexHolder holder(_lock);

  vector_string words;
  extract_words(downcase(extensions), words);

  vector_string::const_iterator wi;
  for (wi = words.begin(); wi != words.end(); ++wi) {
    _type_registry[*wi] = func;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::get_effect_type
//       Access: Public
//  Description: Returns the factory function to construct a new
//               effect of the type appropriate for the indicated
//               filename extension, if any, or NULL if the extension
//               is not one of the extensions for an effect file.
////////////////////////////////////////////////////////////////////
EffectPool::MakeEffectFunc *EffectPool::
get_effect_type(const string &extension) const {
  MutexHolder holder(_lock);

  string c = downcase(extension);
  TypeRegistry::const_iterator ti;
  ti = _type_registry.find(extension);
  if (ti != _type_registry.end()) {
    return (*ti).second;
  }

  // This is an unknown effect type.
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::make_effect
//       Access: Public
//  Description: Creates a new Effect object of the appropriate type
//               for the indicated filename extension, according to
//               the types that have been registered via
//               register_effect_type().
////////////////////////////////////////////////////////////////////
PT(Effect) EffectPool::
make_effect(const string &extension) const {
  MakeEffectFunc *func = get_effect_type(extension);
  if (func != NULL) {
    return func();
  }

  // We don't know what kind of file type this is; return an ordinary
  // Effect.
  return new Effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::write_effect_types
//       Access: Public
//  Description: Outputs a list of the available effect types to the
//               indicated output stream.
////////////////////////////////////////////////////////////////////
void EffectPool::
write_effect_types(ostream &out, int indent_level) const {
  MutexHolder holder(_lock);

  TypeRegistry::const_iterator ti;
  for (ti = _type_registry.begin(); ti != _type_registry.end(); ++ti) {
    string extension = (*ti).first;
    MakeEffectFunc *func = (*ti).second;

    PT(Effect) eff = func();
    string name = eff->get_type().get_name();
    indent(out, indent_level) << name;
    indent(out, max(30 - (int)name.length(), 0))
      << "  ." << extension << "\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_has_effect
//       Access: Private
//  Description: The nonstatic implementation of has_effect().
////////////////////////////////////////////////////////////////////
bool EffectPool::
ns_has_effect(const Filename &orig_filename) {
  MutexHolder holder(_lock);

  Filename filename;
  resolve_filename(filename, orig_filename);

  Effects::const_iterator ei;
  ei = _effects.find(filename);
  if (ei != _effects.end()) {
    // This effect was previously loaded.
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_load_effect
//       Access: Private
//  Description: The nonstatic implementation of load_effect().
////////////////////////////////////////////////////////////////////
Effect *EffectPool::
ns_load_effect(const Filename &orig_filename, const LoaderOptions &options) {
  Filename filename;

  {
    MutexHolder holder(_lock);
    resolve_filename(filename, orig_filename);
    Effects::const_iterator ei;
    ei = _effects.find(filename);
    if (ei != _effects.end()) {
      // This effect was previously loaded.
      Effect *effect = (*ei).second;
      nassertr(!effect->get_fullpath().empty(), effect);
      return effect;
    }
  }

  // The effect was not found in the pool.
  PT(Effect) effect;
  PT(BamCacheRecord) record;
  bool store_record = false;

  // See if the effect can be found in the on-disk cache, if it is active.
  BamCache *cache = BamCache::get_global_ptr();
  if (cache->get_cache_effects()) {
    record = cache->lookup(filename, "fxo");
    if (record != (BamCacheRecord *)NULL && record->has_data()) {
      effect = DCAST(Effect, record->get_data());
      pgraph_cat.info()
        << "Effect " << filename << " found in disk cache.\n";
    }
  }

  if (effect == (Effect *)NULL) {
    // The effect was neither in the pool, nor found in the on-disk
    // cache; it needs to be loaded from its source file.
    pgraph_cat.info()
      << "Loading effect " << filename << "\n";
    effect = make_effect(filename.get_extension());
    if (!effect->read(filename, record)) {
      // This effect was not found or could not be read.
      report_effect_unreadable(filename);
      return NULL;
    }

    store_record = (record != (BamCacheRecord *)NULL);
  }

  if (!cache->get_cache_effects()) {
    // We don't want to save this effect.
    store_record = false;
  }

  // Set the original filename, before we searched along the path.
  nassertr(effect != (Effect *)NULL, NULL);
  effect->set_filename(orig_filename);
  effect->set_fullpath(filename);

  {
    MutexHolder holder(_lock);

    // Now look again--someone may have just loaded this effect in
    // another thread.
    Effects::const_iterator ti;
    ti = _effects.find(filename);
    if (ti != _effects.end()) {
      // This effect was previously loaded.
      Effect *effect = (*ti).second;
      nassertr(!effect->get_fullpath().empty(), effect);
      return effect;
    }

    _effects[filename] = effect;
  }

  if (store_record) {
    // Store the on-disk cache record for next time.
    record->set_data(effect, effect);
    cache->store(record);
  }

  nassertr(!effect->get_fullpath().empty(), effect);

  return effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_add_effect
//       Access: Private
//  Description: The nonstatic implementation of add_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_add_effect(const Filename &filename, Effect *effect) {
  MutexHolder holder(_lock);
  // We blow away whatever effect was there previously, if any.
  _effects[filename] = effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_effect
//       Access: Private
//  Description: The nonstatic implementation of release_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_effect(const Filename &filename) {
  MutexHolder holder(_lock);
  Effects::iterator ti;
  ti = _effects.find(filename);
  if (ti != _effects.end()) {
    _effects.erase(ti);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_add_effect
//       Access: Private
//  Description: The nonstatic implementation of add_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_add_effect(Effect *effect) {
  MutexHolder holder(_lock);
  // We blow away whatever effect was there previously, if any.
  _effects[effect->get_fullpath()] = effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_effect
//       Access: Private
//  Description: The nonstatic implementation of release_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_effect(Effect *effect) {
  MutexHolder holder(_lock);
  Effects::iterator ti;
  ti = _effects.find(effect->get_fullpath());
  if (ti != _effects.end()) {
    _effects.erase(ti);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_all_effects
//       Access: Private
//  Description: The nonstatic implementation of release_all_effects().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_all_effects() {
  MutexHolder holder(_lock);
  _effects.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_garbage_collect
//       Access: Private
//  Description: The nonstatic implementation of garbage_collect().
////////////////////////////////////////////////////////////////////
int EffectPool::
ns_garbage_collect() {
  MutexHolder holder(_lock);

  int num_released = 0;
  Effects new_set;

  Effects::iterator ti;
  for (ti = _effects.begin(); ti != _effects.end(); ++ti) {
    Effect *node = (*ti).second;
    if (node == (Effect *)NULL ||
        node->get_ref_count() == 1) {
      if (loader_cat.is_debug()) {
        loader_cat.debug()
          << "Releasing " << (*ti).first << "\n";
      }
      ++num_released;
    } else {
      new_set.insert(new_set.end(), *ti);
    }
  }

  _effects.swap(new_set);
  return num_released;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_list_contents
//       Access: Private
//  Description: The nonstatic implementation of list_contents().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_list_contents(ostream &out) const {
  MutexHolder holder(_lock);

  out << "effect pool contents:\n";

  Effects::const_iterator ti;
  int num_effects = 0;
  for (ti = _effects.begin(); ti != _effects.end(); ++ti) {
    if ((*ti).second != NULL) {
      ++num_effects;
      out << (*ti).first << "\n"
          << "  (count = " << (*ti).second->get_ref_count()
          << ")\n";
    }
  }

  out << "total number of effects: " << num_effects << " (plus "
      << _effects.size() - num_effects << " entries for nonexistent files)\n";
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::resolve_filename
//       Access: Private
//  Description: Searches for the indicated filename along the
//               model path.  If the filename was previously
//               searched for, doesn't search again, as an
//               optimization.  Assumes _lock is held.
////////////////////////////////////////////////////////////////////
void EffectPool::
resolve_filename(Filename &new_filename, const Filename &orig_filename) {
  RelpathLookup::iterator rpi = _relpath_lookup.find(orig_filename);
  if (rpi != _relpath_lookup.end()) {
    new_filename = (*rpi).second;
    return;
  }

  new_filename = orig_filename;
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  vfs->resolve_filename(new_filename, get_model_path());

  _relpath_lookup[orig_filename] = new_filename;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::get_global_ptr
//       Access: Private, Static
//  Description: Initializes and/or returns the global pointer to the
//               one EffectPool object in the system.
////////////////////////////////////////////////////////////////////
EffectPool *EffectPool::
get_global_ptr() {
  if (_global_ptr == (EffectPool *)NULL) {
    _global_ptr = new EffectPool;
  }
  return _global_ptr;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::report_effect_unreadable
//       Access: Private
//  Description: Prints a suitable error message when an effect could
//               not be loaded.
////////////////////////////////////////////////////////////////////
void EffectPool::
report_effect_unreadable(const Filename &filename) const {
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vfs->exists(filename)) {
    if (filename.is_local()) {
      // The file doesn't exist, and it wasn't
      // fully-qualified--therefore, it wasn't found along either
      // search path.
      pgraph_cat.error()
        << "Unable to find effect \"" << filename << "\""
        << " on model-path " << get_model_path() <<"\n";
    } else {
      // A fully-specified filename is not searched along the path, so
      // don't mislead the user with the error message.
      pgraph_cat.error()
        << "Effect \"" << filename << "\" does not exist.\n";
    }

  } else {
    // The file exists, but it couldn't be read for some reason.
    pgraph_cat.error()
      << "Effect \"" << filename << "\" exists but cannot be read.\n";

    // Maybe the filename extension is unknown.
    MakeEffectFunc *func = get_effect_type(filename.get_extension());
    if (func == (MakeEffectFunc *)NULL) {
      gobj_cat.error()
        << "Effect extension \"" << filename.get_extension()
        << "\" is unknown.  Supported effect types:\n";
      write_effect_types(pgraph_cat.error(false), 2);
    }
  }
}

