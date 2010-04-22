// Filename: colladaGeometry.cxx
// Created by: rdb (20Apr10)
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

#include "colladaGeometry.h"
#include "colladaMesh.h"

TypeHandle ColladaGeometry::_type_handle;
const string ColladaGeometry::_element_name ("geometry");
const string ColladaGeometry::_library_name ("library_geometries");

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::clear
//       Access: Public
//  Description: Resets the ColladaGeometry to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaGeometry::
clear () {
  ColladaAssetElement::clear();
  _geometry_type = GT_none;
  _geometric_element = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::load_xml
//       Access: Public
//  Description: Loads <geometry> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaGeometry::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("mesh");
  if (xchild != NULL) {
    _geometry_type = GT_mesh;
    _geometric_element = new ColladaMesh;
  }

  if (_geometric_element == NULL) {
    collada_cat.error() <<
      "<geometry> element does not have valid geometric subelement\n";
    return false;
  } else {
    _geometric_element->_parent = this;
    return _geometric_element->load_xml(xchild);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaGeometry::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("geometry");

  TiXmlElement *xchild;


  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::get_geometric_element
//       Access: Public
//  Description: Returns the ColladaElement that describes the
//               geometry. Use get_geometry_type to find out
//               of what type it is.
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaGeometry::
get_geometric_element() const {
  return _geometric_element;
}

