// Filename: config_navigation.cxx
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

#include "config_navigation.h"

#include "dconfig.h"
#include "detourNavMeshNode.h"
#include "recastNavMesh.h"

ConfigureDef(config_navigation);
NotifyCategoryDef(navigation, "");


ConfigureFn(config_navigation) {
  init_libnavigation();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libnavigation
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libnavigation() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  DetourNavMeshNode::init_type();
  RecastNavMesh::init_type();
}

