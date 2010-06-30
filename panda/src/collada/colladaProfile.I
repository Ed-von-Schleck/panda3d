// Filename: colladaProfile.I
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
//     Function: ColladaProfile::Constructor
//       Access: Published
//  Description: Constructs a new ColladaProfile object.
////////////////////////////////////////////////////////////////////
INLINE ColladaProfile::
ColladaProfile(Profile profile) : _profile(profile) {
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaProfile::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaProfile to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaProfile::
clear() {
  ColladaAssetElement::clear();
}

