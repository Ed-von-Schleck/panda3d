// Filename: colladaMesh.cxx
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

#include "colladaMesh.h"

TypeHandle ColladaMesh::_type_handle;

void parse_semantic (const string &semantic, PT(InternalName) &cname, GeomEnums::Contents &contents) {
  cname = NULL;
  contents = GeomEnums::C_other;
  if (semantic == "BINORMAL") {
    cname = InternalName::get_binormal();
    contents = GeomEnums::C_vector;
  } else if (semantic == "COLOR") {
    cname = InternalName::get_color();
    contents = GeomEnums::C_color;
  } else if (semantic == "NORMAL") {
    cname = InternalName::get_normal();
    contents = GeomEnums::C_vector;
  } else if (semantic == "POSITION") {
    cname = InternalName::get_vertex();
    contents = GeomEnums::C_point;
  } else if (semantic == "TANGENT") {
    cname = InternalName::get_tangent();
    contents = GeomEnums::C_vector;
  } else {
    cname = InternalName::make(downcase(semantic));
    contents = GeomEnums::C_other;
    collada_cat.warning() << "Unrecognized input semantic '" << semantic << "' found\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::clear
//       Access: Public
//  Description: Resets the ColladaMesh to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaMesh::
clear () {
  ColladaElement::clear();
  _sources.clear();
  _vertices = NULL;
  _primitives.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::load_xml
//       Access: Public
//  Description: Loads <mesh> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaMesh::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "mesh", false);

  bool okflag = true;
  const TiXmlElement *xchild;

  // Read out the <source> elements
  xchild = xelement->FirstChildElement("source");
  while (xchild != NULL) {
    PT(ColladaSource) source = new ColladaSource;
    source->_parent = this;
    source->load_xml(xchild);
    _sources.push_back(source);
    xchild = xchild->NextSiblingElement("source");
  }

  // Read out the <vertices> element
  xchild = xelement->FirstChildElement("vertices");
  if (xchild == NULL) {
    collada_cat.error() << "No <vertices> found in <mesh>!\n";
    okflag = false;
  } else {
    _vertices = new ColladaVertices;
    _vertices->_parent = this;
    _vertices->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement();
  while (xchild != NULL) {
    const string &value = xchild->ValueStr();
    PT(ColladaPrimitive) prim = NULL;
    if (value == "lines") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_lines);
    } else if (value == "linestrips") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_linestrips);
    } else if (value == "polygons") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_polygons);
    } else if (value == "polylist") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_polylist);
    } else if (value == "triangles") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_triangles);
    } else if (value == "trifans") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_trifans);
    } else if (value == "tristrips") {
      prim = new ColladaPrimitive(ColladaPrimitive::PT_tristrips);
    }
    if (prim != NULL) {
      prim->_parent = this;
      prim->load_xml(xchild);
      _primitives.push_back(prim);
    }
    xchild = xchild->NextSiblingElement();
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaMesh::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("mesh");

  for (int i = 0; i < _sources.size(); ++i) {
    xelement->LinkEndChild(_sources[i]->make_xml());
  }

  if (_vertices != NULL) {
    xelement->LinkEndChild(_vertices->make_xml());
  }

  for (int i = 0; i < _primitives.size(); ++i) {
    xelement->LinkEndChild(_primitives[i]->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::get_element_by_id
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaMesh::
get_element_by_id(const string &id) const {
  PT(ColladaElement) result = NULL;

  for (int i = 0; i < _sources.size(); ++i) {
    if (_sources[i]->get_id() == id) {
      return DCAST(ColladaElement, _sources[i]);
    } else {
      result = _sources[i]->get_element_by_id(id);
      if (result != NULL) {
        return result;
      }
    }
  }

  if (_vertices != NULL) {
    if (_vertices->get_id() == id) {
      return DCAST(ColladaElement, _vertices);
    }
    result = _vertices->get_element_by_id(id);
    if (result != NULL) {
      return result;
    }
  }

  for (int i = 0; i < _primitives.size(); ++i) {
    if (_primitives[i]->get_id() == id) {
      return DCAST(ColladaElement, _primitives[i]);
    } else {
      result = _primitives[i]->get_element_by_id(id);
      if (result != NULL) {
        return result;
      }
    }
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::make_geom
//       Access: Published
//  Description: Returns a new Geom object representing this
//               COLLADA mesh object.
////////////////////////////////////////////////////////////////////
PT(Geom) ColladaMesh::
make_geom() const {

}

