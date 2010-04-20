// Filename: colladaEffect.h
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

#ifndef COLLADAEFFECT_H
#define COLLADAEFFECT_H

#include "colladaAssetElement.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaEffect
// Description : Object that represents the <effect> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaEffect : public ColladaAssetElement, public TypedReferenceCount {
public:
  // Needed by ColladaLibrary to validate the element names.
  static const string _element_name;
  static const string _library_name;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaEffect",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

