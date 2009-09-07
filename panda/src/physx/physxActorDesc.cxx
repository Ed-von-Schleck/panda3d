// Filename: physxActorDesc.cxx
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

#include "physxActorDesc.h"
#include "physxManager.h"

TypeHandle PhysxActorDesc::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxActorDesc::set_name
//       Access: Published
//  Description: Sets the optional debug name for the actor.
////////////////////////////////////////////////////////////////////
void PhysxActorDesc::
set_name(const char *name) {

  _desc.name = name;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActorDesc::set_density
//       Access: Published
//  Description: Set the density used during mass/intertia
//               computation. This value is used if the actor's
//               shapes do not have a mass asigned.
////////////////////////////////////////////////////////////////////
void PhysxActorDesc::
set_density(float density) {

  _desc.density = density;
}

