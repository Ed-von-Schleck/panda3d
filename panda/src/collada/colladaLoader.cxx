// Filename: colladaLoader.cxx
// Created by:  rdb (23Aug09)
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

#include "colladaLoader.h"
#include "config_collada.h"

#include "string_utils.h"
#include "modelRoot.h"
#include "virtualFileSystem.h"

////////////////////////////////////////////////////////////////////
//     Function: ColladaLoader::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaLoader::
ColladaLoader() : _error(false), _root(NULL), _record(NULL) {
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLoader::build_graph
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaLoader::
build_graph() {
  _root = new ModelRoot(_filename.get_basename());
}

