// Filename: colladaVisualScene.cxx
// Created by: rdb (13Apr10)
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

#include "colladaVisualScene.h"

TypeHandle ColladaVisualScene::_type_handle;
const string ColladaVisualScene::_element_name ("visual_scene");
const string ColladaVisualScene::_library_name ("library_visual_scenes");

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaVisualScene::
ColladaVisualScene() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::load_xml
//       Access: Public
//  Description: Loads <visual_scene> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaVisualScene::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  nassertr(xelement->ValueStr() == "visual_scene", false);

  const TiXmlElement* xnode = xelement->FirstChildElement("node");
  while (xnode != NULL) {
    PT(ColladaNode) node = new ColladaNode;
    node->_parent = this;
    node->load_xml(xnode);
    _nodes.push_back(node);
    xnode = xnode->NextSiblingElement("node");
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaVisualScene::
make_xml() const {
  TiXmlElement * xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("visual_scene");

  for (int i = 0; i < _nodes.size(); ++i) {
    xelement->LinkEndChild(_nodes.at(i)->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::make_node
//       Access: Public
//  Description: Returns a new ModelNode representing this
//               COLLADA visual scene and the nodes below it.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaVisualScene::
make_node() const {
  PT(ModelNode) pnode = new ModelNode(get_name());

  for (int i = 0; i < _nodes.size(); ++i) {
    pnode->add_child(_nodes.at(i)->make_node());
  }

  return DCAST(PandaNode, pnode);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::get_element_by_id
//       Access: Public
//  Description: Returns the node in the node hierarchy that has
//               the given ID, or NULL if none found.
//               Searches recursively for subnodes as well.
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaVisualScene::
get_element_by_id(const string &id) const {
  PT(ColladaElement) res;
  for (int i = 0; i < _nodes.size(); ++i) {
    res = _nodes[i];
    if (res->get_id() == id) {
      return DCAST(ColladaElement, res);
    } else {
      res = res->get_element_by_id(id);
      if (res != NULL) {
        return DCAST(ColladaElement, res);
      }
    }
  }
  return NULL;
}

