// Filename: detourNavMeshNode.I
// Created by:  rdb (21Jul10)
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
//     Function: DetourNavMeshNode::get_area_cost
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE float DetourNavMeshNode::
get_area_cost(const int area) {
  return _nav_mesh->getAreaCost(area);
}

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::set_area_cost
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE void DetourNavMeshNode::
set_area_cost(const int area, float cost) {
  _nav_mesh->setAreaCost(area, cost);
}

