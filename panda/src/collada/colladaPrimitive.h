// Filename: colladaPrimitive.h
// Created by: rdb (22Apr10)
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

#ifndef COLLADAPRIMITIVE_H
#define COLLADAPRIMITIVE_H

#include "pointerTo.h"
#include "colladaElement.h"
#include "geomPrimitive.h"
#include "pta_int.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaPrimitive
// Description : Object that represents the COLLADA primitive
//               elements, e.g. <vertices>, <lines>, etc.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaPrimitive : public ColladaElement {
PUBLISHED:
  enum PrimitiveType {
    PT_lines,
    PT_linestrips,
    PT_polygons,
    PT_polylist,
    PT_triangles,
    PT_trifans,
    PT_tristrips
  };

  ColladaPrimitive(PrimitiveType primitive_type);
  virtual ~ColladaPrimitive() {};

  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;
  virtual PT(GeomPrimitive) make_primitive() const;

  struct Input {
    string _semantic;
    string _source;
    int _offset;
    int _set;
  };

private:
  PrimitiveType _primitive_type;
  pvector<Input> _inputs;
  PTA_int _p;
  int _count;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaPrimitive",
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

