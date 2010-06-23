// Filename: colladaInput.h
// Created by:  rdb (02Jun10)
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

#ifndef COLLADAINPUT_H
#define COLLADAINPUT_H

#include "colladaElement.h"
#include "geomVertexArrayFormat.h"

class ColladaPrimitive;

////////////////////////////////////////////////////////////////////
//       Class : ColladaInput
// Description : Object that represents the <input> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaInput : public ColladaElement {
PUBLISHED:
  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  PT(InternalName) get_column_name() const;
  int make_columns(GeomVertexArrayFormat *format) const;
  bool write_data(GeomVertexData *data, const PTA_int &p, int stride) const;

  INLINE int get_offset() const;
  INLINE const string &get_semantic() const;
  INLINE int get_set() const;

private:
  int _offset;
  string _semantic;
  string _source;
  int _set;

  friend class ColladaPrimitive;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaInput",
                  ColladaElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaInput.I"

#endif

