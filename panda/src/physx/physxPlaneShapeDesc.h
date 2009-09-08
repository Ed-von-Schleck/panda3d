// Filename: physxPlaneShapeDesc.h
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

#ifndef PHYSXPLANESHAPEDESC_H
#define PHYSXPLANESHAPEDESC_H

#include "pandabase.h"
#include "physxShapeDesc.h"

#include "lvector3.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxPlaneShapeDesc
// Description : Descriptor class for PhysxPlaneShape.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxPlaneShapeDesc : public PhysxShapeDesc {

PUBLISHED:
  INLINE PhysxPlaneShapeDesc();
  INLINE ~PhysxPlaneShapeDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_plane(const LVector3f &normal, float d);

public:
  INLINE PhysxPlaneShapeDesc(NxPlaneShapeDesc &desc);

  virtual NxShapeDesc *ptr() { return (NxShapeDesc *)&_desc; };

private:
  NxPlaneShapeDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxShapeDesc::init_type();
    register_type(_type_handle, "PhysxPlaneShapeDesc", 
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

#include "physxPlaneShapeDesc.I"

#endif // PHYSXPLANESHAPEDESC_H
