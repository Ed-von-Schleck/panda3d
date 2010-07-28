// Filename: detourAgentNode.h
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

#ifndef DETOURAGENTNODE_H
#define DETOURAGENTNODE_H

#include "config_navigation.h"
#include "detourNavMeshNode.h"
#include "pandaNode.h"

////////////////////////////////////////////////////////////////////
//       Class : DetourAgentNode
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_NAVIGATION DetourAgentNode : public PandaNode {
PUBLISHED:
  DetourAgentNode(const string &name);
  CPT(DetourNavMeshNode) get_nav_mesh();

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PandaNode::init_type();
    register_type(_type_handle, "DetourAgentNode",
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#endif

