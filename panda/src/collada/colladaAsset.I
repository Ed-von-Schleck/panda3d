// Filename: colladaAsset.I
// Created by: Xidram (13Apr10)
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
//     Function: ColladaAsset::get_coordinate_system
//       Access: Public
//  Description: Returns the coordinate system in which the file
//               is defined.
////////////////////////////////////////////////////////////////////
INLINE CoordinateSystem ColladaAsset::
get_coordinate_system() const {
  return _coordsys;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::clear
//       Access: Public
//  Description: Resets the ColladaAsset to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
clear() {
  ColladaElement::clear();
  _coordsys = CS_default;
  time_t now = time(NULL);
  _created = *localtime(&now);
  _modified = *localtime(&now);
}

