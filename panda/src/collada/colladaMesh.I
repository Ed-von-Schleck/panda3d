// Filename: colladaMesh.I
// Created by:  rdb (01Jun10)
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


////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_num_sources
//       Access: Published
//  Description: Returns the number of ColladaSource objects stored
//               within the mesh, each of which links to an array
//               and indicates how it is accessed.
////////////////////////////////////////////////////////////////////
INLINE int ColladaMesh::
get_num_sources() const {
  return _sources.size();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_source
//       Access: Published
//  Description: Returns a modifiable pointer to the ith source
//               object stored within the mesh, so application code
//               can directly manipulate the properties of this
//               source.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaSource) ColladaMesh::
get_source(int i) const {
  return _sources.at(i);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_vertices
//       Access: Published
//  Description: Returns the ColladaVertices object that belongs
//               to this mesh.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaVertices) ColladaMesh::
get_vertices() const {
  return _vertices;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::set_vertices
//       Access: Published
//  Description: Sets the ColladaVertices object that belongs
//               to this mesh. This should not be NULL - every
//               mesh element is supposed to have a vertices
//               element.
////////////////////////////////////////////////////////////////////
INLINE void ColladaMesh::
set_vertices(ColladaVertices *vertices) {
  detach(_vertices);
  _vertices = vertices;
  attach(_vertices);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_num_primitives
//       Access: Published
//  Description: Returns the number of ColladaPrimitive objects stored
//               within the mesh, each of which represents a number of
//               primitives of a particular type.
////////////////////////////////////////////////////////////////////
INLINE int ColladaMesh::
get_num_primitives() const {
  return _primitives.size();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_primitive
//       Access: Published
//  Description: Returns a modifiable pointer to the ith primitive
//               object stored within the mesh, so application code
//               can directly manipulate the properties of this
//               primitive.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaPrimitive) ColladaMesh::
get_primitive(int i) const {
  return _primitives.at(i);
}

