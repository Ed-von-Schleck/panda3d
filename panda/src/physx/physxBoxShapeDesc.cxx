// Filename: physxBoxShapeDesc.cxx
// Created by:  enn0x (08Sep09)
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

#include "physxBoxShapeDesc.h"
#include "physxManager.h"

TypeHandle PhysxBoxShapeDesc::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxBoxShapeDesc::set_dimensions
//       Access: Published
//  Description: Sets the dimensions of the box.
//
//               The dimensions are the 'radii' of the box,
//               meaning 1/2 extents in x dimension, 1/2 extents
//               in y dimension, 1/2 extents in z dimension.
////////////////////////////////////////////////////////////////////
void PhysxBoxShapeDesc::
set_dimensions(const LVector3f &dimensions) {

  _desc.dimensions = PhysxManager::vec3_to_nxVec3(dimensions);
}

