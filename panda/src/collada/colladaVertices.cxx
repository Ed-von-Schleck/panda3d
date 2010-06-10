// Filename: colladaVertices.cxx
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

#include "colladaVertices.h"

TypeHandle ColladaVertices::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaVertices::
ColladaVertices() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::clear
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaVertices::
clear() {
  ColladaElement::clear();
  _inputs.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::load_xml
//       Access: Public
//  Description: Loads <vertices> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaVertices::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  bool okflag = true;

  const TiXmlElement* xchild;

  // Read out the input specifiers
  xchild = xelement->FirstChildElement("input");
  while (xchild != NULL) {
    PT(ColladaInput) input = new ColladaInput;
    input->_parent = this;
    input->load_xml(xchild);
    _inputs.push_back(input);
    xchild = xchild->NextSiblingElement("input");
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaVertices::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("vertices");

  for (int i = 0; i < _inputs.size(); ++i) {
    xelement->LinkEndChild(_inputs[i]->make_xml());
  }

  return xelement;
}

