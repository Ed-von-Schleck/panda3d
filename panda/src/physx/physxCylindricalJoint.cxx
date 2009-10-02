// Filename: physxCylindricalJoint.cxx
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

#include "physxCylindricalJoint.h"

TypeHandle PhysxCylindricalJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxCylindricalJoint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCylindricalJoint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isCylindricalJoint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxCylindricalJoint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxCylindricalJoint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

