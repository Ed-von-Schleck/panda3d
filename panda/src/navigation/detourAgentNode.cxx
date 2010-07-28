// Filename: detourAgentNode.cxx
// Created by: Xidram (28Jul10)
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

#include "detourAgentNode.h"

TypeHandle DetourAgentNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
DetourAgentNode::
DetourAgentNode(const string &name) : PandaNode(name) {
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::get_nav_mesh
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
CPT(DetourNavMeshNode) DetourAgentNode::
get_nav_mesh() {
  PT(PandaNode) parent = get_parent();
  while (parent != NULL && !parent->is_of_type(DetourNavMeshNode::get_class_type())) {
    parent = parent->get_parent();
  }
  return parent;
}

