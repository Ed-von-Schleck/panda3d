// Filename: physxForceField.cxx
// Created by:  enn0x (06Nov09)
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

#include "physxForceField.h"
#include "physxForceFieldDesc.h"
#include "physxScene.h"

TypeHandle PhysxForceField::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxForceField::
link(NxForceField *materialPtr) {

  // Link self
  ref();
  _ptr = materialPtr;
  _ptr->userData = this;
  _error_type = ET_ok;

  // Link shapes
  // --TODO--
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxForceField::
unlink() {

  // Unlink shapes
  // --TODO--

  // Unlink self
  _ptr->userData = NULL;
  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxForceField::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  _ptr->getScene().releaseForceField(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::get_scene
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxScene) PhysxForceField::
get_scene() const {

  nassertr(_error_type == ET_ok, NULL);
  return (PhysxScene *)(_ptr->getScene().userData);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::set_name
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxForceField::
set_name(const char *name) {

  nassertv(_error_type == ET_ok);
  _ptr->setName(name);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxForceField::get_name
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
const char *PhysxForceField::
get_name() const {

  nassertr(_error_type == ET_ok, "");
  return _ptr->getName();
}

