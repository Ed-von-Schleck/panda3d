// Filename: physxSceneDesc.cxx
// Created by:  enn0x (05Sep09)
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

#include "physxSceneDesc.h"
#include "physxManager.h"

TypeHandle PhysxSceneDesc::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::set_gravity
//       Access: Published
//  Description: Sets the gravity vector.
////////////////////////////////////////////////////////////////////
void PhysxSceneDesc::
set_gravity(const LVector3f &gravity) {

  nassertv_always(!gravity.is_nan());
  _desc.gravity = PhysxManager::vec3_to_nxVec3(gravity);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::get_gravity
//       Access: Published
//  Description: Get the gravity vector.
////////////////////////////////////////////////////////////////////
LVector3f PhysxSceneDesc::
get_gravity() const {

  return PhysxManager::nxVec3_to_vec3(_desc.gravity);
}

