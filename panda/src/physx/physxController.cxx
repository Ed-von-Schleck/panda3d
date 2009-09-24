// Filename: physxController.cxx
// Created by:  enn0x (24Sep09)
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

#include "event.h"
#include "eventQueue.h"
#include "eventParameter.h"

#include "physxController.h"
#include "physxManager.h"
#include "physxActor.h"
#include "physxBoxController.h"
#include "physxCapsuleController.h"

TypeHandle PhysxController::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxController::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  get_actor()->get_scene()->cm()->releaseController(*ptr());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::factory
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxController) PhysxController::
factory(NxControllerType controllerType) {

  switch (controllerType) {

  case NX_CONTROLLER_BOX:
    return new PhysxBoxController();

  case NX_CONTROLLER_CAPSULE:
    return new PhysxCapsuleController();

  }

  physx_cat.error() << "Unknown controller type.\n";
  return NULL;
}


////////////////////////////////////////////////////////////////////
//     Function: PhysxController::get_actor
//       Access: Published
//  Description: Retrieves the actor which this controller is
//               associated with.
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxController::
get_actor() const {

  nassertr(_error_type == ET_ok, NULL);
  return (PhysxActor *)(ptr()->getActor()->userData);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::set_pos
//       Access: Published
//  Description: Sets the position of the controller is global
//               space. This can be used for initial placement or
//               for teleporting the character.
////////////////////////////////////////////////////////////////////
void PhysxController::
set_pos(const LPoint3f &pos) {

  nassertv(_error_type == ET_ok);
  ptr()->setPosition(PhysxManager::point3_to_nxExtVec3(pos));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::get_pos
//       Access: Published
//  Description: Retruns the position of the controller is global
//               space.
////////////////////////////////////////////////////////////////////
LPoint3f PhysxController::
get_pos() const {

  nassertr(_error_type == ET_ok, LPoint3f::zero());
  return PhysxManager::nxExtVec3_to_point3(ptr()->getPosition());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::set_sharpness
//       Access: Published
//  Description: Sharpness is used to smooth motion with a feedback
//               filter, having a value between 0 (so smooth it
//               doesn't move) and 1 (no smoothing = unfiltered
//               motion). Sharpness can ease the motion curve when
//               the auto-step feature is used with boxes.
//               Default value is 1.0.
////////////////////////////////////////////////////////////////////
void PhysxController::
set_sharpness(float sharpness) {

  nassertv(_error_type == ET_ok);
  nassertv(sharpness > 0.0f);
  nassertv(sharpness <= 1.0f);

  _sharpness = sharpness;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::get_sharpness
//       Access: Published
//  Description: Returns the sharpness used to ease the motion curve
//               when the auto-step feature is used.
//               Default value is 1.0.
////////////////////////////////////////////////////////////////////
float PhysxController::
get_sharpness() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _sharpness;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxController::set_collision
//       Access: Published
//  Description: Enable/Disable collisions for this controller and
//               actor.
////////////////////////////////////////////////////////////////////
void PhysxController::
set_collision(bool enable) {

  nassertv(_error_type == ET_ok);
  ptr()->setCollision(enable);
}

