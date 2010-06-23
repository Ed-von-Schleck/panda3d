// Filename: colladaRoot.I
// Created by:  rdb (21Apr10)
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

#include "dcast.h"

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaRoot::
ColladaRoot(PT(ColladaDocument) document) {
  _document = document;
  clear();
  _library_cameras = NULL;
  _library_controllers = NULL;
  _library_effects = NULL;
  _library_geometries = NULL;
  _library_lights = NULL;
  _library_materials = NULL;
  _library_nodes = NULL;
  _library_visual_scenes = NULL;
  nassertv(document != NULL);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaRoot::get_document
//       Access: Public
//  Description: Returns the document that this root belongs to.
//               This can never be NULL.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaDocument) ColladaRoot::
get_document() const {
  return _document;
}

