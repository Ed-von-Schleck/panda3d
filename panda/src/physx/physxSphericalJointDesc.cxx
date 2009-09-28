// Filename: physxSphericalJointDesc.cxx
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

#include "physxSphericalJointDesc.h"
//#include "physxManager.h"
//#include "physxSpringDesc.h"
//#include "physxJointLimitDesc.h"

TypeHandle PhysxSphericalJointDesc::_type_handle;

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_projection_distance
//       Access: Published
//  Description: Set the distance above which to project joint.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_projection_distance(float distance) {

  _desc.projectionDistance = distance;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_flag
//       Access: Published
//  Description: Sets or clears a single SphericalJointFlag flag.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_flag(PhysxSphericalJointFlag flag, bool value) {

  if (value == true) {
    _desc.flags |= flag;
  }
  else {
    _desc.flags &= ~(flag);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_twist_spring
//       Access: Published
//  Description: Sets a spring that works against twisting.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_twist_spring(const PhysxSpringDesc &spring) {

  _desc.twistSpring = spring.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_swing_spring
//       Access: Published
//  Description: Sets a spring that works against swinging.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_swing_spring(const PhysxSpringDesc &spring) {

  _desc.swingSpring = spring.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_joint_spring
//       Access: Published
//  Description: Sets a spring that lets the joint get pulled apart.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_joint_spring(const PhysxSpringDesc &spring) {

  _desc.jointSpring = spring.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_swing_axis
//       Access: Published
//  Description: Set the swing limit axis defined in the joint
//               space of actor 0.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_swing_axis(const LVector3f &axis) {

  nassertv( !axis.is_nan() );
  _desc.swingAxis = PhysxManager::vec3_to_nxVec3(axis);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_projection_mode
//       Access: Published
//  Description: Use this to enable joint projection.
//               Default is PM_none.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_projection_mode(PhysxProjectionMode mode) {

  _desc.projectionMode = (NxJointProjectionMode)mode;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_twist_limit
//       Access: Published
//  Description: Limits rotation around twist axis.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_twist_limit(const PhysxJointLimitDesc &low, const PhysxJointLimitDesc &high) {

  _desc.twistLimit.low = low.desc();
  _desc.twistLimit.high = high.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJointDesc::set_swing_limit
//       Access: Published
//  Description: Limits swing of twist axis.
////////////////////////////////////////////////////////////////////
void PhysxSphericalJointDesc::
set_swing_limit(const PhysxJointLimitDesc &limit) {

  _desc.swingLimit = limit.desc();
}
*/
