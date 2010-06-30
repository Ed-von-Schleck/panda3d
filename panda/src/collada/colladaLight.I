// Filename: colladaLight.I
// Created by: Xidram (20Apr10)
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
//     Function: ColladaLight::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaLight to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaLight::
clear() {
  ColladaAssetElement::clear();
  _light_type = LT_invalid;
}

