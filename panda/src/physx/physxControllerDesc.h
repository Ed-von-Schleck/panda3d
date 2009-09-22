// Filename: physxControllerDesc.h
// Created by:  enn0x (22Sep09)
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

#ifndef PHYSXCONTROLLERDESC_H
#define PHYSXCONTROLLERDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"
#include "lpoint3.h"

#include "NoMinMax.h"
#include "NxPhysics.h"
#include "NxController.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxControllerDesc
// Description : Descriptor class for a character controller.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxControllerDesc : public TypedReferenceCount {

PUBLISHED:
  INLINE PhysxControllerDesc();
  INLINE ~PhysxControllerDesc();

  virtual void set_to_default() = 0;
  virtual bool is_valid() const = 0;

  void set_pos(const LPoint3f &pos);
  void set_slope_limit(float slopeLimit);
  void set_skin_width(float skinWidth);
  void set_step_offset(float setpOffset);
  void set_interaction_flag(bool interactionFlag);

  LPoint3f get_pos() const;
  float get_slope_limit() const;
  float get_skin_width() const;
  float get_step_offset() const;
  bool get_interaction_flag() const;

public:
  virtual NxControllerDesc *ptr() const = 0;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxControllerDesc", 
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "physxControllerDesc.I"

#endif // PHYSXCONTROLLERDESC_H
