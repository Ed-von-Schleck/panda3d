// Filename: physxSphereShapeDesc.h
// Created by:  enn0x (11Sep09)
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

#ifndef PHYSXSPHERESHAPEDESC_H
#define PHYSXSPHERESHAPEDESC_H

#include "pandabase.h"
#include "physxShapeDesc.h"

#include "lvector3.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxSphereShapeDesc
// Description : Descriptor class for PhysxSphereShape.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxSphereShapeDesc : public PhysxShapeDesc {

PUBLISHED:
  INLINE PhysxSphereShapeDesc();
  INLINE ~PhysxSphereShapeDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_radius(float radius);

  float get_radius() const;

public:
  NxShapeDesc *ptr() const { return (NxShapeDesc *)&_desc; };

private:
  NxSphereShapeDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxShapeDesc::init_type();
    register_type(_type_handle, "PhysxSphereShapeDesc", 
                  PhysxShapeDesc::get_class_type());
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

#include "physxSphereShapeDesc.I"

#endif // PHYSXSPHERESHAPEDESC_H
