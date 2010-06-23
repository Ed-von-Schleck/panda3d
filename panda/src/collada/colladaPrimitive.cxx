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
  _ps.clear();
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

  // Read out the <p> element(s)
  xchild = xelement->FirstChildElement("p");
  while (xchild != NULL) {
    vector_string p;
    tokenize(trim(xchild->GetText()), p, COLLADA_WHITESPACE, true);
    PTA_int array = PTA_int::empty_array(p.size());
    for (size_t i = 0; i < p.size(); ++i) {
      array[i] = atoi(p[i].c_str());
    }
    _ps.push_back(array);
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

  // These elements may only contain one <p> tag
  if (_primitive_type == PT_lines ||
      _primitive_type == PT_polylist ||
      _primitive_type == PT_triangles) {

    // So let's merge all <p> tags together into one
    if (_ps.size() > 0) {
      ostringstream strm;
      TiXmlElement *xp = new TiXmlElement("p");
      for (size_t i = 0; i < _ps.size(); ++i) {
        for (size_t j = 0; j < _ps[i].size(); ++j) {
          if (i != 0 || j != 0) {
            strm << " ";
          }
          strm << _ps[i][j];
        }
      }
      xp->LinkEndChild(new TiXmlText(strm.str()));
      xelement->LinkEndChild(xp);
    }
  } else {
    // Other elements may contain multiple <p> tags
    for (size_t i = 0; i < _ps.size(); ++i) {
      ostringstream strm;
      TiXmlElement *xp = new TiXmlElement("p");
      for (size_t j = 0; j < _ps[i].size(); ++j) {
        if (j != 0) {
          strm << " ";
        }
        strm << _ps[i][j];
      }
      xp->LinkEndChild(new TiXmlText(strm.str()));
      xelement->LinkEndChild(xp);
    }
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
  for (size_t i = 0; i < _inputs.size(); ++i) {
    _inputs[i]->make_columns(aformat);
  }
  PT(GeomVertexFormat) format = new GeomVertexFormat();
  format->add_array(aformat);
  PT(GeomVertexData) vdata = new GeomVertexData(get_name(), GeomVertexFormat::register_format(format), GeomEnums::UH_static);

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
  nassertr(prim != NULL, NULL);

  int stride = 1;
  for (size_t i = 0; i < _inputs.size(); ++i) {
    if (_inputs[i]->_offset >= stride) {
      stride = _inputs[i]->_offset + 1;
    }
  }

  for (size_t i = 0; i < _ps.size(); ++i) {
    for (size_t j = 0; j < _inputs.size(); ++j) {
      _inputs[j]->write_data(vdata, _ps[i], stride);
    }
    //TODO: add <vcount> support for polylists
    if (_primitive_type == PT_lines) {
      for (size_t j = 0; j < _count; ++j) {
        prim->add_next_vertices(2);
        prim->close_primitive();
      }
    } else if (_primitive_type == PT_triangles) {
      for (size_t j = 0; j < _count; ++j) {
        prim->add_next_vertices(3);
        prim->close_primitive();
      }
    } else {
      prim->add_next_vertices(_count);
      prim->close_primitive();
    }
  }

  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(prim);
  return geom;
}
