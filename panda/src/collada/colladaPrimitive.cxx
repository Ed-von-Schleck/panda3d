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
#include "geomVertexWriter.h"

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
  _vcount = NULL;
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

  // Read out the <vcount> element if it's the right primitive type
  if (_primitive_type == PT_polylist) {
    xchild = xelement->FirstChildElement("vcount");
    if (xchild != NULL) {
      vector_string v;
      tokenize(trim(xchild->GetText()), v, COLLADA_WHITESPACE, true);
      _vcount = PTA_int::empty_array(v.size());
      for (size_t i = 0; i < v.size(); ++i) {
        _vcount[i] = atoi(v[i].c_str());
      }
    }
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

  if (_vcount != NULL && _primitive_type == PT_polylist) {
    ostringstream strm;
    TiXmlElement *xvcount = new TiXmlElement("vcount");
    for (size_t i = 0; i < _vcount.size(); ++i) {
      if (i != 0) {
        strm << " ";
      }
      strm << _vcount[i];
    }
    xvcount->LinkEndChild(new TiXmlText(strm.str()));
    xelement->LinkEndChild(xvcount);
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
  CPT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, NULL);

  PT(ColladaInput) vinput = NULL;
  PT(ColladaVertices) vertices = NULL;
  // First, find the input with the VERTEX semantic.
  for (size_t i = 0; i < _inputs.size(); ++i) {
    if (_inputs[i]->_semantic == "VERTEX") {
      if (vertices != NULL) {
        collada_cat.error()
          << "Multiple inputs with VERTEX semantic found!\n";
        break;
      }
      vinput = _inputs[i];
      vertices = DCAST(ColladaVertices, doc->resolve_url(vinput->_source));
    }
  }
  nassertr(vertices != NULL, NULL); // No <vertices> element found


  PT(GeomVertexArrayFormat) aformat = new GeomVertexArrayFormat;
  for (size_t i = 0; i < _inputs.size(); ++i) {
    _inputs[i]->make_column(aformat);
  }
  PT(GeomVertexFormat) format = new GeomVertexFormat();
  format->add_array(vertices->make_array_format());
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
      // Automatically triangulated to tristrips
      prim = new GeomTristrips(GeomEnums::UH_static);
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

  vertices->write_data(vdata);

  int offset = 0;
  for (size_t i = 0; i < _ps.size(); ++i) {
    // Get all of the indices of the VERTEX input first
    pvector<int> vindices;
    for (size_t p = vinput->_offset; p + vinput->_offset < _ps[i].size(); p += stride) {
      vindices.push_back(_ps[i][p]);
    }
    // Then loop through all of the other inputs
    for (size_t j = 0; j < _inputs.size(); ++j) {
      if (_inputs[j]->_semantic != "VERTEX") {
        PT(ColladaSource) source = DCAST(ColladaSource, doc->resolve_url(_inputs[j]->_source));
        nassertd(source != NULL) continue;
        PTA_LVecBase4f values;
        nassertd(source->get_values(values)) continue;
        GeomVertexWriter writer (vdata, _inputs[j]->get_column_name());
        for (size_t v = 0; v < vindices.size(); ++v) {
          writer.set_row(vindices[v]);
          writer.set_data4f(values[_ps[i][v * stride + _inputs[j]->_offset]]);
        }
      }
    }
    if (_primitive_type == PT_polylist) {
      nassertr(_vcount != NULL, NULL);
      // Triangulate to tristrips, that seems easiest
      for (size_t p = 0; p < _count; ++p) {
        for (size_t v = 0; v < _vcount[p]; ++v) {
          if (v <= 1) { // 0 and 1
            prim->add_vertex(vindices[offset + v]);
          } else if (v % 2) { // uneven
            prim->add_vertex(vindices[offset + (v + 1) / 2]);
          } else { // even
            prim->add_vertex(vindices[offset + _vcount[p] - v / 2]);
          }
        }
        offset += _vcount[p];
        prim->close_primitive();
      }
    } else if (_primitive_type == PT_lines) {
      for (size_t j = 0; j < _count; ++j) {
        for (char v = 0; v < 2; ++v) {
          prim->add_vertex(vindices[offset + v]);
        }
        offset += 2;
        prim->close_primitive();
      }
    } else if (_primitive_type == PT_triangles) {
      for (size_t j = 0; j < _count; ++j) {
        for (char v = 0; v < 3; ++v) {
          prim->add_vertex(vindices[offset + v]);
        }
        offset += 3;
        prim->close_primitive();
      }
    } else {
      for (size_t v = 0; v < _count; ++v) {
        prim->add_vertex(vindices[offset + v]);
      }
      offset += _count;
      prim->close_primitive();
    }
  }

  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(prim);
  return geom;
}
