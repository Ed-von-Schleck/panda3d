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
#include "geomVertexWriter.h"

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
//     Function: ColladaInput::get_column_name
//       Access: Public
//  Description: Returns the InternalName that is able to
//               represent this input. Note that an input with
//               semantic VERTICES does not have a column name -
//               instead, you have to get those from the inputs
//               in the target <vertices> element.
////////////////////////////////////////////////////////////////////
PT(InternalName) ColladaInput::
get_column_name() const {
  if (_semantic == "VERTICES") {
    collada_cat.error()
      << "Attempt to call get_column_name() on an input with VERTICES semantic\n";
    return NULL;
  } else if (_semantic == "BINORMAL") {
    return InternalName::get_binormal();
  } else if (_semantic == "COLOR") {
    return InternalName::get_color();
  } else if (_semantic == "NORMAL") {
    return InternalName::get_normal();
  } else if (_semantic == "POSITION") {
    return InternalName::get_vertex();
  } else if (_semantic == "TANGENT") {
    return InternalName::get_tangent();
  } else if (_semantic == "TEXCOORD") {
    return InternalName::get_texcoord();
  }
  collada_cat.warning()
    << "Unrecognized input semantic '" << _semantic << "' found\n";
  return InternalName::make(downcase(_semantic));
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

  PT(InternalName) cname = get_column_name();
  GeomEnums::Contents contents = GeomEnums::C_other;
  if (_semantic == "BINORMAL") {
    contents = GeomEnums::C_vector;
  } else if (_semantic == "COLOR") {
    contents = GeomEnums::C_color;
  } else if (_semantic == "NORMAL") {
    contents = GeomEnums::C_vector;
  } else if (_semantic == "POSITION") {
    contents = GeomEnums::C_point;
  } else if (_semantic == "TANGENT") {
    contents = GeomEnums::C_vector;
  } else if (_semantic == "TEXCOORD") {
    contents = GeomEnums::C_texcoord;
  } else {
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

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::write_data
//       Access: Public
//  Description: Writes data to the indicated GeomVertexData.
////////////////////////////////////////////////////////////////////
bool ColladaInput::
write_data(GeomVertexData *data, const PTA_int &p, int stride) const {
  CPT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, false);

  // If it's has the VERTEX semantic, it redirects to a <vertices> element.
  if (_semantic == "VERTEX") {
    PT(ColladaVertices) vertices = DCAST(ColladaVertices, doc->resolve_url(_source));
    nassertr(vertices != NULL, false);
    for (int i = 0; i < vertices->_inputs.size(); ++i) {
      vertices->_inputs[i]->write_data(data, p, stride);
    }
    return true;
  }

  PT(ColladaSource) source = DCAST(ColladaSource, doc->resolve_url(_source));
  nassertr(source != NULL, false);
  PTA_LVecBase4f values;
  nassertr(source->get_values(values), false);
  nassertr(_offset <= stride, false);

  GeomVertexWriter writer (data, get_column_name());
  for (size_t i = 0; i + _offset < p.size(); i += stride) {
    // Note: Panda may internally do a perspective divide for points
    // This is worked around in ColladaAccessor::get_values().
    writer.add_data4f(values[p[i + _offset]]);
  }
  return true;
}

