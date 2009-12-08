// Filename: physxTriangleMeshDesc.cxx
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

#include "physxTriangleMeshDesc.h"
#include "physxManager.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxTriangleMeshDesc::set_num_vertices
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxTriangleMeshDesc::
set_num_vertices(unsigned int numVertices) {

  if (_desc.points) {
    delete [] _vertices;
  }

  _vertices = new NxVec3[numVertices];

  _desc.numVertices = numVertices;
  _desc.points = _vertices;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxTriangleMeshDesc::set_vertex
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxTriangleMeshDesc::
set_vertex(unsigned int idx, const LPoint3f &vert) {

  nassertv(_desc.numVertices > idx);
  _vertices[idx] = PhysxManager::point3_to_nxVec3(vert);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxTriangleMeshDesc::set_num_triangles
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxTriangleMeshDesc::
set_num_triangles(unsigned int numTriangles) {

  if (_desc.triangles) {
    delete [] _triangles;
  }

  _triangles = new NxU32[3 * numTriangles];

  _desc.numTriangles = numTriangles;
  _desc.triangles = _triangles;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxTriangleMeshDesc::set_triangles
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxTriangleMeshDesc::
set_triangle(unsigned int idx, unsigned int i1, unsigned int i2, unsigned int i3) {

  nassertv(_desc.numTriangles > idx);

  idx = 3 * idx;
  _triangles[idx] = i1;
  _triangles[idx + 1] = i2;
  _triangles[idx + 2] = i3;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxTriangleMeshDesc::get_desc
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
const NxTriangleMeshDesc &PhysxTriangleMeshDesc::
get_desc() const {

  return _desc;
}

