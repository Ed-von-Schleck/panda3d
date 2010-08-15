// Filename: effectFile.h
// Created by: Xidram (15Aug10)
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

#ifndef EFFECTFILE_H
#define EFFECTFILE_H

#include "effect.h"
#include "pandabase.h"
#include "referenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : EffectFile
// Description : A node of this type is created automatically at the
//               root of each effect file that is loaded.  It may
//               eventually contain some information about the
//               contents of the effect; at the moment, it contains no
//               special information, but can be used as a flag to
//               indicate the presence of a loaded effect file.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH EffectFile : public Effect {
PUBLISHED:
  INLINE EffectFile(const string &name);

  INLINE int get_effect_ref_count() const;

  INLINE const Filename &get_fullpath() const;
  INLINE void set_fullpath(const Filename &fullpath);

  // This class is used to unify references to the same effect.
  class EffectReference : public ReferenceCount {
  PUBLISHED:
    INLINE EffectReference();
  };

  INLINE EffectReference *get_reference() const;
  void set_reference(EffectReference *ref);

protected:
  INLINE EffectFile(const EffectFile &copy);

public:
  virtual PandaNode *make_copy() const;

private:
  Filename _fullpath;
  PT(EffectReference) _reference;

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &dg);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    Effect::init_type();
    register_type(_type_handle, "EffectFile",
                  Effect::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "effectFile.I"

#endif


