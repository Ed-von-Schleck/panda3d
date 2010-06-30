// Filename: colladaPrimitive.I
// Created by:  rdb (30Jun10)
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
//     Function: ColladaPrimitive::get_primitive_type
//       Access: Published
//  Description: Returns the type of this primitive object.
////////////////////////////////////////////////////////////////////
INLINE ColladaPrimitive::PrimitiveType ColladaPrimitive::
get_primitive_type() const {
  return _primitive_type;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::has_material
//       Access: Published
//  Description: Returns true if this primitive has a material
//               symbol assigned.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaPrimitive::
has_material() const {
  return !_material.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::get_material
//       Access: Published
//  Description: Returns the material symbol or an empty string if
//               this primitive has no material assigned.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaPrimitive::
get_material() const {
  return _material;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::set_material
//       Access: Published
//  Description: Sets the material symbol for this primitive.
////////////////////////////////////////////////////////////////////
INLINE void ColladaPrimitive::
set_material(const string &symbol) {
  _material = symbol;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaPrimitive::clear_material
//       Access: Published
//  Description: Clears the material symbol for this primitive.
////////////////////////////////////////////////////////////////////
INLINE void ColladaPrimitive::
clear_material() {
  _material = "";
}

