// Filename: physxConvexMeshDesc.cxx
// Created by:  enn0x (11Oct09)
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

#include "physxConvexMeshDesc.h"
#include "physxManager.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMeshDesc::set_num_vertices
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxConvexMeshDesc::
set_num_vertices(unsigned int numVertices) {

  if (_desc.points) {
    delete [] _vertices;
  }

  _vertices = new NxVec3[numVertices];

  _desc.numVertices = numVertices;
  _desc.points = _vertices;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMeshDesc::set_vertex
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxConvexMeshDesc::
set_vertex(unsigned int idx, const LPoint3f &vert) {

  nassertv(_desc.numVertices > idx);
  _vertices[idx] = PhysxManager::point3_to_nxVec3(vert);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxConvexMeshDesc::get_desc
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
const NxConvexMeshDesc &PhysxConvexMeshDesc::
get_desc() const {

  return _desc;
}

