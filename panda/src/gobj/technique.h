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
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : Technique
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_GOBJ Technique : public TypedReferenceCount {
PUBLISHED:
  INLINE int get_num_render_passes() const;
  INLINE CPT(RenderPass) get_render_pass(int i) const;
  MAKE_SEQ(get_render_passes, get_num_render_passes, get_render_pass);
  INLINE PT(RenderPass) modify_render_pass(int i);
  void set_render_pass(int i, const RenderPass *pass);
  void add_render_pass(const RenderPass *pass);
  void remove_render_pass(int i);
  void clear_render_pass();

private:
  pvector<PT(RenderPass)> _render_passes;

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

