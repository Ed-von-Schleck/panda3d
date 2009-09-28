// Filename: physxPulleyJointDesc.cxx
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

#include "physxPulleyJointDesc.h"
//#include "physxMotorDesc.h"
//#include "physxConverter.h"

TypeHandle PhysxPulleyJointDesc::_type_handle;

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_distance
//       Access: Published
//  Description: Sets the rest length of the rope connecting the
//               two objects. 
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_distance(float distance) {

  _desc.distance = distance;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_stiffness
//       Access: Published
//  Description: Sets how stiff the constraint is, between 0 and 1
//               (stiffest)
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_stiffness(float stiffness) {

  _desc.stiffness = stiffness;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_ratio
//       Access: Published
//  Description: Sets the transmission ratio.
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_ratio(float ratio) {

  _desc.ratio = ratio;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_flag
//       Access: Published
//  Description: Sets or clears a single PulleyJointFlag flag.
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_flag(PhysxPulleyJointFlag flag, bool value) {

  if (value == true) {
    _desc.flags |= flag;
  }
  else {
    _desc.flags &= ~(flag);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_pulley
//       Access: Published
//  Description: Sets the suspension points of two bodies in world
//               space.
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_pulley(unsigned int idx, const LPoint3f pos) {

  nassertv_always(idx < 2);
  _desc.pulley[idx] = Point3_to_NxVec3(pos);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPulleyJointDesc::set_motor
//       Access: Published
//  Description: Sets an optional joint motor.
////////////////////////////////////////////////////////////////////
void PhysxPulleyJointDesc::
set_motor(const PhysxMotorDesc &motor) {

  _desc.motor = motor.desc();
}
*/
