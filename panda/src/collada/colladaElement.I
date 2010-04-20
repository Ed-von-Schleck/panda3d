// Filename: colladaElement.I
// Created by: Xidram (15Apr10)
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


////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::clear
//       Access: Public
//  Description: Resets the ColladaElement to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
clear() {
  clear_name();
  clear_id();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::load_xml
//       Access: Public
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
//       Access: Public
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

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::set_id
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
set_id(const string &id) {
  _id = id;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::clear_id
//       Access: Public
//  Description: Resets the element's ID to empty.
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
clear_id() {
  _id = "";
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::has_id
//       Access: Public
//  Description: Returns true if the element has a nonempty ID set,
//               false if the ID is empty.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaElement::
has_id() const {
  return !_id.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::get_id
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaElement::
get_id() const {
  return _id;
}
