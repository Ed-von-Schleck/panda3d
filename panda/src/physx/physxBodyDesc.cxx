// Filename: physxBodyDesc.cxx
// Created by:  enn0x (05Sep09)
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

#include "physxBodyDesc.h"

TypeHandle PhysxBodyDesc::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxBodyDesc::set_mass
//       Access: Published
//  Description: Set the mass of body. 
////////////////////////////////////////////////////////////////////
void PhysxBodyDesc::
set_mass(float mass) {

  _desc.mass = mass;
}

