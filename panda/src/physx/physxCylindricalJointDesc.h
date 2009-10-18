// Filename: physxCylindricalJointDesc.h
// Created by:  enn0x (28Sep09)
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

#ifndef PHYSXCYLINDRICALJOINTDESC_H
#define PHYSXCYLINDRICALJOINTDESC_H

#include "pandabase.h"

#include "physxJointDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxCylindricalJointDesc
// Description : Descriptor class for sliding joint. See
//               PhysxCylindricalJoint.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxCylindricalJointDesc : public PhysxJointDesc {

PUBLISHED:
  INLINE PhysxCylindricalJointDesc();
  INLINE ~PhysxCylindricalJointDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

public:
  INLINE PhysxCylindricalJointDesc(NxCylindricalJointDesc &desc);

  NxJointDesc *ptr() const { return (NxJointDesc *)&_desc; };

private:
  NxCylindricalJointDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxJointDesc::init_type();
    register_type(_type_handle, "PhysxCylindricalJointDesc", 
                  PhysxJointDesc::get_class_type());
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

#include "physxCylindricalJointDesc.I"

#endif // PHYSXCYLINDRICALJOINTDESC_H
