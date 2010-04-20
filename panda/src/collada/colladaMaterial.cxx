// Filename: colladaMaterial.cxx
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

#include "colladaMaterial.h"

TypeHandle ColladaMaterial::_type_handle;
const string ColladaMaterial::_element_name ("material");
const string ColladaMaterial::_library_name ("library_materials");

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::load_xml
//       Access: Public
//  Description: Loads <material> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaMaterial::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("instance_effect");
  if (xchild != NULL) {
    _effect->load_xml(xchild);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaMaterial::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("material");
  xelement->LinkEndChild(_effect->make_xml());

  return xelement;
}

