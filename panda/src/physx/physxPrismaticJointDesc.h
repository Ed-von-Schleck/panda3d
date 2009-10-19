// Filename: physxPrismaticJointDesc.h
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

#ifndef PHYSXPRISMATICJOINTDESC_H
#define PHYSXPRISMATICJOINTDESC_H

#include "pandabase.h"

#include "physxJointDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxPrismaticJointDesc
// Description : Descriptor class for prismatic joint. See
//               PhysxPrismaticJoint.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxPrismaticJointDesc : public PhysxJointDesc {

PUBLISHED:
  INLINE PhysxPrismaticJointDesc();
  INLINE ~PhysxPrismaticJointDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

public:
  NxJointDesc *ptr() const { return (NxJointDesc *)&_desc; };

private:
  NxPrismaticJointDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxJointDesc::init_type();
    register_type(_type_handle, "PhysxPrismaticJointDesc", 
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

#include "physxPrismaticJointDesc.I"

#endif // PHYSXPRISMATICJOINTDESC_H
