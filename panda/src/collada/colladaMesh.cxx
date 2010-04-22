// Filename: colladaMesh.cxx
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

#include "colladaMesh.h"
#include "colladaPrimitive.h"

TypeHandle ColladaMesh::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::clear
//       Access: Public
//  Description: Resets the ColladaMesh to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaMesh::
clear () {
  ColladaElement::clear();
  _primitives.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::load_xml
//       Access: Public
//  Description: Loads <mesh> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaMesh::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "mesh", false);

  bool okflag = true;
  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement();
  while (xchild != NULL) {
    const string &value = xchild->ValueStr();
    PT(ColladaPrimitive) prim = NULL;
    if (value == "lines") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_lines);
    } else if (value == "linestrips") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_linestrips);
    } else if (value == "polygons") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_polygons);
    } else if (value == "polylist") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_polylist);
    } else if (value == "triangles") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_triangles);
    } else if (value == "trifans") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_trifans);
    } else if (value == "tristrips") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_tristrips);
    }
    if (prim != NULL) {
      prim->_parent = this;
      prim->load_xml(xchild);
      _primitives.push_back(prim);
    }
    xchild = xchild->NextSiblingElement();
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaMesh::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("mesh");

  for (int i = 0; i < _primitives.size(); ++i) {
    xelement->LinkEndChild(_primitives.at(i)->make_xml());
  }

  return xelement;
}

