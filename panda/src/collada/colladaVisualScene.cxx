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

  return xelement;
}
