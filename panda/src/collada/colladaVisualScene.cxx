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
  _asset = NULL;
  clear_name();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::load_xml
//       Access: Public
//  Description: Loads <visual_scene> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaVisualScene::
load_xml(const TiXmlElement *xelement) {
  nassertr(xelement != NULL, false);
  nassertr(xelement->ValueStr() == "visual_scene", false);

  clear_name();
  _nodes.clear();

  const char* name = xelement->Attribute("name");
  if (name) {
    set_name(name);
  }

  const TiXmlElement* xnode = xelement->FirstChildElement("node");
  while (xnode != NULL) {
    PT(ColladaNode) node = new ColladaNode;
    node->load_xml(xnode);
    _nodes.push_back(node);
    xnode = xnode->NextSiblingElement("node");
  }

  const TiXmlElement* xasset = xelement->FirstChildElement("asset");
  if (xasset != NULL) {
    _asset = new ColladaAsset();
    _asset->load_xml(xasset);
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
  TiXmlElement * xelement = new TiXmlElement("visual_scene");

  if (has_name()) {
    xelement->SetAttribute("name", get_name());
  }

  if (_asset) {
    xelement->LinkEndChild(_asset->make_xml());
  }

  for (int i = 0; i < _nodes.size(); ++i) {
    xelement->LinkEndChild(_nodes.at(i)->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaVisualScene::make_node
//       Access: Public
//  Description: Returns a new ModelRoot representing this
//               COLLADA visual scene and the nodes below it.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaVisualScene::
make_node() const {
  PT(ModelRoot) pnode = new ModelRoot(get_name());

  for (int i = 0; i < _nodes.size(); ++i) {
    pnode->add_child(_nodes.at(i)->make_node());
  }

  return DCAST(PandaNode, pnode);
}
