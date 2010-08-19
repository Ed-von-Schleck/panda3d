// Filename: oggAudioCursor.h
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

#ifndef OGGAUDIOCURSOR_H
#define OGGAUDIOCURSOR_H
#ifndef HAVE_FFMPEG
#ifdef HAVE_OGG
#ifdef HAVE_VORBIS

#include "pandabase.h"
#include "namable.h"
#include "texture.h"
#include "pointerTo.h"
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <iostream>
#include <sstream>

enum StreamType {
  TYPE_VORBIS,
  TYPE_THEORA,
  TYPE_UNKNOWN
};

////////////////////////////////////////////////////////////////////
//       Class : OggDecoder
// Description : Helper class that abtracts a Vorbis stream.
////////////////////////////////////////////////////////////////////

class VorbisStream
{
public:
  int mSerial;
  ogg_stream_state mState;
  StreamType mType;
  bool mActive;

  vorbis_info mInfo;
  vorbis_comment mComment;
  vorbis_dsp_state mDsp;
  vorbis_block mBlock;

public:
  VorbisStream(int serial = -1) : 
    mSerial(serial),
    mType(TYPE_UNKNOWN),
    mActive(true)
  { 
    vorbis_info_init(&mInfo);
    vorbis_comment_init(&mComment);
  }

  ~VorbisStream() {
    int ret = ogg_stream_clear(&mState);
    assert(ret == 0);
  }
};

typedef map<int, VorbisStream*> StreamMap; 

////////////////////////////////////////////////////////////////////
//       Class : OggDecoder
// Description : Helper class that encapsulates OGG handling
////////////////////////////////////////////////////////////////////

class OggDecoder
{
public:
  StreamMap mStreams;  
  ogg_sync_state state;
  VorbisStream* audio;
private:
  bool handle_vorbis_header(VorbisStream* stream, ogg_packet* packet);
  void read_headers(istream& stream, ogg_sync_state* state);

  bool read_page(istream& stream, ogg_sync_state* state, ogg_page* page);
  bool read_packet(istream& is, ogg_sync_state* state, VorbisStream* stream, ogg_packet* packet);

public:
  OggDecoder()
  {
    audio = 0;
  }

  ~OggDecoder() {

  }
  void initialize_playback(istream& is);
  void decode(istream& is, vector<PN_int16>& os, int &stream_size);
  void finalize_playback();

};

////////////////////////////////////////////////////////////////////
//       Class : OggAudioCursor
// Description : A stream that generates a sequence of audio samples.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_MOVIES OggAudioCursor : public MovieAudioCursor {
  friend class OggAudio;

PUBLISHED:
  OggAudioCursor(OggAudio *src);
  virtual ~OggAudioCursor();
  virtual void seek(double offset);
  
public:
  virtual void read_samples(int n, PN_int16 *data);
  INLINE bool is_valid() const;

protected:
  void cleanup();

  bool _opened;

  OggDecoder decoder;
  istream *_ogg_stream;

	unsigned char _bytes_per_sample;
	unsigned int _num_samples;

  vector<PN_int16> _audio_buffer;

  Filename _filename;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "OggAudioCursor",
                  MovieAudioCursor::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "oggAudioCursor.I"

#endif // !HAVE_VORBIS
#endif // !HAVE_OGG
#endif // !HAVE_FFMPEG
#endif // OGG_AUDIO.H
