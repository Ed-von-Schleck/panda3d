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
#include "physxBounds3.h"

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

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::set_flag
//       Access: Published
//  Description: Raise or lower individual scene flags.
////////////////////////////////////////////////////////////////////
void PhysxSceneDesc::
set_flag(const PhysxSceneFlag flag, bool value) {

  if (value == true) {
    _desc.flags |= flag;
  }
  else {
    _desc.flags &= ~(flag);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::get_flag
//       Access: Published
//  Description: Returns the specified scene flag.
////////////////////////////////////////////////////////////////////
bool PhysxSceneDesc::
get_flag(const PhysxSceneFlag flag) const {

  return (_desc.flags & flag) ? true : false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::set_max_bounds
//       Access: Published
//  Description: Set the max scene bounds. 
//
//               If scene bounds are provided (maxBounds in the
//               descriptor), the SDK takes advantage of this
//               information to accelerate scene-level collision
//               queries (e.g. raycasting). When using maxBounds,
//               you have to make sure created objects stay within
//               the scene bounds. In particular, the position of
//               dynamic shapes should stay within the provided
//               bounds. Otherwise the shapes outside the bounds
//               will not be taken into account by all scene queries
//               (raycasting, sweep tests, overlap tests, etc). They
//               will nonetheless still work correctly for the main
//               physics simulation.
////////////////////////////////////////////////////////////////////
void PhysxSceneDesc::
set_max_bounds(PhysxBounds3 &bounds) {

  _desc.maxBounds = &(bounds._bounds);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSceneDesc::get_max_bounds
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PhysxBounds3 PhysxSceneDesc::
get_max_bounds() const {

  throw "Not Implemented";

  //PhysxBounds3 value;
  //value._bounds = *(_desc.maxBounds);
  //return value;
}

