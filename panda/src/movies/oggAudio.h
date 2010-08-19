// Filename: oggAudio.h
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

#ifndef OGGAUDIO_H
#define OGGAUDIO_H
#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

#include "movieAudio.h"

class OggAudioCursor;

////////////////////////////////////////////////////////////////////
//       Class : OggAudio
// Description : A stream that generates a sequence of audio samples.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_MOVIES OggAudio : public MovieAudio {

PUBLISHED:
  OggAudio(const Filename &name);
  virtual ~OggAudio();
  virtual PT(MovieAudioCursor) open();

 private:
  friend class OggAudioCursor;
  
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "OggAudio",
                  MovieAudio::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "oggAudio.I"

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
#endif // OGG_AUDIO.H
