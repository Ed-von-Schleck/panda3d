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

#include "colladaElement.h"
#include "colladaPrimitive.h"
#include "colladaSource.h"
#include "colladaVertices.h"
#include "geom.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaMesh
// Description : Object that represents the <mesh> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaMesh : public ColladaElement {
PUBLISHED:
  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;
  PT(Geom) make_geom() const;

  virtual PT(ColladaElement) get_element_by_id(const string &id) const;

  INLINE int get_num_sources() const;
  INLINE PT(ColladaSource) get_source(int i) const;
  MAKE_SEQ(get_source, get_num_sources, get_source);

  INLINE PT(ColladaVertices) get_vertices() const;
  INLINE void set_vertices(ColladaVertices *vertices);

  INLINE int get_num_primitives() const;
  INLINE PT(ColladaPrimitive) get_primitive(int i) const;
  MAKE_SEQ(get_primitives, get_num_primitives, get_primitive);

private:
  pvector<PT(ColladaSource)> _sources;
  PT(ColladaVertices) _vertices;
  pvector<PT(ColladaPrimitive)> _primitives;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaMesh",
                  ColladaElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaMesh.I"

#endif

