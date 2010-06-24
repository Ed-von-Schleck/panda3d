// Filename: colladaController.cxx
// Created by:  rdb (23Jun10)
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

#include "colladaController.h"
#include "colladaGeometry.h"
#include "character.h"

TypeHandle ColladaController::_type_handle;
const string ColladaController::_element_name ("controller");
const string ColladaController::_library_name ("library_controllers");

////////////////////////////////////////////////////////////////////
//     Function: ColladaController::clear
//       Access: Public
//  Description: Resets the ColladaController to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaController::
clear () {
  ColladaAssetElement::clear();
  _source = "";
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaController::load_xml
//       Access: Public
//  Description: Loads <controller> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaController::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "controller", false);

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("skin");
  if (xchild == NULL) {
    xchild = xelement->FirstChildElement("morph");
  }
  if (xchild == NULL) {
    collada_cat.error()
      << "Controller should have either <skin> or <morph>!\n";
  }
  if (xchild != NULL && xchild->Attribute("source")) {
    _source = xchild->Attribute("source");
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaController::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaController::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("controller");

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaController::make_node
//       Access: Public
//  Description: Returns a new PandaNode representing this element.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaController::
make_node() const {
  PT(Character) cnode = new Character(get_name());

  if (!_source.empty()) {
    CPT(ColladaDocument) doc = get_document();
    nassertr(doc != NULL, false);
    PT(ColladaGeometry) geom = DCAST(ColladaGeometry, doc->resolve_url(_source));
    nassertr(geom != NULL, false);
    cnode->add_child(geom->make_node());
  }

  return DCAST(PandaNode, cnode);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaController::get_element_by_id
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaController::
get_element_by_id(const string &id) const {
  return NULL;
}

