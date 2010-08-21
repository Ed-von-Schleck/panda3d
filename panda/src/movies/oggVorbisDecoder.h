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

#ifndef OGGVORBISDECODER_H
#define OGGVORBISDECODER_H

#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

class OggStream;

#include <ogg/ogg.h>
#include <vorbis/codec.h>

////////////////////////////////////////////////////////////////////
//       Class : OggDecoder
// Description : Class that represents the state of a OGG-contained
//               vorbis stream being decoded.
////////////////////////////////////////////////////////////////////

class OggVorbisDecoder {
public:
  vorbis_info _info;
  vorbis_comment _comment;
  vorbis_dsp_state _dsp;
  vorbis_block _block;

public:
  OggVorbisDecoder()
  {
    vorbis_info_init(&_info);
    vorbis_comment_init(&_comment);    
  }

  void init_for_data(OggStream* stream);
};

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG

#endif //OGGVORBISDECODER_H
