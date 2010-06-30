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

#include "colladaRoot.h"
#include "colladaVisualScene.h"
#include "config_collada.h"
#include "config_util.h"
#include "config_express.h"
#include "string_utils.h"
#include "modelRoot.h"

TypeHandle ColladaRoot::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::clear
//       Access: Published, Virtual
//  Description: Resets the stored data of this ColladaDocument,
//               and makes it as if it were a new instance.
//               Note that the filename value is not cleared.
////////////////////////////////////////////////////////////////////
void ColladaRoot::
clear() {
  ColladaAssetElement::clear();
  _instance_visual_scene = NULL;
  _library_cameras = NULL;
  _library_controllers = NULL;
  _library_effects = NULL;
  _library_geometries = NULL;
  _library_lights = NULL;
  _library_materials = NULL;
  _library_nodes = NULL;
  _library_visual_scenes = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::load_xml
//       Access: Published, Virtual
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

  xchild = xelement->FirstChildElement("library_cameras");
  if (xchild != NULL) {
    _library_cameras = new ColladaLibraryCameras;
    _library_cameras->_parent = this;
    _library_cameras->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_controllers");
  if (xchild != NULL) {
    _library_controllers = new ColladaLibraryControllers;
    _library_controllers->_parent = this;
    _library_controllers->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_effects");
  if (xchild != NULL) {
    _library_effects = new ColladaLibraryEffects;
    _library_effects->_parent = this;
    _library_effects->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_geometries");
  if (xchild != NULL) {
    _library_geometries = new ColladaLibraryGeometries;
    _library_geometries->_parent = this;
    _library_geometries->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_lights");
  if (xchild != NULL) {
    _library_lights = new ColladaLibraryLights;
    _library_lights->_parent = this;
    _library_lights->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_materials");
  if (xchild != NULL) {
    _library_materials = new ColladaLibraryMaterials;
    _library_materials->_parent = this;
    _library_materials->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_nodes");
  if (xchild != NULL) {
    _library_nodes = new ColladaLibraryNodes;
    _library_nodes->_parent = this;
    _library_nodes->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_visual_scenes");
  if (xchild != NULL) {
    _library_visual_scenes = new ColladaLibraryVisualScenes;
    _library_visual_scenes->_parent = this;
    _library_visual_scenes->load_xml(xchild);
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
//       Access: Published, Virtual
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

  if (_library_cameras != NULL && _library_cameras->size() > 0) {
    xelement->LinkEndChild(_library_cameras->make_xml());
  }
  if (_library_controllers != NULL && _library_controllers->size() > 0) {
    xelement->LinkEndChild(_library_controllers->make_xml());
  }
  if (_library_effects != NULL && _library_effects->size() > 0) {
    xelement->LinkEndChild(_library_effects->make_xml());
  }
  if (_library_geometries != NULL && _library_geometries->size() > 0) {
    xelement->LinkEndChild(_library_geometries->make_xml());
  }
  if (_library_lights != NULL && _library_lights->size() > 0) {
    xelement->LinkEndChild(_library_lights->make_xml());
  }
  if (_library_materials != NULL && _library_materials->size() > 0) {
    xelement->LinkEndChild(_library_materials->make_xml());
  }
  if (_library_nodes != NULL && _library_nodes->size() > 0) {
    xelement->LinkEndChild(_library_nodes->make_xml());
  }
  if (_library_visual_scenes != NULL && _library_visual_scenes->size() > 0) {
    xelement->LinkEndChild(_library_visual_scenes->make_xml());
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
//       Access: Published
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

  LMatrix4f conv = LMatrix4f::convert_mat(get_coordinate_system(), CS_default);
  if (conv != LMatrix4f::ident_mat()) {
    node->set_transform(TransformState::make_mat(conv));
  }

  return DCAST(PandaNode, node);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::get_element_by_id
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaRoot::get_element_by_id(const string &id) const {
  PT(ColladaElement) result = NULL;
  if (_library_cameras != NULL){
    if (_library_cameras->get_id() == id) {
      return DCAST(ColladaElement, _library_cameras);
    }
    if ((result = _library_cameras->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_controllers != NULL){
    if (_library_controllers->get_id() == id) {
      return DCAST(ColladaElement, _library_controllers);
    }
    if ((result = _library_controllers->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_effects != NULL){
    if (_library_effects->get_id() == id) {
      return DCAST(ColladaElement, _library_effects);
    }
    if ((result = _library_effects->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_geometries != NULL){
    if (_library_geometries->get_id() == id) {
      return DCAST(ColladaElement, _library_geometries);
    }
    if ((result = _library_geometries->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_lights != NULL){
    if (_library_lights->get_id() == id) {
      return DCAST(ColladaElement, _library_lights);
    }
    if ((result = _library_lights->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_materials != NULL){
    if (_library_materials->get_id() == id) {
      return DCAST(ColladaElement, _library_materials);
    }
    if ((result = _library_materials->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_nodes != NULL){
    if (_library_nodes->get_id() == id) {
      return DCAST(ColladaElement, _library_nodes);
    }
    if ((result = _library_nodes->get_element_by_id(id))) {
      return result;
    }
  }
  if (_library_visual_scenes != NULL){
    if (_library_visual_scenes->get_id() == id) {
      return DCAST(ColladaElement, _library_visual_scenes);
    }
    if ((result = _library_visual_scenes->get_element_by_id(id))) {
      return result;
    }
  }
  return NULL;
}

