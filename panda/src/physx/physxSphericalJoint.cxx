// Filename: physxSphericalJoint.cxx
// Created by:  enn0x (02Oct09)
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

#include "physxSphericalJoint.h"

TypeHandle PhysxSphericalJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJoint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxSphericalJoint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isSphericalJoint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphericalJoint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxSphericalJoint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

