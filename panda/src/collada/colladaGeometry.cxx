// Filename: colladaGeometry.cxx
// Created by: rdb (20Apr10)
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

#include "colladaGeometry.h"
#include "colladaMesh.h"
#include "geomNode.h"

TypeHandle ColladaGeometry::_type_handle;
const string ColladaGeometry::_element_name ("geometry");
const string ColladaGeometry::_library_name ("library_geometries");

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaGeometry to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaGeometry::
clear () {
  ColladaAssetElement::clear();
  _geometry_type = GT_none;
  _geometric_element = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::load_xml
//       Access: Published, Virtual
//  Description: Loads <geometry> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaGeometry::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "geometry", false);

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("mesh");
  if (xchild != NULL) {
    _geometry_type = GT_mesh;
    _geometric_element = new ColladaMesh;
  }

  if (_geometric_element == NULL) {
    collada_cat.error() <<
      "<geometry> element does not have valid geometric subelement\n";
    return false;
  } else {
    _geometric_element->_parent = this;
    return _geometric_element->load_xml(xchild);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaGeometry::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("geometry");

  if (_geometric_element != NULL) {
    xelement->LinkEndChild(_geometric_element->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::make_node
//       Access: Published
//  Description: Returns a new PandaNode representing this element.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaGeometry::
make_node() const {
  if (_geometric_element == NULL || !_geometric_element->is_of_type(ColladaMesh::get_class_type())) {
    return new GeomNode(get_name());
  }

  PT(GeomNode) gnode = new GeomNode(get_name());

  pvector<PT(ColladaPrimitive)> primitives = DCAST(ColladaMesh, _geometric_element)->_primitives;
  pmap<string, PT(GeomNode)> geom_by_symbol;
  for (int i = 0; i < primitives.size(); ++i) {
    PT(Geom) geom = primitives[i]->make_geom();
    if (geom != NULL) {
      if (primitives[i]->has_material()) {
        const string &material = primitives[i]->get_material();
        if (geom_by_symbol.count(material) == 0) {
          geom_by_symbol[material] = new GeomNode(material);
        }
        geom_by_symbol[material]->add_geom(geom);
      } else {
        gnode->add_geom(geom);
      }
    }
  }

  // Collapse into a single GeomNode, if possible.
  if (geom_by_symbol.size() == 1 && gnode->get_num_geoms() == 0) {
    gnode = geom_by_symbol.begin()->second;
    gnode->set_name(get_name());
  } else {
    pmap<string, PT(GeomNode)>::const_iterator it;
    for (it = geom_by_symbol.begin(); it != geom_by_symbol.end(); ++it) {
      gnode->add_child(it->second);
    }
  }

  return DCAST(PandaNode, gnode);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::get_geometric_element
//       Access: Public
//  Description: Returns the ColladaElement that describes the
//               geometry. Use get_geometry_type to find out
//               of what type it is.
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaGeometry::
get_geometric_element() const {
  return _geometric_element;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaGeometry::get_element_by_id
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaGeometry::
get_element_by_id(const string &id) const {
  if (_geometric_element) {
    if (_geometric_element->get_id() == id) {
      return _geometric_element;
    } else {
      return _geometric_element->get_element_by_id(id);
    }
  }
  return NULL;
}

