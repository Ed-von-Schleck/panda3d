// Filename: detourNavMeshNode.cxx
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

#include "detourNavMeshNode.h"

TypeHandle DetourNavMeshNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
DetourNavMeshNode::
DetourNavMeshNode(const string &name) : PandaNode(name) {
}

