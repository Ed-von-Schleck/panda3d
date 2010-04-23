// Filename: wavAudioCursor.cxx
// Created by: gogg (20Mar2010)
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

// Acknowledgement: I used the BSD-licensed DromeAudio audio library
// as a reference on the WAV format. The lines that may have been
// adapted are copyright their authors.

// FIXME: This implementation assumes little-endianness.

// Format support: 8 and 16-bit uncompressed PCM wav.

#ifndef HAVE_FFMPEG

#include "wavAudioCursor.h"
#include "virtualFileSystem.h"

TypeHandle WavAudioCursor::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: WavAudioCursor::Constructor
//       Access: Protected
//  Description: xxx
////////////////////////////////////////////////////////////////////
WavAudioCursor::
WavAudioCursor(WavAudio *src) :
  MovieAudioCursor(src),
  _filename(src->_filename)
{

  _can_seek = false;
  _can_seek_fast = false;
  _samples_read = 0;

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  Filename filename = Filename::binary_filename(src->_filename);
  PT(VirtualFile) file = vfs->get_file(filename);

  if (file == (VirtualFile *)NULL) {
    movies_cat.error()
      << "Could not find " << src->_filename << "\n";
    cleanup();
    return;
  }

  istream *str = file->open_read_file(true);

  if (str == NULL){
    movies_cat.error()
      << "Could not open " << src->_filename << "\n";
    cleanup();
    return;
  }

  // Read riff header
	WavChunkHeader hdr;
  str->read((char *)&hdr, sizeof(hdr));

	if(hdr.chunk_id[0] != 'R' || hdr.chunk_id[1] != 'I' ||
	   hdr.chunk_id[2] != 'F' || hdr.chunk_id[3] != 'F') {
    file->close_read_file(str);
    movies_cat.error()
      << "Invalid file: " << src->_filename << "\n";
    cleanup();
    return;
  }

	// Read riff type
	char tmp[4];
  str->read(tmp, sizeof(tmp));

	if(tmp[0] != 'W' || tmp[1] != 'A' || tmp[2] != 'V' || tmp[3] != 'E') {
    file->close_read_file(str);
    movies_cat.error()
      << "Invalid riff file: " << src->_filename << "\n";
    cleanup();
    return;
  }

  // Read format header
  str->read((char *)&hdr, sizeof(hdr));

	if(hdr.chunk_id[0] != 'f' || hdr.chunk_id[1] != 'm' ||
	   hdr.chunk_id[2] != 't' || hdr.chunk_id[3] != ' ') {
    file->close_read_file(str);
    movies_cat.error()
      << "Invalid file format: " << src->_filename << "\n";
    cleanup();
    return;
  }

  // Read format chunk
	WavFormatChunk fmt;
  str->read((char *)&fmt, sizeof(fmt));

  if(fmt.type != 1) {
    file->close_read_file(str);
    movies_cat.error()
      << "Invalid format. WAV file " << src->_filename << "is not uncompressed PCM\n";
    cleanup();
    return;
  }

	_audio_channels = fmt.channels;
	_bytes_per_sample = fmt.bits_per_sample / 8;
	_audio_rate = fmt.rate;

	// Look for data chunk
	bool dataFound = false;
  str->read((char *)&hdr, sizeof(hdr));

	while(!str->fail() && !str->eof()) {
		if(hdr.chunk_id[0] == 'd' && hdr.chunk_id[1] == 'a' &&
		   hdr.chunk_id[2] == 't' && hdr.chunk_id[3] == 'a') {
			dataFound = true;
			break;
		} else {
      str->seekg(hdr.chunk_size, std::ios::cur);
      if(!str->fail() && !str->eof()){
				break;
      }
		}
    str->read((char *)&hdr, sizeof(hdr));
	}

	if(!dataFound) {
    file->close_read_file(str);
    movies_cat.error()
      << "Invalid WAV file. Couldn't find data chunk in file " << src->_filename << "\n";
    cleanup();
    return;
  }

	_audio_buffer_size = hdr.chunk_size;
	_num_samples = _audio_buffer_size / _audio_channels / _bytes_per_sample;

  _length = _num_samples / (float)_audio_rate;
	
  // Read data chunk
  _audio_buffer = new PN_uint8 [_audio_buffer_size];
  str->read((char *)_audio_buffer, sizeof(PN_uint8)*_audio_buffer_size);

}

////////////////////////////////////////////////////////////////////
//     Function: WavAudioCursor::Destructor
//       Access: Protected, Virtual
//  Description: xxx
////////////////////////////////////////////////////////////////////
WavAudioCursor::
~WavAudioCursor() {
  cleanup();
}


////////////////////////////////////////////////////////////////////
//     Function: WavAudioCursor::seek
//       Access: Protected
//  Description: Unimplemented. Should seek to a target location.
//               Afterward, the packet_time is guaranteed to be less
//               than or equal to the specified time.
////////////////////////////////////////////////////////////////////
void WavAudioCursor::
seek(double t) {

}

////////////////////////////////////////////////////////////////////
//     Function: WavAudioCursor::read_samples
//       Access: Public, Virtual
//  Description: Read audio samples from the stream.  N is the
//               number of samples you wish to read.  Your buffer
//               must be equal in size to N * channels.
//               Multiple-channel audio will be interleaved.
////////////////////////////////////////////////////////////////////
void WavAudioCursor::
read_samples(int n, PN_int16 *data) {

  for (int s = 0; s < n; s++){

    if (_samples_read + 1 > _num_samples){
      return;
    }

	  if(_bytes_per_sample == 2) {
      for (int c = 0; c < _audio_channels; c++){
	      PN_uint16 *sample = (PN_uint16 *)_audio_buffer + _samples_read * _audio_channels + c;
	      data[s*_audio_channels+c] = *sample;
      }
      if (_audio_channels > 0){
        _samples_read++;
      }
    } else if(_bytes_per_sample == 1) {
      for (int c = 0; c < _audio_channels; c++){
	      PN_uint8 *sample = (PN_uint8 *)_audio_buffer + _samples_read * _audio_channels + c;
        PN_uint16 converted_sample = (PN_uint16)*sample;
	      data[s*_audio_channels+c] = (converted_sample - 128) << 8;
      }
      if (_audio_channels > 0){
        _samples_read++;
      }
    } else {
      movies_cat.error()
        << "Unsupported number of bytes per sample reading wav file: " << _bytes_per_sample
        << "\n";
      cleanup();
      return;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: FfmpegAudioCursor::cleanup
//       Access: Protected
//  Description: Reset to a standard inactive state.
////////////////////////////////////////////////////////////////////
void WavAudioCursor::
cleanup() {
  delete[] _audio_buffer;
  _audio_buffer_size = 0;
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_FFMPEG
