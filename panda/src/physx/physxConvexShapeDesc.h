// Filename: physxConvexShapeDesc.h
// Created by:  enn0x (14Oct09)
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

#ifndef PHYSXCONVEXSHAPEDESC_H
#define PHYSXCONVEXSHAPEDESC_H

#include "pandabase.h"

#include "physxShapeDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxConvexMesh;

////////////////////////////////////////////////////////////////////
//       Class : PhysxConvexShapeDesc
// Description : Descriptor class for PhysxConvexShape.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxConvexShapeDesc : public PhysxShapeDesc {

PUBLISHED:
  INLINE PhysxConvexShapeDesc();
  INLINE ~PhysxConvexShapeDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_mesh(PT(PhysxConvexMesh) mesh);

public:
  NxShapeDesc *ptr() const { return (NxShapeDesc *)&_desc; };

private:
  NxConvexShapeDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxShapeDesc::init_type();
    register_type(_type_handle, "PhysxConvexShapeDesc", 
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

#include "physxConvexShapeDesc.I"

#endif // PHYSXCONVEXSHAPEDESC_H
