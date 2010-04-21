// Filename: colladaSource.h
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

#ifndef COLLADASOURCE_H
#define COLLADASOURCE_H

#include "colladaAssetElement.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaSource
// Description : Object that represents the <source> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaSource : public ColladaAssetElement {
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaSource::init_type();
    register_type(_type_handle, "ColladaSource",
                  ColladaSource::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

