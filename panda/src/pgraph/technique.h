// Filename: technique.h
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

#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include "pointerTo.h"
#include "pvector.h"
#include "renderPass.h"
#include "typedWritableReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : Technique
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_GOBJ Technique : public TypedWritableReferenceCount {
PUBLISHED:
  INLINE Technique();
  INLINE int get_num_passes() const;
  INLINE PT(RenderPass) get_pass(int i) const;
  MAKE_SEQ(get_passes, get_num_passes, get_pass);
  void set_pass(int i, RenderPass *pass);
  void add_pass(RenderPass *pass);
  void remove_pass(int i);
  void clear_pass();

private:
   typedef pvector<PT(RenderPass)> RenderPasses;
   RenderPasses _passes;

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &me);
  virtual int complete_pointers(TypedWritable **plist, BamReader *manager);

  virtual void write(ostream &out, int indent_level) const;

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "Technique",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "technique.I"

#endif

