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
//       Access: Published, Virtual
//  Description: Loads <material> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaMaterial::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("instance_effect");
  if (xchild != NULL) {
    _instance_effect = new ColladaInstanceEffect;
    _instance_effect->_parent = this;
    _instance_effect->load_xml(xchild);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaMaterial::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("material");

  if (_instance_effect != NULL) {
    xelement->LinkEndChild(_instance_effect->make_xml());
  }

  return xelement;
}

