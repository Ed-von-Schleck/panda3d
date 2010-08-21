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

#ifndef OGGDECODER_H
#define OGGDECODER_H

#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

#include "oggVorbisDecoder.h"
#include "oggStream.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>

// Since libogg and libvorbis are undocumented, the OGG/Vorbis loading code
// is based on a series of blog posts by Chris Double and his example ogg
// player "plogg".
// Thus, although heavily refactored, the lines that have been lifted from
// his BSD-licensed code are also (c) Chris Double.

////////////////////////////////////////////////////////////////////
//       Class : OggDecoder
// Description : Helper class that encapsulates OGG handling
////////////////////////////////////////////////////////////////////

class OggDecoder
{
public:
  StreamMap _streams;  
  ogg_sync_state state;
  OggStream* audio;
private:
  bool handle_vorbis_header(OggStream* stream, ogg_packet* packet);
  void read_headers(istream& stream, ogg_sync_state* state);

  bool read_page(istream& stream, ogg_sync_state* state, ogg_page* page);
  bool read_packet(istream& is, ogg_sync_state* state, OggStream* stream, ogg_packet* packet);

public:
  OggDecoder():
      audio(0)
  {
  }

  ~OggDecoder() {
  }

  void initialize_decoding(istream& is);
  void decode(istream& is, vector<PN_int16>& os, int &stream_size);
  void finalize_decoding();

};

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG

#endif //OGGDECODER_H
