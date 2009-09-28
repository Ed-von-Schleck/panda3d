// Filename: physxRevoluteJointDesc.h
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

#ifndef PHYSXREVOLUTEJOINTDESC_H
#define PHYSXREVOLUTEJOINTDESC_H

#include "pandabase.h"

#include "physxJointDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxRevoluteJointDesc
// Description : Descriptor class for distance joint. See
//               PhysxRevoluteJoint.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxRevoluteJointDesc : public PhysxJointDesc {

PUBLISHED:
  INLINE PhysxRevoluteJointDesc();
  INLINE ~PhysxRevoluteJointDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  //void set_projection_distance(float distance);
  //void set_projection_angle(float angle);
  //void set_spring(const PhysxSpringDesc &spring);
  //void set_flag(PhysxRevoluteJointFlag flag, bool value);
  //void set_motor(const PhysxMotorDesc &motor);
  //void set_limit(const PhysxJointLimitDesc &low, const PhysxJointLimitDesc &high);
  //void set_projection_mode(PhysxProjectionMode mode);

  // TODO: getter

public:
  INLINE PhysxRevoluteJointDesc(NxRevoluteJointDesc &desc);

  virtual NxJointDesc *ptr() const { return (NxJointDesc *)&_desc; };

private:
  NxRevoluteJointDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxJointDesc::init_type();
    register_type(_type_handle, "PhysxRevoluteJointDesc", 
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

#include "physxRevoluteJointDesc.I"

#endif // PHYSXREVOLUTEJOINTDESC_H
