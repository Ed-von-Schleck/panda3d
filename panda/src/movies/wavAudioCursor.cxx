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
// as a reference on the WAV format. The lines that have been
// adapted are copyright their authors.

// FIXME: This implementation is endian independent but it requires
// testing in this aspect.

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
  _filename(src->_filename),
  _opened(false),
  _audio_buffer(0),
  _audio_buffer_size(0)
{

  _can_seek = 0;
  _can_seek_fast = true;
  _samples_read = 0;

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  Filename filename = Filename::binary_filename(src->_filename);
  PT(VirtualFile) file = vfs->get_file(filename);

  if (file == (VirtualFile *)NULL) {
    movies_cat.error()
      << "Could not find " << src->_filename << "\n";
    cleanup(); return;
  }

  istream *wav_stream = file->open_read_file(true);

  if (wav_stream == NULL){
    movies_cat.error()
      << "Could not open " << src->_filename << "\n";
    cleanup(); return;
  }

  // Read riff header
	WavChunkHeader header;
  wav_stream->read((char *)&header, sizeof(header));

  if (wav_stream->fail() || wav_stream->eof()){
    file->close_read_file(wav_stream);
    movies_cat.error() << (wav_stream->eof()?"Unexpected end of file":"Failure")
      << " while reading file: " << src->_filename << "\n";
    cleanup(); return;
  }

	if(header.chunks[0] != 'R' || header.chunks[1] != 'I' ||
	   header.chunks[2] != 'F' || header.chunks[3] != 'F') {
    file->close_read_file(wav_stream);
    movies_cat.error()
      << "Invalid file: " << src->_filename << "\n";
    cleanup(); return;
  }

	// Read riff type
	char tmp[4];
  wav_stream->read(tmp, sizeof(tmp));

  if (wav_stream->fail() || wav_stream->eof()){
    file->close_read_file(wav_stream);
    movies_cat.error() << (wav_stream->eof()?"Unexpected end of file":"Failure")
      << " while reading file: " << src->_filename << "\n";
    cleanup(); return;
  }

	if(tmp[0] != 'W' || tmp[1] != 'A' || tmp[2] != 'V' || tmp[3] != 'E') {
    file->close_read_file(wav_stream);
    movies_cat.error()
      << "Invalid riff file: " << src->_filename << "\n";
    cleanup(); return;
  }

  // Read format header
  wav_stream->read((char *)&header, sizeof(header));

  if (wav_stream->fail() || wav_stream->eof()){
    file->close_read_file(wav_stream);
    movies_cat.error() << (wav_stream->eof()?"Unexpected end of file":"Failure")
      << " while reading file: " << src->_filename << "\n";
    cleanup(); return;
  }

	if(header.chunks[0] != 'f' || header.chunks[1] != 'm' ||
	   header.chunks[2] != 't' || header.chunks[3] != ' ') {
    file->close_read_file(wav_stream);
    movies_cat.error()
      << "Invalid file format: " << src->_filename << "\n";
    cleanup(); return;
  }

  // Read format chunk
	WavFormatChunk format;
  wav_stream->read((char *)&format, sizeof(format));
  
  if (wav_stream->fail() || wav_stream->eof()){
    file->close_read_file(wav_stream);
    movies_cat.error() << (wav_stream->eof()?"Unexpected end of file":"Failure")
      << " while reading file: " << src->_filename << "\n";
    cleanup(); return;
  }

  if(format.type != 1) {
    file->close_read_file(wav_stream);
    movies_cat.error()
      << "Invalid format. WAV file " << src->_filename << "is not uncompressed PCM\n";
    cleanup(); return;
  }

  LittleEndian audio_channels(&format.channels, sizeof(format.channels));
	_audio_channels = *((PN_uint16*)(audio_channels.get_data()));

  LittleEndian bits_per_sample(&format.bits_per_sample, sizeof(format.bits_per_sample));
	_bytes_per_sample = *((PN_uint16*)(bits_per_sample.get_data())) / 8;

  LittleEndian audio_rate(&format.rate, sizeof(format.rate));
	_audio_rate = *((PN_uint16*)(audio_rate.get_data()));

	// Look for data chunk
	bool data_chunk_found = false;
  wav_stream->read((char *)&header, sizeof(header));

	while(!wav_stream->fail() && !wav_stream->eof()) {
		if(header.chunks[0] == 'd' && header.chunks[1] == 'a' &&
		   header.chunks[2] == 't' && header.chunks[3] == 'a') {
			data_chunk_found = true;
			break;
		} else {
      LittleEndian chunk_size(&header.chunk_size, sizeof(header.chunk_size));
      PN_uint32 native_chunk_size = *((PN_uint32*)(chunk_size.get_data()));

      wav_stream->seekg(native_chunk_size, std::ios::cur);
      if(!wav_stream->fail() && !wav_stream->eof()){
				break;
      }
		}
    wav_stream->read((char *)&header, sizeof(header));
	}

	if(!data_chunk_found) {
    file->close_read_file(wav_stream);
    movies_cat.error()
      << "Invalid WAV file. Couldn't find data chunk in file " << src->_filename << "\n";
    cleanup(); return;
  }

  LittleEndian audio_buffer_size(&header.chunk_size, sizeof(header.chunk_size));
  _audio_buffer_size = *((PN_uint32*)(audio_buffer_size.get_data()));

  _num_samples = _audio_buffer_size / _audio_channels / _bytes_per_sample;

  _length = _num_samples / (float)_audio_rate;
	
  // Read data chunk
  _audio_buffer = new PN_uint8 [_audio_buffer_size];
  wav_stream->read((char *)_audio_buffer, sizeof(PN_uint8)*_audio_buffer_size);

  if (wav_stream->fail() || wav_stream->eof()){
    file->close_read_file(wav_stream);
    movies_cat.error() << (wav_stream->eof()?"Unexpected end of file":"Failure")
      << " while reading file: " << src->_filename << "\n";
    cleanup(); return;
  }

  file->close_read_file(wav_stream);

  // Byte-swapping the audio data itself is not necessary because OpenAL is little
  // endian, but I'm gonna leave the code here in case we want to use it some day
  // with other sound backend.
  // Also note that enclsoing it in "ifdef WORDS_BIGENDIAN" isn't necessary
  // either, as the LittleEndian class takes care of it. It's just an
  // optimization.

  //#ifdef WORDS_BIGENDIAN
  //	if(_bytes_per_sample == 2) {
  //		PN_uint16 *data = (PN_uint16 *)_audio_buffer;
  //    for(unsigned int i = 0; i < _audio_buffer_size / 2; i++){
  //      LittleEndian word(&data[i], sizeof(data[i]));
  //	    data[i] = *((PN_uint16*)(word.get_data()));
  //    }
  //	}
  //#endif

  _opened = true;

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
  if (_opened){
    if (_num_samples < 1){
      return;
    }
    PN_uint16 desired_sample = _length * _audio_rate;
    if (desired_sample > _num_samples - 1){
      desired_sample = _num_samples - 1;
    }
    _samples_read = desired_sample;
  }
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
  if (!_opened){
    return;
  }
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
//     Function: WavAudioCursor::cleanup
//       Access: Protected
//  Description: Reset to a standard inactive state.
////////////////////////////////////////////////////////////////////
void WavAudioCursor::
cleanup() {
  if (_audio_buffer != 0){
    delete[] _audio_buffer;
    _audio_buffer = 0;
    _audio_buffer_size = 0;
  }
  _samples_read = 0;
  _opened = false;
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_FFMPEG
