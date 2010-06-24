// Filename: colladaVertices.h
// Created by: rdb (29May10)
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

#ifndef COLLADAVERTICES_H
#define COLLADAVERTICES_H

#include "pointerTo.h"
#include "colladaElement.h"
#include "colladaInput.h"

#include "geomVertexArrayFormat.h"
#include "geomVertexArrayData.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaVertices
// Description : Object that represents the COLLADA <vertices> tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaVertices : public ColladaElement {
PUBLISHED:
  ColladaVertices();
  virtual ~ColladaVertices() {};

  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  PT(GeomVertexArrayFormat) make_array_format() const;
  bool write_data(GeomVertexData *data) const;

private:
  pvector<PT(ColladaInput)> _inputs;

  friend class ColladaInput;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaVertices",
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

