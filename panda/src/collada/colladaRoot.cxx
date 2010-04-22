// Filename: colladaRoot.cxx
// Created by:  rdb (21Apr10)
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

#include "colladaDocument.h"
#include "config_collada.h"
#include "config_util.h"
#include "config_express.h"
#include "string_utils.h"
#include "modelRoot.h"

TypeHandle ColladaRoot::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::clear
//       Access: Public
//  Description: Resets the stored data of this ColladaDocument,
//               and makes it as if it were a new instance.
//               Note that the filename value is not cleared.
////////////////////////////////////////////////////////////////////
void ColladaRoot::
clear() {
  ColladaAssetElement::clear();
  _instance_visual_scene = NULL;
  _library_effects.clear();
  _library_geometries.clear();
  _library_materials.clear();
  _library_nodes.clear();
  _library_visual_scenes.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::load_xml
//       Access: Public
//  Description: Parses the dae syntax contained in the indicated
//               TiXmlElement.  Returns true if the stream was a
//               completely valid dae file, false if there were some
//               errors, in which case the data may be partially read.
//
//               Before you call this routine, you should probably
//               call set_filename() to set the name of the dae
//               file we're processing, if at all possible.  If there
//               is no such filename, you may set it to the empty
//               string.
////////////////////////////////////////////////////////////////////
bool ColladaRoot::
load_xml(const TiXmlElement *xelement) {
  clear();

  nassertr(xelement != NULL, false);

  if (xelement->ValueStr() != "COLLADA") {
    collada_cat.error() << "Root element must be <COLLADA>, not <" << xelement->Value() << ">\n";
    return false;
  }

  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("library_effects");
  if (xchild != NULL) {
    _library_effects.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_geometries");
  if (xchild != NULL) {
    _library_geometries.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_materials");
  if (xchild != NULL) {
    _library_materials.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_nodes");
  if (xchild != NULL) {
    _library_nodes.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_visual_scenes");
  if (xchild != NULL) {
    _library_visual_scenes.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("scene");
  if (xchild != NULL) {
    const TiXmlElement *xinst;
    xinst = xchild->FirstChildElement("instance_visual_scene");
    if (xinst != NULL) {
      _instance_visual_scene = new ColladaInstanceVisualScene;
      _instance_visual_scene->_parent = this;
      _instance_visual_scene->load_xml(xinst);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::make_xml
//       Access: Public
//  Description: Returns the root <COLLADA> element of the COLLADA
//               document as new TiXmlElement. This contains the
//               entire structure of the COLLADA document.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaRoot::
make_xml() const {
  TiXmlElement * xelement = ColladaAssetElement::make_xml();
  if (xelement == NULL) {
    return NULL;
  }
  xelement->SetValue("COLLADA");

  // Currently, we write valid 1.5.0 collada
  xelement->SetAttribute("version", "1.5.0");
  xelement->SetAttribute("xmlns", "http://www.collada.org/2008/03/COLLADASchema");

  if (!_asset) {
    //FIXME: what to do when there is no asset? collada spec requires one, I'm fairly certain
  }

  if (_library_effects.size() > 0) {
    xelement->LinkEndChild(_library_effects.make_xml());
  }
  if (_library_geometries.size() > 0) {
    xelement->LinkEndChild(_library_geometries.make_xml());
  }
  if (_library_materials.size() > 0) {
    xelement->LinkEndChild(_library_materials.make_xml());
  }
  if (_library_nodes.size() > 0) {
    xelement->LinkEndChild(_library_nodes.make_xml());
  }
  if (_library_visual_scenes.size() > 0) {
    xelement->LinkEndChild(_library_visual_scenes.make_xml());
  }

  TiXmlElement * xscene = new TiXmlElement("scene");
  if (_instance_visual_scene != NULL) {
    xscene->LinkEndChild(_instance_visual_scene->make_xml());
  }
  xelement->LinkEndChild(xscene);

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::make_node
//       Access: Public
//  Description: The main interface for loading COLLADA data
//               into the scene graph. Returns a ModelRoot
//               representing the ColladaDocument and its children.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaRoot::
make_node() const {
  PT(ModelRoot) node = new ModelRoot(_filename.get_basename());

  if (_instance_visual_scene != NULL) {
    CPT(ColladaVisualScene) vis_scene = DCAST(ColladaVisualScene, _instance_visual_scene->get_target());
    if (vis_scene != NULL) {
      node->add_child(vis_scene->make_node());
    }
  }

  return DCAST(PandaNode, node);
}

