// Filename: colladaData.I
// Created by:  rdb (13Apr10)
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
//     Function: ColladaData::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaData::
ColladaData() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::get_coordinate_system
//       Access: Public
//  Description: Returns the coordinate system in which the file
//               is defined. Note that this returns the global
//               coordinate system for the entire file - it is
//               possible that elements in the document override
//               this value.
////////////////////////////////////////////////////////////////////
INLINE CoordinateSystem ColladaData::
get_coordinate_system() const {
  return _asset ? _asset->get_coordinate_system() : CS_yup_right;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::set_filename
//       Access: Public
//  Description: Sets the filename--especially the directory part--in
//               which the file is considered to reside.  This is
//               also implicitly set by read().
////////////////////////////////////////////////////////////////////
INLINE void ColladaData::
set_filename(const Filename &filename) {
  _filename = filename;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::get_filename
//       Access: Public
//  Description: Returns the directory in which the collada file is
//               considered to reside.
////////////////////////////////////////////////////////////////////
INLINE const Filename &ColladaData::
get_filename() const {
  return _filename;
}

