// Filename: colladaElement.cxx
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

#include "colladaElement.h"

TypeHandle ColladaElement::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::load_xml
//       Access: Published, Virtual
//  Description: Loads data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaElement::
load_xml(const TiXmlElement *xelement) {
  clear();
  nassertr (xelement != NULL, false);

  const char* name = xelement->Attribute("name");
  if (name) {
    set_name(name);
  }

  const char* id = xelement->Attribute("id");
  if (id) {
    set_id(id);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaElement::
make_xml() const {
  TiXmlElement * xelement = new TiXmlElement("");

  if (has_name()) {
    xelement->SetAttribute("name", get_name());
  }

  if (has_id()) {
    xelement->SetAttribute("id", get_id());
  }

  return xelement;
}

