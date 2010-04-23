// Filename: wavAudio.cxx
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

#ifndef HAVE_FFMPEG

#include "wavAudio.h"
#include "wavAudioCursor.h"

TypeHandle WavAudio::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: WavAudio::Constructor
//       Access: Protected
//  Description: xxx
////////////////////////////////////////////////////////////////////
WavAudio::
WavAudio(const Filename &name) :
  MovieAudio(name)
{
  _filename = name;
}

////////////////////////////////////////////////////////////////////
//     Function: WavAudio::Destructor
//       Access: Protected, Virtual
//  Description: xxx
////////////////////////////////////////////////////////////////////
WavAudio::
~WavAudio() {
}

////////////////////////////////////////////////////////////////////
//     Function: WavAudio::open
//       Access: Published, Virtual
//  Description: Open this audio, returning a MovieAudioCursor
////////////////////////////////////////////////////////////////////
PT(MovieAudioCursor) WavAudio::
open() {
  PT(WavAudioCursor) result = new WavAudioCursor(this);
  if (!result->is_valid()) {
    movies_cat.error() << "Could not open " << _filename << "\n";
    return NULL;
  } else {
    return (MovieAudioCursor*)(WavAudioCursor*)result;
  }
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_FFMPEG
