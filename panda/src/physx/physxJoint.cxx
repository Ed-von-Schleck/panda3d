// Filename: physxJoint.cxx
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

#include "physxJoint.h"
#include "physxManager.h"
#include "physxActor.h"
#include "physxScene.h"
#include "physxCylindricalJoint.h"
#include "physxDistanceJoint.h"
#include "physxFixedJoint.h"
#include "physxPointInPlaneJoint.h"
#include "physxPointOnLineJoint.h"
#include "physxPrismaticJoint.h"
#include "physxPulleyJoint.h"
#include "physxRevoluteJoint.h"
#include "physxSphericalJoint.h"

TypeHandle PhysxJoint::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxJoint::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  ptr()->getScene().releaseJoint(*ptr());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::factory
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxJoint) PhysxJoint::
factory(NxJointType shapeType) {

  switch (shapeType) {

  case NX_JOINT_PRISMATIC:
    return new PhysxPrismaticJoint();

  case NX_JOINT_REVOLUTE:
    return new PhysxRevoluteJoint();

  case NX_JOINT_CYLINDRICAL:
    return new PhysxCylindricalJoint();

  case NX_JOINT_SPHERICAL:
    return new PhysxSphericalJoint();

  case NX_JOINT_POINT_ON_LINE:
    return new PhysxPointOnLineJoint();

  case NX_JOINT_POINT_IN_PLANE:
    return new PhysxPointInPlaneJoint();

  case NX_JOINT_DISTANCE:
    return new PhysxDistanceJoint();

  case NX_JOINT_PULLEY:
    return new PhysxPulleyJoint();

  case NX_JOINT_FIXED:
    return new PhysxFixedJoint();

  case NX_JOINT_D6:
    return new PhysxFixedJoint();
  }

  physx_cat.error() << "Unknown joint type.\n";
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::set_name
//       Access: Published
//  Description: Sets a name string for this object. The name can
//               be retrieved again with get_name().
//               This is for debugging and is not used by the
//               physics engine.
////////////////////////////////////////////////////////////////////
void PhysxJoint::
set_name(const char *name) {

  nassertv(_error_type == ET_ok);
  ptr()->setName(name);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::get_name
//       Access: Published
//  Description: Returns the name string. 
////////////////////////////////////////////////////////////////////
const char *PhysxJoint::
get_name() const {

  nassertr(_error_type == ET_ok, "");
  return ptr()->getName();
}

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::get_actor
//       Access: Published
//  Description: Retrieves the actor which this shape is associated
//               with.
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxJoint::
get_actor() const {

  nassertr(_error_type == ET_ok, NULL);
  return (PhysxActor *)(ptr()->getActor().userData);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxJoint::set_flag
//       Access : Published
//  Description : Sets the specified shape flag.
//
//                The shape may be turned into a trigger by setting
//                one or more of the TriggerFlags to true. A trigger
//                shape will not collide with other shapes. Instead,
//                if a shape enters the trigger's volume, a trigger
//                event will be sent. Trigger events can be listened
//                to by DirectObjects.
//                The following trigger events can be sent:
//                - physx-trigger-enter
//                - physx-trigger-stay
//                - physx-trigger-leave
////////////////////////////////////////////////////////////////////
void PhysxJoint::
set_flag(PhysxJointFlag flag, bool value) {

  nassertv(_error_type == ET_ok);

  ptr()->setFlag((NxJointFlag)flag, value);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxJoint::get_flag
//       Access : Published
//  Description : Returns the specified shape flag.
////////////////////////////////////////////////////////////////////
bool PhysxJoint::
get_flag(PhysxJointFlag flag) const {

  nassertr(_error_type == ET_ok, false);

  return (ptr()->getFlag((NxJointFlag)flag)) ? true : false;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJoint::set_skin_width
//       Access: Published
//  Description: Sets the skin width. 
//               The skin width must be non-negative.
////////////////////////////////////////////////////////////////////
void PhysxJoint::
set_skin_width(float skinWidth) {

  nassertv(_error_type == ET_ok);
  nassertv(skinWidth >= 0.0f);

  ptr()->setSkinWidth(skinWidth);
}
*/

