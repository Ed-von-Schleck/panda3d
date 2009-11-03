// Filename: physxPrismaticJoint.cxx
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

#include "physxPrismaticJoint.h"
#include "physxPrismaticJointDesc.h"

TypeHandle PhysxPrismaticJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxPrismaticJoint::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPrismaticJoint::
link(NxJoint *jointPtr) {

  ref();
  _ptr = jointPtr->isPrismaticJoint();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxPrismaticJoint::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxPrismaticJoint::
unlink() {

  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxPrismaticJoint::save_to_desc
//       Access : Published
//  Description : Saves the state of the joint object to a 
//                descriptor.
////////////////////////////////////////////////////////////////////
void PhysxPrismaticJoint::
save_to_desc(PhysxPrismaticJointDesc &jointDesc) const {

  nassertv(_error_type == ET_ok);
  _ptr->saveToDesc(jointDesc._desc);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysPrismaticJoint::load_from_desc
//       Access : Published
//  Description : Loads the entire state of the joint from a 
//                descriptor with a single call.
////////////////////////////////////////////////////////////////////
void PhysxPrismaticJoint::
load_from_desc(const PhysxPrismaticJointDesc &jointDesc) {

  nassertv(_error_type == ET_ok);
  _ptr->loadFromDesc(jointDesc._desc);
}

