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
