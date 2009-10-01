// Filename: physxSphericalJointDesc.h
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

#ifndef PHYSXSPHERICALJOINTDESC_H
#define PHYSXSPHERICALJOINTDESC_H

#include "pandabase.h"

#include "physxJointDesc.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxSpringDesc;
class PhysxJointLimitDesc;

////////////////////////////////////////////////////////////////////
//       Class : PhysxSphericalJointDesc
// Description : Descriptor class for distance joint. See
//               PhysxSphericalJoint.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxSphericalJointDesc : public PhysxJointDesc {

PUBLISHED:
  INLINE PhysxSphericalJointDesc();
  INLINE ~PhysxSphericalJointDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_projection_distance(float distance);
  void set_flag(PhysxSphericalJointFlag flag, bool value);
  void set_swing_axis(const LVector3f &axis);
  void set_twist_spring(const PhysxSpringDesc &spring);
  void set_swing_spring(const PhysxSpringDesc &spring);
  void set_joint_spring(const PhysxSpringDesc &spring);
  void set_twist_limit_low(const PhysxJointLimitDesc &low);
  void set_twist_limit_high(const PhysxJointLimitDesc &high);
  void set_swing_limit(const PhysxJointLimitDesc &limit);
  void set_projection_mode(PhysxProjectionMode mode);

  float get_projection_distance() const;
  bool get_flag(PhysxSphericalJointFlag flag) const;
  LVector3f get_swing_axis() const;
  PT(PhysxSpringDesc) get_twist_spring() const;
  PT(PhysxSpringDesc) get_swing_spring() const;
  PT(PhysxSpringDesc) get_joint_spring() const;
  PT(PhysxJointLimitDesc) get_twist_limit_low() const;
  PT(PhysxJointLimitDesc) get_twist_limit_high() const;
  PT(PhysxJointLimitDesc) get_swing_limit() const;
  PhysxProjectionMode get_projection_mode() const;

public:
  INLINE PhysxSphericalJointDesc(NxSphericalJointDesc &desc);

  virtual NxJointDesc *ptr() const { return (NxJointDesc *)&_desc; };

private:
  NxSphericalJointDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxJointDesc::init_type();
    register_type(_type_handle, "PhysxSphericalJointDesc", 
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

#include "physxSphericalJointDesc.I"

#endif // PHYSXSPHERICALJOINTDESC_H
