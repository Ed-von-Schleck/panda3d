// Filename: physxConvexMesh.cxx
// Created by:  enn0x (13Oct09)
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

#include "physxConvexMesh.h"

TypeHandle PhysxConvexMesh::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMesh::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxConvexMesh::
link(NxConvexMesh *meshPtr) {

  // Link self
  //ref();
  _ptr = meshPtr;
  //_ptr->userData = this;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMesh::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxConvexMesh::
unlink() {

  // Unlink self
  //_ptr->userData = NULL;
  _error_type = ET_released;
  //unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMesh::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxConvexMesh::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  //_ptr->getScene().releaseConvexMesh(*_ptr);
  //_ptr = NULL;

  NxGetPhysicsSDK()->releaseConvexMesh(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMesh::get_reference_count
//       Access: Published
//  Description: Returns the reference count for shared meshes.
////////////////////////////////////////////////////////////////////
unsigned int PhysxConvexMesh::
get_reference_count() const {

  nassertr(_error_type == ET_ok, 0);

  return _ptr->getReferenceCount();
}

