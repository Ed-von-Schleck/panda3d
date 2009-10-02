// Filename: physxD6Joint.cxx
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

#include "physxD6Joint.h"

TypeHandle PhysxD6Joint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxD6Joint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxD6Joint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isD6Joint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxD6Joint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxD6Joint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

