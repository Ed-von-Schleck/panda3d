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

#include "oggVorbisDecoder.h"

void OggVorbisDecoder::init_for_data(OggStream* stream) {
  int ret = vorbis_synthesis_init(&stream->_vorbis_decoder._dsp, &stream->_vorbis_decoder._info);
  assert(ret == 0);
  ret = vorbis_block_init(&stream->_vorbis_decoder._dsp, &stream->_vorbis_decoder._block);
  assert(ret == 0);
}

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
