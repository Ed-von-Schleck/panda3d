// Filename: physxRevoluteJointDesc.cxx
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

#include "physxRevoluteJointDesc.h"
//#include "physxSpringDesc.h"
//#include "physxMotorDesc.h"
//#include "physxJointLimitDesc.h"

TypeHandle PhysxRevoluteJointDesc::_type_handle;

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_projection_distance
//       Access: Published
//  Description: Sets the distance beyond which the joint is
//               projected.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_projection_distance(float distance) {

  _desc.projectionDistance = distance;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_projection_angle
//       Access: Published
//  Description: Sets the angle beyond which the joint is projected.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_projection_angle(float angle) {

  _desc.projectionAngle = angle;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_spring
//       Access: Published
//  Description: Sets an aptional spring. 
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_spring(const PhysxSpringDesc &spring) {

  _desc.spring = spring.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_flag
//       Access: Published
//  Description: Sets or clears a single RevoluteJointFlag flag.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_flag(PhysxRevoluteJointFlag flag, bool value) {

  if (value == true) {
    _desc.flags |= flag;
  }
  else {
    _desc.flags &= ~(flag);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_projection_mode
//       Access: Published
//  Description: Use this to enable joint projection.
//               Default is PM_none.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_projection_mode(PhysxProjectionMode mode) {

  _desc.projectionMode = (NxJointProjectionMode)mode;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_motor
//       Access: Published
//  Description: Sets an optional joint motor.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_motor(const PhysxMotorDesc &motor) {

  _desc.motor = motor.desc();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxRevoluteJointDesc::set_limit
//       Access: Published
//  Description: Sets optional limits for the angular motion of the
//               joint.
////////////////////////////////////////////////////////////////////
void PhysxRevoluteJointDesc::
set_limit(const PhysxJointLimitDesc &low, const PhysxJointLimitDesc &high) {

  _desc.limit.low = low.desc();
  _desc.limit.high = high.desc();
}
*/
