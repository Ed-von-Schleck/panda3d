// Filename: colladaMesh.h
// Created by: Xidram (20Apr10)
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

#ifndef COLLADAMESH_H
#define COLLADAMESH_H

#include "colladaGeometry.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaMesh
// Description : Object that represents the <mesh> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaMesh : public ColladaGeometry {
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaGeometry::init_type();
    register_type(_type_handle, "ColladaMesh",
                  ColladaGeometry::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

