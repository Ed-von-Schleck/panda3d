// Filename: effect.h
// Created by: Xidram (13Aug10)
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

#ifndef EFFECT_H
#define EFFECT_H

#include "bamCacheRecord.h"
#include "internalName.h"
#include "pandaNode.h"
#include "pmap.h"
#include "technique.h"

////////////////////////////////////////////////////////////////////
//       Class : Effect
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH Effect : public PandaNode {
PUBLISHED:
  INLINE Effect(const string &name = string());
  virtual ~Effect() {};

  virtual bool read(const Filename &fullpath, BamCacheRecord *record = NULL);

  INLINE bool has_technique(CPT(InternalName) name);
  INLINE CPT(Technique) get_technique(CPT(InternalName) name) const;
  INLINE void set_technique(CPT(InternalName) name, PT(Technique) technique);
  INLINE void remove_technique(CPT(InternalName) name);

  INLINE bool has_filename() const;
  INLINE const Filename &get_filename() const;
  INLINE void set_filename(const Filename &filename);
  INLINE void clear_filename();

  INLINE bool has_fullpath() const;
  INLINE const Filename &get_fullpath() const;
  INLINE void set_fullpath(const Filename &fullpath);
  INLINE void clear_fullpath();

  INLINE static PT(Effect) make_effect();

public:
  virtual bool cull_callback(CullTraverser *trav, CullTraverserData &data);

  virtual void output(ostream &out) const;
  virtual void write(ostream &out, int indent_level) const;

private:
  typedef phash_map<CPT(InternalName), PT(Technique)> Techniques;
  Techniques _techniques;

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &me);
  virtual int complete_pointers(TypedWritable **plist, BamReader *manager);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

protected:
  Filename _filename;
  Filename _fullpath;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "Effect",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "effect.I"

#endif

