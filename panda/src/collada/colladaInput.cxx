// Filename: colladaInput.cxx
// Created by: rdb (02Jun10)
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

#include "colladaInput.h"
#include "colladaVertices.h"

TypeHandle ColladaInput::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaInput to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaInput::
clear () {
  ColladaElement::clear();
  _offset = 0;
  _semantic.clear();
  _source.clear();
  _set = -1;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::load_xml
//       Access: Public
//  Description: Loads <input> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaInput::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "input", false);

  if (xelement->QueryIntAttribute("offset", &_offset) != TIXML_SUCCESS) {
    _offset = 0;
  }
  if (xelement->Attribute("semantic") != NULL) {
    _semantic = upcase(xelement->Attribute("semantic"));
  }
  if (xelement->Attribute("source") != NULL) {
    _source = xelement->Attribute("source");
  }
  if (xelement->QueryIntAttribute("set", &_set) != TIXML_SUCCESS) {
    _set = -1;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaInput::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("input");

  if (_offset != 0) {
    xelement->SetAttribute("offset", _offset);
  }
  if (!_semantic.empty()) {
    xelement->SetAttribute("semantic", _semantic);
  }
  if (!_source.empty()) {
    xelement->SetAttribute("source", _source);
  }
  if (_set >= 0) {
    xelement->SetAttribute("set", _set);
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::make_column
//       Access: Public
//  Description: Adds GeomVertexColumns to the indicated array
//               format. This is often just one column, but this
//               can be more than one if the input has the
//               VERTEX semantic. Returns the amount of columns
//               that were added to the format.
////////////////////////////////////////////////////////////////////
int ColladaInput::
make_columns(GeomVertexArrayFormat *format) const {
  nassertr(format != NULL, 0);
  nassertr(!_semantic.empty(), 0);

  CPT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, 0);

  // If it's has the VERTEX semantic, it redirects to a <vertices> element.
  if (_semantic == "VERTEX") {
    PT(ColladaVertices) vertices = DCAST(ColladaVertices, doc->resolve_url(_source));
    nassertr(vertices != NULL, 0);
    int counter = 0;
    for (int i = 0; i < vertices->_inputs.size(); ++i) {
      counter += vertices->_inputs[i]->make_columns(format);
    }
    return counter;
  }

  PT(InternalName) cname = NULL;
  GeomEnums::Contents contents = GeomEnums::C_other;
  if (_semantic == "BINORMAL") {
    cname = InternalName::get_binormal();
    contents = GeomEnums::C_vector;
  } else if (_semantic == "COLOR") {
    cname = InternalName::get_color();
    contents = GeomEnums::C_color;
  } else if (_semantic == "NORMAL") {
    cname = InternalName::get_normal();
    contents = GeomEnums::C_vector;
  } else if (_semantic == "POSITION") {
    cname = InternalName::get_vertex();
    contents = GeomEnums::C_point;
  } else if (_semantic == "TANGENT") {
    cname = InternalName::get_tangent();
    contents = GeomEnums::C_vector;
  } else if (_semantic == "TEXCOORD") {
    cname = InternalName::get_texcoord();
    contents = GeomEnums::C_texcoord;
  } else {
    cname = InternalName::make(downcase(_semantic));
    contents = GeomEnums::C_other;
    collada_cat.warning()
      << "Unrecognized input semantic '" << _semantic << "' found\n";
  }

  PT(ColladaSource) source = DCAST(ColladaSource, doc->resolve_url(_source));
  nassertr(source != NULL, 0);
  PT(ColladaAccessor) accessor = source->get_accessor();
  nassertr(accessor != NULL, 0);
  PT(ColladaArrayBase) array = accessor->get_array();
  nassertr(array != NULL, 0);
  GeomEnums::NumericType ntype;
  if (array->get_array_type() == AT_float) {
    ntype = GeomEnums::NT_float32;
  } else if (array->get_array_type() == AT_int) {
    ntype = GeomEnums::NT_uint32;
  } else {
    collada_cat.error() << "Unusable array type\n";
    return 0;
  }
  format->add_column(cname, accessor->get_num_bound_params(), ntype, contents);

  return 1;
}

