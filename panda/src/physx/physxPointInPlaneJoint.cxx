// Filename: physxPointInPlaneJoint.cxx
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

#include "physxPointInPlaneJoint.h"

TypeHandle PhysxPointInPlaneJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxPointInPlaneJoint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPointInPlaneJoint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isPointInPlaneJoint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPointInPlaneJoint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPointInPlaneJoint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

