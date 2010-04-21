// Filename: colladaGeometry.h
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

#ifndef COLLADAGEOMETRY_H
#define COLLADAGEOMETRY_H

#include "colladaAssetElement.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaGeometry
// Description : Object that represents the <geometry> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaGeometry : public ColladaAssetElement {
PUBLISHED:
  virtual void clear();

public:
  static const string _element_name;
  static const string _library_name;

  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaGeometry",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

