// Filename: physxPointOnLineJoint.cxx
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

#include "physxPointOnLineJoint.h"

TypeHandle PhysxPointOnLineJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxPointOnLineJoint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPointOnLineJoint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isPointOnLineJoint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPointOnLineJoint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPointOnLineJoint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

