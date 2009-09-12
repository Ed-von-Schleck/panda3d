// Filename: physxShapeDesc.h
// Created by:  enn0x (08Sep09)
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

#ifndef PHYSXSHAPEDESC_H
#define PHYSXSHAPEDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "lpoint3.h"
#include "lmatrix.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxShapeDesc
// Description : Descriptor class for the PhysxShape class. Shape
//               descriptors for all the different shape types are
//               derived from this class.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxShapeDesc : public TypedReferenceCount {

PUBLISHED:
  INLINE PhysxShapeDesc();
  INLINE ~PhysxShapeDesc();

  virtual void set_to_default() = 0;
  virtual bool is_valid() const = 0;

  enum PhysxShapeFlag {
    SF_trigger_on_enter      = 1<<0, // NX_TRIGGER_ON_ENTER
    SF_trigger_on_leave      = 1<<1, // NX_TRIGGER_ON_LEAVE
    SF_trigger_on_stay       = 1<<2, // NX_TRIGGER_ON_STAY
    SF_trigger_enable        = 7,    // NX_TRIGGER_ENABLE
    SF_visualization         = 1<<3, // NX_SF_VISUALIZATION
    SF_disable_collision     = 1<<4, // NX_SF_DISABLE_COLLISION
    SF_disable_raycasting    = 1<<6, // NX_SF_DISABLE_RAYCASTING
  };

  void set_name(const char *name);
  void set_trigger(bool value);
  void set_local_pos(const LPoint3f &pos);
  void set_local_mat(const LMatrix4f &mat);
  void set_local_hpr(float h, float p, float r);
  void set_skin_width(float skinWidth);
  void set_flag(const PhysxShapeFlag flag, bool value);

  bool get_flag(const PhysxShapeFlag flag) const;

  //void set_material(const PhysMaterial &material);

public:
  virtual NxShapeDesc *ptr() const = 0;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxShapeDesc", 
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

#include "physxShapeDesc.I"

#endif // PHYSXSHAPEDESC_H
