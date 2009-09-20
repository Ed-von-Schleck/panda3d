// Filename: physxShape.cxx
// Created by:  enn0x (16Sep09)
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

#include "physxShape.h"
#include "physxActor.h"
#include "physxBoxShape.h"
#include "physxCapsuleShape.h"
#include "physxPlaneShape.h"
#include "physxSphereShape.h"
//#include "physxConvexShape.h"
//#include "physxHeightFieldShape.h"
//#include "physxTriangleMeshShape.h"
//#include "physxWheelShape.h"

TypeHandle PhysxShape::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxShape::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxShape::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  ptr()->getActor().releaseShape(*ptr());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxShape::factory
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxShape) PhysxShape::
factory(NxShapeType shapeType) {

  switch (shapeType) {

  case NX_SHAPE_PLANE:
    return new PhysxPlaneShape();

  case NX_SHAPE_SPHERE:
    return new PhysxSphereShape();

  case NX_SHAPE_BOX:
    return new PhysxBoxShape();

  case NX_SHAPE_CAPSULE:
    return new PhysxCapsuleShape();

  //case NX_SHAPE_CONVEX:
  //  return new PhysxConvexShape();

  //case NX_SHAPE_MESH:
  //  return new PhysxTriangleMeshShape();

  //case NX_SHAPE_HEIGHTFIELD:
  //  return new PhysxHeightFieldShape();

  //case NX_SHAPE_WHEEL:
  //  return new PhysxWheelShape();
  }

  physx_cat.error() << "Unknown shape type.\n";
  return NULL;
}


////////////////////////////////////////////////////////////////////
//     Function: PhysxShape::get_actor
//       Access: Published
//  Description: Retrieves the actor which this shape is associated
//               with.
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxShape::
get_actor() const {

  nassertr(_error_type == ET_ok, NULL);
  return (PhysxActor *)(ptr()->getActor().userData);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxShape::set_name
//       Access: Published
//  Description: Sets a name string for this object. The name can
//               be retrieved again with get_name().
//               This is for debugging and is not used by the
//               physics engine.
////////////////////////////////////////////////////////////////////
void PhysxShape::
set_name(const char *name) {

  nassertv(_error_type == ET_ok);
  ptr()->setName(name);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxShape::get_name
//       Access: Published
//  Description: Returns the name string. 
////////////////////////////////////////////////////////////////////
const char *PhysxShape::
get_name() const {

  nassertr(_error_type == ET_ok, "");
  return ptr()->getName();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxShape::set_flag
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
void PhysxShape::
set_flag(PhysxShapeFlag flag, bool value) {

  nassertv(_error_type == ET_ok);

  ptr()->setFlag((NxShapeFlag)flag, value);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxShape::get_flag
//       Access : Published
//  Description : Returns the specified shape flag.
////////////////////////////////////////////////////////////////////
bool PhysxShape::
get_flag(PhysxShapeFlag flag) const {

  nassertr(_error_type == ET_ok, false);

  return (ptr()->getFlag((NxShapeFlag)flag)) ? true : false;
}

