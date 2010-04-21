// Filename: colladaAssetElement.cxx
// Created by:  rdb (21Apr10)
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

#include "colladaAssetElement.h"

TypeHandle ColladaAssetElement::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::load_xml
//       Access: Public
//  Description: Loads <asset> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaAssetElement::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("asset");
  if (xchild != NULL) {
    _asset = new ColladaAsset;
    _asset->_parent = this;
    return _asset->load_xml(xchild);
  } else {
    _asset = NULL;
    return true;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaAssetElement::
make_xml() const {
  TiXmlElement * xelement = ColladaElement::make_xml();

  if (_asset) {
    xelement->LinkEndChild(_asset->make_xml());
  }

  return xelement;
}
