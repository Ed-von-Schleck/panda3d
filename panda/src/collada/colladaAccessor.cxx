// Filename: colladaAccessor.cxx
// Created by: rdb (31May10)
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

#include "colladaAccessor.h"

TypeHandle ColladaAccessor::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaAccessor::
ColladaAccessor() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::clear
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaAccessor::
clear() {
  ColladaElement::clear();
  _count = 0;
  _offset = 0;
  _source = "";
  _stride = 1;
  _params.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::load_xml
//       Access: Public
//  Description: Loads <accessor> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaAccessor::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  bool okflag = true;

  nassertr(xelement->QueryIntAttribute("count", &_count) == TIXML_SUCCESS, false);
  if (xelement->QueryIntAttribute("offset", &_offset) != TIXML_SUCCESS) {
    _offset = 0;
  }
  if (xelement->Attribute("source") != NULL) {
    _source = xelement->Attribute("source");
  }
  if (xelement->QueryIntAttribute("stride", &_stride) != TIXML_SUCCESS) {
    _stride = 1;
  }

  const TiXmlElement* xchild;

  // Read out the param specifiers
  xchild = xelement->FirstChildElement("param");
  while (xchild != NULL) {
    Param param;
    if (xchild->Attribute("name") != NULL) {
      param._name = xchild->Attribute("name");
    }
    if (xchild->Attribute("type") != NULL) {
      param._type = xchild->Attribute("type");
    }
    if (xchild->Attribute("semantic") != NULL) {
      param._semantic = xchild->Attribute("semantic");
    }
    _params.push_back(param);
    xchild = xchild->NextSiblingElement("param");
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaAccessor::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("accessor");

  xelement->SetAttribute("count", _count);
  xelement->SetAttribute("offset", _offset);
  xelement->SetAttribute("source", _source);
  xelement->SetAttribute("stride", _stride);

  for (int i = 0; i < _params.size(); ++i) {
    const Param &param = _params[i];
    TiXmlElement *xinput = new TiXmlElement("param");
    if (!param._name.empty()) {
      xinput->SetAttribute("name", param._name);
    }
    xinput->SetAttribute("type", param._type);
    if (!param._semantic.empty()) {
      xinput->SetAttribute("semantic", param._semantic);
    }
    xelement->LinkEndChild(xinput);
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::get_source
//       Access: Public
//  Description: Returns the ColladaArray that this accessor
//               operates on, or NULL if it couldn't be located.
////////////////////////////////////////////////////////////////////
PT(ColladaArrayBase) ColladaAccessor::
get_source() const {
  PT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, NULL);
  PT(ColladaArrayBase) source = DCAST(ColladaArrayBase, doc->resolve_url(_source));
  return source;
}

