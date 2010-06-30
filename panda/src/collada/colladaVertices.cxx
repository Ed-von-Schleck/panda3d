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
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaVertices::
clear() {
  ColladaElement::clear();
  _inputs.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::load_xml
//       Access: Published, Virtual
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
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaVertices::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("vertices");

  for (size_t i = 0; i < _inputs.size(); ++i) {
    xelement->LinkEndChild(_inputs[i]->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::make_array_format
//       Access: Published
//  Description: Returns a new GeomVertexArrayFormat representing
//               this <vertices> element and its children.
////////////////////////////////////////////////////////////////////
PT(GeomVertexArrayFormat) ColladaVertices::
make_array_format() const {
  PT(GeomVertexArrayFormat) aformat = new GeomVertexArrayFormat;
  for (size_t i = 0; i < _inputs.size(); ++i) {
    _inputs[i]->make_column(aformat);
  }
  return aformat;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVertices::write_data
//       Access: Published
//  Description: Writes data to the indicated GeomVertexData.
////////////////////////////////////////////////////////////////////
bool ColladaVertices::
write_data(GeomVertexData *data) const {
  CPT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, false);

  for (size_t i = 0; i < _inputs.size(); ++i) {
    if (_inputs[i]->_semantic == "VERTEX") {
      collada_cat.error() << "Recursive vertex input\n";
      continue;
    }

    PT(ColladaSource) source = DCAST(ColladaSource, doc->resolve_url(_inputs[i]->_source));
    nassertd(source != NULL) continue;
    PTA_LVecBase4f values;
    nassertd(source->get_values(values)) continue;

    GeomVertexWriter writer (data, _inputs[i]->get_column_name());
    for (size_t i = 0; i < values.size(); ++i) {
      // NB: Panda may internally do a perspective divide for points.
      // This is worked around in ColladaAccessor::get_values().
      writer.add_data4f(values[i]);
    }
  }

  return true;
}

