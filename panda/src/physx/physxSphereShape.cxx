// Filename: physxSphereShape.cxx
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

#include "physxSphereShape.h"

TypeHandle PhysxSphereShape::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphereShape::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxSphereShape::
link(NxShape *shapePtr) {

  ref();
  _ptr = shapePtr->isSphere();
  _ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphereShape::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxSphereShape::
unlink() {

  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphereShape::set_radius
//       Access: Published
//  Description: Sets the sphere radius. 
////////////////////////////////////////////////////////////////////
void PhysxSphereShape::
set_radius(float radius) {

  nassertv(_error_type == ET_ok);
  _ptr->setRadius(radius);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxSphereShape::get_radius
//       Access: Published
//  Description: Returns the radius of the sphere.
////////////////////////////////////////////////////////////////////
float PhysxSphereShape::
get_radius() const {

  nassertr(_error_type == ET_ok, 0.0f);
  return _ptr->getRadius();
}

