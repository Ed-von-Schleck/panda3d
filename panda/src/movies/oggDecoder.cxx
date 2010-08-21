// Filename: oggAudio.cxx
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

#include "oggDecoder.h"

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

bool OggDecoder::read_packet(istream& is, ogg_sync_state* state, OggStream* stream, ogg_packet* packet) {
  int ret = 0;

  while ((ret = ogg_stream_packetout(&stream->_state, packet)) != 1) {
    ogg_page page;
    if (!read_page(is, state, &page))
      return false;

    int serial = ogg_page_serialno(&page);
    assert(_streams.find(serial) != _streams.end());
    OggStream* pageStream = _streams[serial];
    
    // Drop data for streams we're not interested in.
    if (stream->_active) {
      ret = ogg_stream_pagein(&pageStream->_state, &page);
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
    OggStream* stream = 0;

    if(ogg_page_bos(&page)) {
      // At the beginning of the stream, read headers
      // Initialize the stream, giving it the serial
      // number of the stream for this page.
      stream = new OggStream(serial);
      ret = ogg_stream_init(&stream->_state, serial);
      assert(ret == 0);
      _streams[serial] = stream;
    }

    assert(_streams.find(serial) != _streams.end());
    stream = _streams[serial];

    // Add a complete page to the bitstream
    ret = ogg_stream_pagein(&stream->_state, &page);
    assert(ret == 0);

    // Process all available header packets in the stream. When we hit
    // the first data stream we don't decode it, instead we
    // return. The caller can then choose to process whatever data
    // streams it wants to deal with.
    ogg_packet packet;
    while (!headersDone &&
	   (ret = ogg_stream_packetpeek(&stream->_state, &packet)) != 0) {
      assert(ret == 1);

      // A packet is available. If it is not a header packet we exit.
      // If it is a header packet, process it as normal.
      headersDone = headersDone || handle_vorbis_header(stream, &packet);
      if (!headersDone) {
	// Consume the packet
	ret = ogg_stream_packetout(&stream->_state, &packet);
	assert(ret == 1);
      }
    }
  } 
}


////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::finalize_decoding
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::finalize_decoding(){
  // Cleanup
  int ret = ogg_sync_clear(&state);
  assert(ret == 0);
}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::initialize_decoding
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

void OggDecoder::initialize_decoding(istream& is) {

  int ret = ogg_sync_init(&state);
  assert(ret == 0);

  // Read headers for all streams
  read_headers(is, &state);

  // Find and initialize the vorbis stream.
  audio = 0;
  for(StreamMap::iterator it = _streams.begin(); it != _streams.end(); ++it) {
    OggStream* stream = (*it).second;
    if (!audio && stream->_type == TYPE_VORBIS) {
      audio = stream;
      audio->_vorbis_decoder.init_for_data(audio);
    }
    else
      stream->_active = false;
  }

  assert(audio);
  movies_cat.info()
      << "Audio stream is " 
      << audio->_serial << " "
      << audio->_vorbis_decoder._info.channels << " channels "
      << audio->_vorbis_decoder._info.rate << "KHz"
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
    if (vorbis_synthesis(&audio->_vorbis_decoder._block, &packet) == 0) {
      int ret = vorbis_synthesis_blockin(&audio->_vorbis_decoder._dsp, &audio->_vorbis_decoder._block);
      assert(ret == 0);
    }

    float** pcm = 0;
    int samples = 0;
    while ((samples = vorbis_synthesis_pcmout(&audio->_vorbis_decoder._dsp, &pcm)) > 0) {
      if (samples > 0) {
        size_t size = samples * audio->_vorbis_decoder._info.channels;
        short* buffer = new short[size];
        short* p = buffer;
        for (int i=0;i < samples; ++i) {
          for(int j=0; j < audio->_vorbis_decoder._info.channels; ++j) {
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

      int ret = vorbis_synthesis_read(&audio->_vorbis_decoder._dsp, samples);
      assert(ret == 0);
    }

  }

}

////////////////////////////////////////////////////////////////////
//     Function: OggDecoder::decode
//       Access: Public
//  Description: handle_vorbis_header
////////////////////////////////////////////////////////////////////

bool OggDecoder::handle_vorbis_header(OggStream* stream, ogg_packet* packet) {
  int ret = vorbis_synthesis_headerin(&stream->_vorbis_decoder._info,
				      &stream->_vorbis_decoder._comment,
				      packet);
  // Unlike libtheora, libvorbis does not provide a return value to
  // indicate that we've finished loading the headers and got the
  // first data packet. To detect this I check if I already know the
  // stream type and if the vorbis_synthesis_headerin call failed.
  if (stream->_type == TYPE_VORBIS && ret == OV_ENOTVORBIS) {
    // First data packet
    return true;
  }
  else if (ret == 0) {
    stream->_type = TYPE_VORBIS;
  }
  return false;
}


#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
