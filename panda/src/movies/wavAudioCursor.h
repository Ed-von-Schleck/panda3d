// Filename: wavAudioCursor.h
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

#ifndef WAVAUDIOCURSOR_H
#define WAVAUDIOCURSOR_H
#ifndef HAVE_FFMPEG

#include "pandabase.h"
#include "namable.h"
#include "texture.h"
#include "pointerTo.h"

////////////////////////////////////////////////////////////////////
//       Class : WavAudioCursor
// Description : A stream that generates a sequence of audio samples.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_MOVIES WavAudioCursor : public MovieAudioCursor {
  friend class WavAudio;

  struct WavChunkHeader {
	  PN_uint8 chunk_id[4];
	  PN_uint32 chunk_size;
  };

  struct WavFormatChunk {
	  PN_uint16 type;
	  PN_uint16 channels;
	  PN_uint32 rate;
	  PN_uint32 bytes_per_second;
	  PN_uint16 block_align;
	  PN_uint16 bits_per_sample;
  };

PUBLISHED:
  WavAudioCursor(WavAudio *src);
  virtual ~WavAudioCursor();
  virtual void seek(double offset);
  
public:
  virtual void read_samples(int n, PN_int16 *data);
  INLINE bool is_valid() const;

protected:
  void cleanup();

  bool _opened;

	unsigned char _bytes_per_sample;
	unsigned int _num_samples;

  PN_uint32 _audio_buffer_size;
  PN_uint8 *_audio_buffer;

  Filename _filename;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "WavAudioCursor",
                  MovieAudioCursor::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "wavAudioCursor.I"

#endif // !HAVE_FFMPEG
#endif // WAV_AUDIO.H
