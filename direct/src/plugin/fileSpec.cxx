// Filename: fileSpec.cxx
// Created by:  drose (29Jun09)
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

#include "fileSpec.h"
#include "openssl/md5.h"

#include <fstream>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <sys/utime.h>
#include <direct.h>
#define stat _stat
#define utime _utime
#define utimbuf _utimbuf

#else
#include <utime.h>

#endif

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
FileSpec::
FileSpec() {
  _size = 0;
  _timestamp = 0;
  memset(_hash, 0, sizeof(_hash));
  _got_hash = false;
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::load_xml
//       Access: Public
//  Description: Reads the data from the indicated XML file.
////////////////////////////////////////////////////////////////////
void FileSpec::
load_xml(TiXmlElement *element) {
  const char *filename = element->Attribute("filename");
  if (filename != NULL) {
    _filename = filename;
  }

  const char *size = element->Attribute("size");
  if (size != NULL) {
    char *endptr;
    _size = strtoul(size, &endptr, 10);
  }

  const char *timestamp = element->Attribute("timestamp");
  if (timestamp != NULL) {
    char *endptr;
    _timestamp = strtoul(timestamp, &endptr, 10);
  }

  _got_hash = false;
  const char *hash = element->Attribute("hash");
  if (hash != NULL && strlen(hash) == (hash_size * 2)) {
    // Decode the hex hash string.
    _got_hash = decode_hex(_hash, hash, hash_size);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::quick_verify
//       Access: Public
//  Description: Performs a quick test to ensure the file has not been
//               modified.  This test is vulnerable to people
//               maliciously attempting to fool the program (by
//               setting datestamps etc.).
//
//               Returns true if it is intact, false if it needs to be
//               redownloaded.
////////////////////////////////////////////////////////////////////
bool FileSpec::
quick_verify(const string &package_dir) const {
  string pathname = get_pathname(package_dir);
  struct stat st;
  if (stat(pathname.c_str(), &st) != 0) {
    //cerr << "file not found: " << _filename << "\n";
    return false;
  }

  if (st.st_size != _size) {
    // If the size is wrong, the file fails.
    //cerr << "size wrong: " << _filename << "\n";
    return false;
  }

  if (st.st_mtime == _timestamp) {
    // If the size is right and the timestamp is right, the file passes.
    //cerr << "file ok: " << _filename << "\n";
    return true;
  }

  //cerr << "modification time wrong: " << _filename << "\n";

  // If the size is right but the timestamp is wrong, the file
  // soft-fails.  We follow this up with a hash check.
  if (!check_hash(pathname)) {
    // Hard fail, the hash is wrong.
    //cerr << "hash check wrong: " << _filename << "\n";
    return false;
  }

  //cerr << "hash check ok: " << _filename << "\n";

  // The hash is OK after all.  Change the file's timestamp back to
  // what we expect it to be, so we can quick-verify it successfully
  // next time.
  utimbuf utb;
  utb.actime = st.st_atime;
  utb.modtime = _timestamp;
  utime(pathname.c_str(), &utb);

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::full_verify
//       Access: Public
//  Description: Performs a more thorough test to ensure the file has
//               not been modified.  This test is less vulnerable to
//               malicious attacks, since it reads and verifies the
//               entire file.
//
//               Returns true if it is intact, false if it needs to be
//               redownloaded.
////////////////////////////////////////////////////////////////////
bool FileSpec::
full_verify(const string &package_dir) const {
  string pathname = get_pathname(package_dir);
  struct stat st;
  if (stat(pathname.c_str(), &st) != 0) {
    //cerr << "file not found: " << _filename << "\n";
    return false;
  }

  if (st.st_size != _size) {
    // If the size is wrong, the file fails.
    //cerr << "size wrong: " << _filename << "\n";
    return false;
  }

  // If the size is right but the timestamp is wrong, the file
  // soft-fails.  We follow this up with a hash check.
  if (!check_hash(pathname)) {
    // Hard fail, the hash is wrong.
    //cerr << "hash check wrong: " << _filename << "\n";
    return false;
  }

  //cerr << "hash check ok: " << _filename << "\n";

  // The hash is OK.  If the timestamp is wrong, change it back to
  // what we expect it to be, so we can quick-verify it successfully
  // next time.

  if (st.st_mtime != _timestamp) {
    utimbuf utb;
    utb.actime = st.st_atime;
    utb.modtime = _timestamp;
    utime(pathname.c_str(), &utb);
  }
    
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::check_hash
//       Access: Public
//  Description: Returns true if the file has the expected md5 hash,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool FileSpec::
check_hash(const string &pathname) const {
  ifstream stream(pathname.c_str(), ios::in | ios::binary);
  if (!stream) {
    //cerr << "unable to read " << pathname << "\n";
    return false;
  }

  unsigned char md[hash_size];

  MD5_CTX ctx;
  MD5_Init(&ctx);

  static const int buffer_size = 1024;
  char buffer[buffer_size];

  stream.read(buffer, buffer_size);
  size_t count = stream.gcount();
  while (count != 0) {
    MD5_Update(&ctx, buffer, count);
    stream.read(buffer, buffer_size);
    count = stream.gcount();
  }

  MD5_Final(md, &ctx);

  return (memcmp(md, _hash, hash_size) == 0);
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::decode_hex
//       Access: Private, Static
//  Description: Decodes the hex string in source into the character
//               array in dest.  dest must have has least size bytes;
//               source must have size * 2 bytes.
//
//               Returns true on success, false if there was a non-hex
//               digit in the string.
////////////////////////////////////////////////////////////////////
bool FileSpec::
decode_hex(unsigned char *dest, const char *source, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    int high = decode_hexdigit(source[i * 2]);
    int low = decode_hexdigit(source[i * 2 + 1]);
    if (high < 0 || low < 0) {
      return false;
    }
    dest[i] = (high << 4) | low;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::encode_hex
//       Access: Private, Static
//  Description: Encodes a character array into a hex string for
//               output.  dest must have at least size * 2 bytes;
//               source must have size bytes.  The result is not
//               null-terminated.
////////////////////////////////////////////////////////////////////
void FileSpec::
encode_hex(char *dest, const unsigned char *source, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    int high = (source[i] >> 4) & 0xf;
    int low = source[i] & 0xf;
    dest[2 * i] = encode_hexdigit(high);
    dest[2 * i + 1] = encode_hexdigit(low);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: FileSpec::stream_hex
//       Access: Private, Static
//  Description: Writes the indicated buffer as a string of hex
//               characters to the given ostream.
////////////////////////////////////////////////////////////////////
void FileSpec::
stream_hex(ostream &out, const unsigned char *source, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    int high = (source[i] >> 4) & 0xf;
    int low = source[i] & 0xf;
    out.put(encode_hexdigit(high));
    out.put(encode_hexdigit(low));
  }
}