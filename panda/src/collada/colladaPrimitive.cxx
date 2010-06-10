// Filename: colladaPrimitive.cxx
// Created by: rdb (21Apr10)
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

#include "colladaPrimitive.h"

#include "geomLines.h"
#include "geomLinestrips.h"
#include "geomTriangles.h"
#include "geomTrifans.h"
#include "geomTristrips.h"

TypeHandle ColladaPrimitive::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaPrimitive::
ColladaPrimitive(PrimitiveType primitive_type) {
  _primitive_type = primitive_type;
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::clear
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaPrimitive::
clear() {
  ColladaElement::clear();
  _count = 0;
  _inputs.clear();
  _p.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::load_xml
//       Access: Public
//  Description: Loads <node> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaPrimitive::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  bool okflag = true;

  if (xelement->QueryIntAttribute("count", &_count) != TIXML_SUCCESS) {
    _count = 0;
    collada_cat.error()
      << "Invalid count attribute in " << xelement->Value() << "\n";
    okflag = false;
  }

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

  // Read out the <p> element
  xchild = xelement->FirstChildElement("p");
  while (xchild != NULL) {
    vector_string p;
    tokenize(trim(xchild->GetText()), p, COLLADA_WHITESPACE, true);
    vector_string::iterator it;
    for (it = p.begin(); it != p.end(); ++it) {
      _p.push_back(atoi(it->c_str()));
    }
    xchild = xchild->NextSiblingElement("p");
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaPrimitive::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  switch (_primitive_type) {
    case PT_lines:
      xelement->SetValue("lines");
      break;
    case PT_linestrips:
      xelement->SetValue("linestrips");
      break;
    case PT_polygons:
      xelement->SetValue("polygons");
      break;
    case PT_polylist:
      xelement->SetValue("polylist");
      break;
    case PT_triangles:
      xelement->SetValue("triangles");
      break;
    case PT_trifans:
      xelement->SetValue("trifans");
      break;
    case PT_tristrips:
      xelement->SetValue("tristrips");
      break;
    default:
      collada_cat.error() << "Invalid primitive type!\n";
      return NULL;
  }
  xelement->SetAttribute("count", _count);

  for (int i = 0; i < _inputs.size(); ++i) {
    xelement->LinkEndChild(_inputs[i]->make_xml());
  }

  if (_p.size() > 0) {
    ostringstream strm;
    TiXmlElement *xp = new TiXmlElement("p");
    for (int i = 0; i < _p.size(); ++i) {
      if (i != 0) {
        strm << " ";
      }
      strm << _p[i];
    }
    xp->LinkEndChild(new TiXmlText(strm.str()));
    xelement->LinkEndChild(xp);
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMesh::make_geom
//       Access: Published
//  Description: Returns a new Geom object representing this
//               COLLADA primitive object.
////////////////////////////////////////////////////////////////////
PT(Geom) ColladaPrimitive::
make_geom() const {
  PT(GeomVertexArrayFormat) aformat = new GeomVertexArrayFormat();
  for (int i = 0; i < _inputs.size(); ++i) {
    _inputs[i]->make_columns(aformat);
  }
  PT(GeomVertexFormat) format = new GeomVertexFormat();
  format->add_array(aformat);
  PT(GeomVertexData) vdata = new GeomVertexData(get_name(), GeomVertexFormat::register_format(format), GeomEnums::UH_static);
  PT(Geom) geom = new Geom(vdata);

  PT(GeomPrimitive) prim = NULL;
  switch (_primitive_type) {
    case PT_lines:
      prim = new GeomLines(GeomEnums::UH_static);
      break;
    case PT_linestrips:
      prim = new GeomLinestrips(GeomEnums::UH_static);
      break;
    case PT_polygons:
      collada_cat.error() << "<polygons> not yet supported!\n";
      break;
    case PT_polylist:
      collada_cat.error() << "<polylist> not yet supported!\n";
      break;
    case PT_triangles:
      prim = new GeomTriangles(GeomEnums::UH_static);
      break;
    case PT_trifans:
      prim = new GeomTrifans(GeomEnums::UH_static);
      break;
    case PT_tristrips:
      prim = new GeomTristrips(GeomEnums::UH_static);
      break;
    default:
      collada_cat.error() << "Invalid primitive type!\n";
  }
  if (prim != NULL) {
    geom->add_primitive(prim);
  }
  return geom;
}

