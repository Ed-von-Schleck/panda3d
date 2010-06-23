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

  if (_stride < _params.size()) {
    collada_cat.warning()
      << "Stride count of accessor is lower than the amount of params!\n";
    _stride = _params.size();
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
//     Function: ColladaAccessor::get_array
//       Access: Public
//  Description: Returns the ColladaArray that this accessor
//               operates on, or NULL if it couldn't be located.
////////////////////////////////////////////////////////////////////
PT(ColladaArrayBase) ColladaAccessor::
get_array() const {
  PT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, NULL);
  PT(ColladaArrayBase) source = DCAST(ColladaArrayBase, doc->resolve_url(_source));
  return source;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::get_num_bound_params
//       Access: Published
//  Description: Returns the number of params that are bound to
//               a value. This usually means the amount of columns
//               in a vector, position or color.
////////////////////////////////////////////////////////////////////
int ColladaAccessor::
get_num_bound_params() const {
  int i = 0;
  for (int p = 0; p < _params.size(); ++p) {
    if (!_params[i]._name.empty()) {
      ++i;
    }
  }
  return i;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::get_values
//       Access: Public
//  Description: Clears the given array and fills in the values
//               as specified by the accessor settings. Returns
//               false if this failed for some reason. On failure,
//               the given array remains unmodified.
//               Note that unbound parameter values are omitted.
////////////////////////////////////////////////////////////////////
bool ColladaAccessor::
get_values(PTA_LVecBase4f &into) const {
  CPT(ColladaArrayBase) array_base = get_array();
  if (array_base == NULL || array_base->get_array_type() != AT_float) {
    return false;
  }
  CPT(ColladaFloatArray) array = DCAST(ColladaFloatArray, array_base);
  into.clear();
  int pos = _offset;
  for (int a = 0; a < _count; ++a) {
    int i = 0;
    // Yes, the last component defaults to 1 to work around a
    // perspective divide that Panda3D does internally for points.
    LVecBase4f v (0, 0, 0, 1);
    for (int p = 0; p < _params.size(); ++p) {
      if (!_params[i]._name.empty()) {
        v._v.data[i++] = array->at(pos + p);
      }
    }
    into.push_back(v);
    pos += _stride;
  }
  return true;
}

