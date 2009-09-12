// Filename: physxBoxShapeDesc.h
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

#ifndef PHYSXBOXSHAPEDESC_H
#define PHYSXBOXSHAPEDESC_H

#include "pandabase.h"
#include "physxShapeDesc.h"

#include "lvector3.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxBoxShapeDesc
// Description : Descriptor class for PhysxBoxShape.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxBoxShapeDesc : public PhysxShapeDesc {

PUBLISHED:
  INLINE PhysxBoxShapeDesc();
  INLINE ~PhysxBoxShapeDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_dimensions(const LVector3f &dimensions);

public:
  INLINE PhysxBoxShapeDesc(NxBoxShapeDesc &desc);

  virtual NxShapeDesc *ptr() const { return (NxShapeDesc *)&_desc; };

private:
  NxBoxShapeDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxShapeDesc::init_type();
    register_type(_type_handle, "PhysxBoxShapeDesc", 
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

#include "physxBoxShapeDesc.I"

#endif // PHYSXBOXSHAPEDESC_H
