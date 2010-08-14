// Filename: renderPass.h
// Created by:  rdb (13Aug10)
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

#ifndef RENDERPASS_H
#define RENDERPASS_H

#include "typedWritableReferenceCount.h"
#include "drawableRegion.h"

////////////////////////////////////////////////////////////////////
//       Class : RenderPass
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_GOBJ RenderPass : public TypedWritableReferenceCount, public Namable, public DrawableRegion {
PUBLISHED:
  enum DrawType {
    DT_geometry = 0,
    DT_full_screen_quad,
  };

  INLINE RenderPass(const string &name, DrawType draw_type = DT_geometry);
  virtual ~RenderPass() {};

  INLINE DrawType get_draw_type() const;
  INLINE void set_draw_type(DrawType draw_type);

private:
  DrawType _draw_type;

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &me);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "RenderPass",
                  TypedWritableReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "renderPass.I"

#endif

