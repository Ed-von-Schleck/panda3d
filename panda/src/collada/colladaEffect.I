// Filename: colladaEffect.I
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
//     Function: ColladaEffect::add_profile
//       Access: Public
//  Description: Adds a profile to the ColladaEffect's vector of profiles.
////////////////////////////////////////////////////////////////////
INLINE void ColladaEffect::
add_profile(PT(ColladaProfile) profile) {
  _profiles.push_back(profile);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaEffect::get_profile
//       Access: Public
//  Description: Returns the profile at a certain index of the
//               ColladaEffect's vector of profiles.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaProfile) ColladaEffect::
get_profile(int index) const {
  return _profiles.at(index);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaEffect::get_profile
//       Access: Public
//  Description: Returns the profile at a certain index of the
//               ColladaEffect's vector of profiles.
////////////////////////////////////////////////////////////////////
INLINE int ColladaEffect::
get_num_profiles() const {
  return _profiles.size();
}

