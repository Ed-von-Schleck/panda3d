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
//     Function: ColladaElement::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaElement::
ColladaElement() {
  _parent = NULL;
  clear();
}

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
//     Function: ColladaElement::set_name
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
set_name(const string &name) {
  _name = name;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::clear_name
//       Access: Public
//  Description: Resets the element's name to empty.
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
clear_name() {
  _name = "";
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::has_name
//       Access: Public
//  Description: Returns true if the element has a nonempty name set,
//               false if the name is empty.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaElement::
has_name() const {
  return !_name.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::get_name
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaElement::
get_name() const {
  return _name;
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

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::attach
//       Access: Protected
//  Description: If not NULL, attaches the given child node to
//               this node by simply setting its _parent member.
//               This is a convenience method for internal use only.
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
attach(ColladaElement *child) const {
  if (child == NULL) {
    return;
  }
  child->_parent = this;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaElement::detach
//       Access: Protected
//  Description: If not NULL, detaches the given child node from
//               this node by simply unsetting its _parent member.
//               This is a convenience method for internal use only.
////////////////////////////////////////////////////////////////////
INLINE void ColladaElement::
detach(ColladaElement *child) const {
  if (child == NULL) {
    return;
  }
  child->_parent = NULL;
}

