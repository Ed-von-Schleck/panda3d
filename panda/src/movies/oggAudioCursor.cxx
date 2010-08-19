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

TypeHandle OggAudioCursor::_type_handle;

#include <cassert>
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

// The OGG/Vorbis loading code is based on a series of blog posts by Chris Double.
// Thus, the lines that have been lifted from his BSD-licensed code are (c) Chris Double.

// Note: This class is WIP for P2, styles must be harmonized, the cleanup code is
// probably lacking and more error handling and debug messages must be added.

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::read_page
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

bool OggDecoder::read_page(istream& stream, ogg_sync_state* state, ogg_page* page) {
  int ret = 0;

  // If we've hit end of file we still need to continue processing
  // any remaining pages that we've got buffered.
  if (!stream.good())
    return ogg_sync_pageout(state, page) == 1;

  while((ret = ogg_sync_pageout(state, page)) != 1) {
    // Returns a buffer that can be written too
    // with the given size. This buffer is stored
    // in the ogg synchronisation structure.
    char* buffer = ogg_sync_buffer(state, 4096);
    assert(buffer);

    // Read from the file into the buffer
    stream.read(buffer, 4096);
    int bytes = stream.gcount();
    if (bytes == 0) {
      // End of file. 
      continue;
    }

    // Update the synchronisation layer with the number
    // of bytes written to the buffer
    ret = ogg_sync_wrote(state, bytes);
    assert(ret == 0);
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::read_packet
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////

bool OggDecoder::read_packet(istream& is, ogg_sync_state* state, VorbisStream* stream, ogg_packet* packet) {
  int ret = 0;

  while ((ret = ogg_stream_packetout(&stream->mState, packet)) != 1) {
    ogg_page page;
    if (!read_page(is, state, &page))
      return false;

    int serial = ogg_page_serialno(&page);
    assert(mStreams.find(serial) != mStreams.end());
    VorbisStream* pageStream = mStreams[serial];
    
    // Drop data for streams we're not interested in.
    if (stream->mActive) {
      ret = ogg_stream_pagein(&pageStream->mState, &page);
      assert(ret == 0);
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::read_headers
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::read_headers(istream& stream, ogg_sync_state* state) {
  ogg_page page;

  bool headersDone = false;
  while (!headersDone && read_page(stream, state, &page)) {
    int ret = 0;
    int serial = ogg_page_serialno(&page);
    VorbisStream* stream = 0;

    if(ogg_page_bos(&page)) {
      // At the beginning of the stream, read headers
      // Initialize the stream, giving it the serial
      // number of the stream for this page.
      stream = new VorbisStream(serial);
      ret = ogg_stream_init(&stream->mState, serial);
      assert(ret == 0);
      mStreams[serial] = stream;
    }

    assert(mStreams.find(serial) != mStreams.end());
    stream = mStreams[serial];

    // Add a complete page to the bitstream
    ret = ogg_stream_pagein(&stream->mState, &page);
    assert(ret == 0);

    // Process all available header packets in the stream. When we hit
    // the first data stream we don't decode it, instead we
    // return. The caller can then choose to process whatever data
    // streams it wants to deal with.
    ogg_packet packet;
    while (!headersDone &&
	   (ret = ogg_stream_packetpeek(&stream->mState, &packet)) != 0) {
      assert(ret == 1);

      // A packet is available. If it is not a header packet we exit.
      // If it is a header packet, process it as normal.
      headersDone = headersDone || handle_vorbis_header(stream, &packet);
      if (!headersDone) {
	// Consume the packet
	ret = ogg_stream_packetout(&stream->mState, &packet);
	assert(ret == 1);
      }
    }
  } 
}


////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::finalize_playback
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::finalize_playback(){
  // Cleanup
  int ret = ogg_sync_clear(&state);
  assert(ret == 0);
}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::initialize_playback
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::initialize_playback(istream& is) {

  int ret = ogg_sync_init(&state);
  assert(ret == 0);

  // Read headers for all streams
  read_headers(is, &state);

  // Find and initialize the vorbis stream.
  audio = 0;
  for(StreamMap::iterator it = mStreams.begin(); it != mStreams.end(); ++it) {
    VorbisStream* stream = (*it).second;
    if (!audio && stream->mType == TYPE_VORBIS) {
      audio = stream;
      int ret = vorbis_synthesis_init(&audio->mDsp, &audio->mInfo);
      assert(ret == 0);
      ret = vorbis_block_init(&audio->mDsp, &audio->mBlock);
      assert(ret == 0);
    }
    else
      stream->mActive = false;
  }

  assert(audio);

  cout << "Audio stream is " 
       << audio->mSerial << " "
       << audio->mInfo.channels << " channels "
       << audio->mInfo.rate << "KHz"
       << endl;

}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::decode
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::decode(istream& is, vector<PN_int16>& os, int &stream_size){
  // Read audio packets, sending audio data to the sound hardware.
  // When it's time to display a frame, decode the frame and display it.
  ogg_packet packet;
  stream_size = 0;
  while (read_packet(is, &state, audio, &packet)) {
    if (vorbis_synthesis(&audio->mBlock, &packet) == 0) {
      int ret = vorbis_synthesis_blockin(&audio->mDsp, &audio->mBlock);
      assert(ret == 0);
    }

    float** pcm = 0;
    int samples = 0;
    while ((samples = vorbis_synthesis_pcmout(&audio->mDsp, &pcm)) > 0) {
      if (samples > 0) {
        size_t size = samples * audio->mInfo.channels;
        short* buffer = new short[size];
        short* p = buffer;
        for (int i=0;i < samples; ++i) {
          for(int j=0; j < audio->mInfo.channels; ++j) {
            int v = static_cast<int>(floorf(0.5 + pcm[j][i]*32767.0));
            if (v > 32767) v = 32767;
            if (v <-32768) v = -32768;
            *p++ = v;
            os.push_back(v);
          }
        }
        stream_size += samples;
        delete[] buffer;
      }

      int ret = vorbis_synthesis_read(&audio->mDsp, samples);
      assert(ret == 0);
    }

  }

}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::decode
//       Access: Public
//  Description: handle_vorbis_header
////////////////////////////////////////////////////////////////////

bool OggDecoder::handle_vorbis_header(VorbisStream* stream, ogg_packet* packet) {
  int ret = vorbis_synthesis_headerin(&stream->mInfo,
				      &stream->mComment,
				      packet);
  // Unlike libtheora, libvorbis does not provide a return value to
  // indicate that we've finished loading the headers and got the
  // first data packet. To detect this I check if I already know the
  // stream type and if the vorbis_synthesis_headerin call failed.
  if (stream->mType == TYPE_VORBIS && ret == OV_ENOTVORBIS) {
    // First data packet
    return true;
  }
  else if (ret == 0) {
    stream->mType = TYPE_VORBIS;
  }
  return false;
}

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

  decoder.initialize_playback(*_ogg_stream);

  if (decoder.audio == NULL){
    movies_cat.error()
      << "Could not initialize stream " << src->_filename << "\n";
    cleanup(); return;
  }

	_audio_channels = decoder.audio->mInfo.channels;
	_bytes_per_sample = 2;
	_audio_rate = decoder.audio->mInfo.rate;

  int size;
  decoder.decode (*_ogg_stream, _audio_buffer, size);

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

  for(StreamMap::iterator it = decoder.mStreams.begin();
      it != decoder.mStreams.end(); ++it)
  {
    VorbisStream* stream = (*it).second;
    delete stream;
  }
  _audio_buffer.clear();
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
