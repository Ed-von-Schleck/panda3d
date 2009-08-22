// Filename: p3dMultifileReader.cxx
// Created by:  drose (15Jun09)
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

#include "p3dMultifileReader.h"
#include "p3dPackage.h"
#include "mkdir_complete.h"

#include <time.h>

#ifdef _WIN32
#include <sys/utime.h>
#include <direct.h>
#define stat _stat
#define utime _utime
#define utimbuf _utimbuf

#else
#include <utime.h>

#endif

// This sequence of bytes begins each Multifile to identify it as a
// Multifile.
const char P3DMultifileReader::_header[] = "pmf\0\n\r";
const size_t P3DMultifileReader::_header_size = 6;

const int P3DMultifileReader::_current_major_ver = 1;
const int P3DMultifileReader::_current_minor_ver = 1;

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
P3DMultifileReader::
P3DMultifileReader() {
}

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::extract_all
//       Access: Public
//  Description: Reads the named multifile, and extracts all the
//               expected extractable components within it to the
//               indicated directory.  Returns true on success, false
//               on failure.
//
//               The parameters package, start_progress, and
//               progress_size are provided to make the appropriate
//               status updates on the package's progress callbacks
//               during this operation.
////////////////////////////////////////////////////////////////////
bool P3DMultifileReader::
extract_all(const string &pathname, const string &to_dir,
            P3DPackage *package, double start_progress, double progress_size) {
  if (!read_header(pathname)) {
    return false;
  }

  // Now walk through all of the files, and extract only the ones we
  // expect to encounter.
  size_t num_processed = 0;
  size_t num_expected = _subfiles.size();
  if (package != NULL) {
    num_expected = package->_extracts.size();
  }

  Subfiles::iterator si;
  for (si = _subfiles.begin(); si != _subfiles.end(); ++si) {
    const Subfile &s = (*si);
    if (package != NULL && !package->is_extractable(s._filename)) {
      continue;
    }

    string output_pathname = to_dir + "/" + s._filename;
    if (!mkfile_complete(output_pathname, nout)) {
      return false;
    }

    ofstream out(output_pathname.c_str(), ios::out | ios::binary);
    if (!out) {
      nout << "Unable to write to " << output_pathname << "\n";
      return false;
    }

    if (!extract_subfile(out, s)) {
      return false;
    }

    // Set the timestamp according to the multifile.
    out.close();
    utimbuf utb;
    utb.actime = time(NULL);
    utb.modtime = s._timestamp;
    utime(output_pathname.c_str(), &utb);


    ++num_processed;
    if (package != NULL) {
      double progress = (double)num_processed / (double)num_expected;
      package->report_progress(start_progress + progress * progress_size);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::extract_one
//       Access: Public
//  Description: Reads the named multifile, and extracts only the
//               named component to the indicated stream.  Returns
//               true on success, false on failure.
////////////////////////////////////////////////////////////////////
bool P3DMultifileReader::
extract_one(const string &pathname, ostream &out, const string &filename) {
  if (!read_header(pathname)) {
    return false;
  }

  // Look for the named component.
  Subfiles::iterator si;
  for (si = _subfiles.begin(); si != _subfiles.end(); ++si) {
    const Subfile &s = (*si);
    if (s._filename == filename) {
      return extract_subfile(out, s);
    }
  }

  nout << "Could not extract " << filename << ": not found.\n";
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::read_header
//       Access: Private
//  Description: Opens the named multifile and reads the header
//               information and index, returning true on success,
//               false on failure.
////////////////////////////////////////////////////////////////////
bool P3DMultifileReader::
read_header(const string &pathname) {
  _subfiles.clear();

  _in.open(pathname.c_str(), ios::in | ios::binary);
  if (!_in) {
    nout << "Couldn't open " << pathname << "\n";
    return false;
  }

  char this_header[_header_size];
  _in.seekg(0);
  _in.read(this_header, _header_size);
  if (_in.fail() || _in.gcount() != (unsigned)_header_size) {
    nout
      << "Unable to read Multifile header " << pathname << ".\n";
    return false;
  }

  // Here's a special case: if the multifile begins with a hash
  // character, then we skip at least 6 characters, and continue
  // reading and discarding lines of ASCII text, until we come across
  // a nonempty line that does not begin with a hash character.  This
  // allows a P3D application (which is a multifile) to be run
  // directly on the command line on Unix-based systems.
  if (this_header[0] == '#') {
    int ch = '#';
    while (ch != EOF && ch == '#') {
      // Skip to the end of the line.
      while (ch != EOF && ch != '\n') {
        ch = _in.get();
      }
      // Skip to the first non-whitespace character of the line.
      while (ch != EOF && (isspace(ch) || ch == '\r')) {
        ch = _in.get();
      }
    }

    // Now fill up the header.
    this_header[0] = ch;
    _in.read(this_header + 1, _header_size - 1);
  }

  if (memcmp(this_header, _header, _header_size) != 0) {
    nout << "Failed header check: " << pathname << "\n";
    return false;
  }

  unsigned int major = read_uint16();
  unsigned int minor = read_uint16();
  if (major != _current_major_ver || minor != _current_minor_ver) {
    nout << "Incompatible multifile version: " << pathname << "\n";
    return false;
  }

  unsigned int scale = read_uint32();
  if (scale != 1) {
    nout << "Unsupported scale factor in " << pathname << "\n";
    return false;
  }

  // We don't care about the overall timestamp.
  read_uint32();

  if (!read_index()) {
    nout << "Error reading multifile index\n";
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::read_index
//       Access: Private
//  Description: Assuming the file stream is positioned at the first
//               record, reads all of the records into the _subfiles
//               list.  Returns true on success, false on failure.
////////////////////////////////////////////////////////////////////
bool P3DMultifileReader::
read_index() {
  unsigned int next_entry = read_uint32();
  if (!_in) {
    return false;
  }
  while (next_entry != 0) {
    Subfile s;
    s._start = read_uint32();
    s._length = read_uint32();
    unsigned int flags = read_uint16();
    if ((flags & 0x18) != 0) {
      // Skip over the uncompressed length.
      read_uint32();
    }
      
    s._timestamp = read_uint32();
    size_t name_length = read_uint16();
    char *buffer = new char[name_length];
    _in.read(buffer, name_length);

    // The filenames are xored with 0xff just for fun.
    for (size_t ni = 0; ni < name_length; ++ni) {
      buffer[ni] ^= 0xff;
    }

    s._filename = string(buffer, name_length);
    delete[] buffer;

    if (flags == 0) {
      // We can only support subfiles with no particular flags set.
      _subfiles.push_back(s);
    }

    _in.seekg(next_entry);
    next_entry = read_uint32();
    if (!_in) {
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: P3DMultifileReader::extract_subfile
//       Access: Private
//  Description: Extracts the indicated subfile and writes it to the
//               indicated stream.  Returns true on success, false on
//               failure.
////////////////////////////////////////////////////////////////////
bool P3DMultifileReader::
extract_subfile(ostream &out, const Subfile &s) {
  _in.seekg(s._start);

  static const size_t buffer_size = 1024;
  char buffer[buffer_size];
  
  size_t remaining_data = s._length;
  _in.read(buffer, min(buffer_size, remaining_data));
  size_t count = _in.gcount();
  while (count != 0) {
    remaining_data -= count;
    out.write(buffer, count);
    _in.read(buffer, min(buffer_size, remaining_data));
    count = _in.gcount();
  }
  
  if (remaining_data != 0) {
    nout << "Unable to extract " << s._filename << "\n";
    return false;
  }

  return true;
}
