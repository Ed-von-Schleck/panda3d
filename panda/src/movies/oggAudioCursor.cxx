// Filename: oggAudioCursor.cxx
// Created by: gogg (14Aug2010)
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

#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

#include "oggAudioCursor.h"
#include "virtualFileSystem.h"
#include "oggDecoder.h"

TypeHandle OggAudioCursor::_type_handle;

#include <cassert>
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

////////////////////////////////////////////////////////////////////
//     Function: OggAudioCursor::Constructor
//       Access: Protected
//  Description: xxx
////////////////////////////////////////////////////////////////////
OggAudioCursor::
OggAudioCursor(OggAudio *src) :
  MovieAudioCursor(src),
  _filename(src->_filename),
  _opened(false),
  _ogg_stream(0)
{

  _can_seek = 0;
  _can_seek_fast = false;
  _samples_read = 0;

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  Filename filename = Filename::binary_filename(src->_filename);
  PT(VirtualFile) file = vfs->get_file(filename);

  if (file == (VirtualFile *)NULL) {
    movies_cat.error()
      << "Could not find " << src->_filename << "\n";
    cleanup(); return;
  }

  _ogg_stream = file->open_read_file(true);

  if (_ogg_stream == NULL){
    movies_cat.error()
      << "Could not open " << src->_filename << "\n";
    cleanup(); return;
  }

  decoder.initialize_decoding(*_ogg_stream);

  if (decoder.audio == NULL){
    movies_cat.error()
      << "Could not initialize stream " << src->_filename << "\n";
    cleanup(); return;
  }

	_audio_channels = decoder.audio->_vorbis_decoder._info.channels;
	_bytes_per_sample = 2;
	_audio_rate = decoder.audio->_vorbis_decoder._info.rate;

  int size;
  decoder.decode (*_ogg_stream, _audio_buffer, size);
  decoder.finalize_decoding();

  _num_samples = size;
  _length = size / (float)_audio_rate;

  file->close_read_file(_ogg_stream);
  _opened = true;

}

////////////////////////////////////////////////////////////////////
//     Function: OggAudioCursor::Destructor
//       Access: Protected, Virtual
//  Description: xxx
////////////////////////////////////////////////////////////////////
OggAudioCursor::
~OggAudioCursor() {
  cleanup();
}


////////////////////////////////////////////////////////////////////
//     Function: OggAudioCursor::seek
//       Access: Protected
//  Description: Unimplemented. Should seek to a target location.
//               Afterward, the packet_time is guaranteed to be less
//               than or equal to the specified time.
////////////////////////////////////////////////////////////////////
void OggAudioCursor::
seek(double t) {
}

////////////////////////////////////////////////////////////////////
//     Function: OggAudioCursor::read_samples
//       Access: Public, Virtual
//  Description: Read audio samples from the stream.  N is the
//               number of samples you wish to read.  Your buffer
//               must be equal in size to N * channels.
//               Multiple-channel audio will be interleaved.
////////////////////////////////////////////////////////////////////
void OggAudioCursor::
read_samples(int n, PN_int16 *data) {
  if (!_opened){
    return;
  }
  for (int s = 0; s < n; s++){

    if (_samples_read + 1 > _num_samples){
      return;
    }

    for (int c = 0; c < _audio_channels; c++){
      data[s*_audio_channels+c] = _audio_buffer[_samples_read * _audio_channels + c];
    }
    _samples_read++;

  }
}

////////////////////////////////////////////////////////////////////
//     Function: OggAudioCursor::cleanup
//       Access: Protected
//  Description: Reset to a standard inactive state.
////////////////////////////////////////////////////////////////////
void OggAudioCursor::
cleanup() {

  for(StreamMap::iterator it = decoder._streams.begin();
      it != decoder._streams.end(); ++it)
  {
    OggStream* stream = (*it).second;
    delete stream;
  }
  _audio_buffer.clear();
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
