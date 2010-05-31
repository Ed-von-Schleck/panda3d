// Filename: colladaAccessor.h
// Created by: rdb (31May10)
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

#ifndef COLLADAACCESSOR_H
#define COLLADAACCESSOR_H

#include "pointerTo.h"
#include "colladaElement.h"
#include "geomPrimitive.h"
#include "pta_int.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaAccessor
// Description : Object that represents the COLLADA <accessor> tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaAccessor : public ColladaElement {
PUBLISHED:
  ColladaAccessor();
  virtual ~ColladaAccessor() {};

  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  struct Param {
    string _name;
    string _type;
    string _semantic;
  };

private:
  int _count;
  int _offset;
  string _source;
  int _stride;
  pvector<Param> _params;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaAccessor",
                  ColladaElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

