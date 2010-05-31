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
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  PT(ColladaElement) get_geometric_element() const;

  enum GeometryType {
    GT_none = 0,
    GT_convex_mesh,
    GT_mesh,
    GT_spline,
    GT_brep
  };

  virtual PT(ColladaElement) get_element_by_id(const string &id) const;

private:
  GeometryType _geometry_type;
  PT(ColladaElement) _geometric_element;

public:
  static const string _element_name;
  static const string _library_name;

public:
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

