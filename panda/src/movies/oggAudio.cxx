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

#include "oggAudio.h"
#include "oggAudioCursor.h"

TypeHandle OggAudio::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: OggAudio::Constructor
//       Access: Protected
//  Description: xxx
////////////////////////////////////////////////////////////////////
OggAudio::
OggAudio(const Filename &name) :
  MovieAudio(name)
{
  _filename = name;
}

////////////////////////////////////////////////////////////////////
//     Function: OggAudio::Destructor
//       Access: Protected, Virtual
//  Description: xxx
////////////////////////////////////////////////////////////////////
OggAudio::
~OggAudio() {
}

////////////////////////////////////////////////////////////////////
//     Function: OggAudio::open
//       Access: Published, Virtual
//  Description: Open this audio, returning a MovieAudioCursor
////////////////////////////////////////////////////////////////////
PT(MovieAudioCursor) OggAudio::
open() {
  PT(OggAudioCursor) result = new OggAudioCursor(this);
  if (!result->is_valid()) {
    movies_cat.error() << "Could not open " << _filename << "\n";
    return NULL;
  } else {
    return (MovieAudioCursor*)(OggAudioCursor*)result;
  }
}

////////////////////////////////////////////////////////////////////

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
