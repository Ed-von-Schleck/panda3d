// Filename: colladaEffect.cxx
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

#include "colladaEffect.h"

TypeHandle ColladaEffect::_type_handle;
const string ColladaEffect::_element_name ("effect");
const string ColladaEffect::_library_name ("library_effects");

////////////////////////////////////////////////////////////////////
//     Function: ColladaEffect::clear
//       Access: Public
//  Description: Resets the ColladaEffect to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaEffect::
clear () {
  ColladaAssetElement::clear();
  _profiles.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaEffect::load_xml
//       Access: Public
//  Description: Loads <effect> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaEffect::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  nassertr(xelement->ValueStr() == "effect", false);

  const TiXmlElement* xchild = xelement->FirstChildElement();
  while (xchild != NULL) {
    PT(ColladaProfile) item = new ColladaProfile;
    item->_parent = this;
    item->load_xml(xchild);
    _profiles.push_back(item);
    xchild = xchild->NextSiblingElement();
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaEffect::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the asset.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaEffect::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("effect");

  for (int i = 0; i < _profiles.size(); ++i) {
    xelement->LinkEndChild(_profiles.at(i)->make_xml());
  }

  return xelement;
}

