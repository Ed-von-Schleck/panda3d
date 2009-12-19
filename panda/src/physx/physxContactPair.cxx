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
#include "physxActor.h"

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

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxContactPair::foo
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxContactPair::
foo() {

  cout << ">>>>>>>>>>>>>>>>>\n";
  NxContactStreamIterator i(_pair.stream);
  while(i.goNextPair()) {
    cout << "..pair\n";
    while(i.goNextPatch()) {
      const NxVec3 &contactNormal = i.getPatchNormal();
      cout << "....patch\n";
      while(i.goNextPoint()) {
        const NxVec3 &contactPoint = i.getPoint();
        cout << "......point" << contactPoint.x << " " << contactPoint.y << " " << contactPoint.z << "\n";
      }
    }
  }
  cout << "<<<<<<<<<<<<<<<<\n";
}
*/

