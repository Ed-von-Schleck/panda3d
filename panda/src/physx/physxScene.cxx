// Filename: physxScene.cxx
// Created by:  enn0x (14Sep09)
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

#include "physxScene.h"
#include "physxManager.h"

TypeHandle PhysxScene::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
link(NxScene *scenePtr) {

  // TODO: link actors etc.

  ref();
  _ptr = scenePtr;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
unlink() {

  // TODO: unlink actors etc.

  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  NxPhysicsSDK *sdk = NxGetPhysicsSDK(); // TODO: make manager a singleton ???
  // TODO: assert sdk != NULL
  sdk->releaseScene(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::set_gravity
//       Access: Published
//  Description: Sets a constant gravity for the entire scene.
////////////////////////////////////////////////////////////////////
void PhysxScene::
set_gravity(const LVector3f &gravity) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!gravity.is_nan());

  _ptr->setGravity(PhysxManager::vec3_to_nxVec3(gravity));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_gravity
//       Access: Published
//  Description: Retrieves the current gravity setting.
////////////////////////////////////////////////////////////////////
LVector3f PhysxScene::
get_gravity() const {

  nassertr(_error_type == ET_ok, LVector3f::zero());

  NxVec3 gravity;
  _ptr->getGravity(gravity);
  return PhysxManager::nxVec3_to_vec3(gravity);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::do_physics
//       Access: Published
//  Description: Advances the simulation by an elapsedTime time. 
//
//               If elapsedTime is large, it is internally
//               subdivided according to parameters provided with
//               the setFixedTimesteps() method.
//
//               The elapsed time has to be in the range (0, inf).
////////////////////////////////////////////////////////////////////
void PhysxScene::
do_physics(float dt) {

  nassertv(_error_type == ET_ok);

  // TODO
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_num_actors
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
unsigned int PhysxScene::
get_num_actors() const {

  nassertr(_error_type == ET_ok,-1);

  return _ptr->getNbActors();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::create_actor
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxScene::
create_actor(PhysxActorDesc &desc) {

  nassertr(_error_type == ET_ok, NULL);
  nassertr(desc.is_valid(),NULL);

  PT(PhysxActor) actor = new PhysxActor();

  desc.ptr()->userData = actor;
  NxActor *ptr = _ptr->createActor(*desc.ptr());

  actor->link(ptr);

  return actor;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_actor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxScene::
get_actor(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);
  nassertr_always(idx < _ptr->getNbActors(), NULL);

  NxActor *actorPtr = _ptr->getActors()[idx];
  PhysxActor *actor = (PhysxActor *)(actorPtr->userData);

  return actor;
}

