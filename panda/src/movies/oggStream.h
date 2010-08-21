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

#ifndef OGGSTREAM_H
#define OGGSTREAM_H

#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

#include <ogg/ogg.h>
#include <vorbis/codec.h>

#include "oggVorbisDecoder.h"

enum StreamType {
  TYPE_VORBIS,
  TYPE_THEORA,
  TYPE_UNKNOWN
};

////////////////////////////////////////////////////////////////////
//       Class : OggDecoder
// Description : Helper class that abtracts a Ogg stream that could
//               be Vorbis (done) or Theora (to be implemented).
////////////////////////////////////////////////////////////////////

class OggStream
{
public:
  int _serial;
  ogg_stream_state _state;
  StreamType _type;
  bool _active;

  OggVorbisDecoder _vorbis_decoder;

public:
  OggStream(int serial = -1) : 
    _serial(serial),
    _type(TYPE_UNKNOWN),
    _active(true)
  { 
  }

  ~OggStream() {
    int ret = ogg_stream_clear(&_state);
    assert(ret == 0);
  }
};

typedef map<int, OggStream*> StreamMap; 

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG

#endif // OGGSTREAM_H
