// Filename: recastNavMesh.cxx
// Created by:  rdb (20Jul10)
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

#include "recastNavMesh.h"

TypeHandle RecastNavMesh::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////

RecastNavMesh::
RecastNavMesh()
{
  _cell_size = 0;
  _cell_height = 0;
  _walkable_slope_angle = 0;
  _walkable_height = 0;
  _walkable_climb = 0;
  _walkable_radius = 0;
  _max_edge_length = 0;
  _max_simplification_error = 0;
  _min_region_size = 0;
  _merge_region_size = 0;
  _detail_sample_distance = 0;
  _detail_sample_max_error = 0;
}

