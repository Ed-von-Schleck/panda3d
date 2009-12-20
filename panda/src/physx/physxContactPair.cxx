// Filename: physxContactPair.cxx
// Created by:  enn0x (19Dec09)
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

#include "physxContactPair.h"
#include "physxManager.h"
#include "physxActor.h"
#include "physxContactPoint.h"

TypeHandle PhysxContactPair::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_actor_a
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxContactPair::
get_actor_a() const {

  NxActor *actorPtr = _pair.actors[0];
  return (actorPtr == NULL) ? NULL : (PhysxActor *)actorPtr->userData;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_actor_b
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxContactPair::
get_actor_b() const {

  NxActor *actorPtr = _pair.actors[1];
  return (actorPtr == NULL) ? NULL : (PhysxActor *)actorPtr->userData;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_sum_normal_force
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
LVector3f PhysxContactPair::
get_sum_normal_force() const {

  return PhysxManager::nxVec3_to_vec3(_pair.sumNormalForce);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_sum_friction_force
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
LVector3f PhysxContactPair::
get_sum_friction_force() const {

  return PhysxManager::nxVec3_to_vec3(_pair.sumFrictionForce);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_num_contact_points
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
unsigned int PhysxContactPair::
get_num_contact_points() {

  if (_contacts.size() == 0) {
    NxContactStreamIterator it(_pair.stream);
    while(it.goNextPair()) {
      while(it.goNextPatch()) {
        while(it.goNextPoint()) {
          PhysxContactPoint cp;
          cp.set(it);
          _contacts.push_back(cp);
        }
      }
    }
  }

  return _contacts.size();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::get_contact_point
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PhysxContactPoint PhysxContactPair::
get_contact_point(unsigned int idx) const {

  nassertr(idx < _contacts.size(), PhysxContactPoint::empty());
  return _contacts[idx];
}

