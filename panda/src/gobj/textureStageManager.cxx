// Filename: textureStageManager.cxx
// Created by: masad (15Jul04)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "pandabase.h"
#include "textureStageManager.h"
#include "preparedGraphicsObjects.h"

TextureStageManager *TextureStageManager::_global_ptr = NULL;

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::Constructor
//       Access: Private
//  Description: 
////////////////////////////////////////////////////////////////////
TextureStageManager::
TextureStageManager() {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::Destructor
//       Access: Private
//  Description: 
////////////////////////////////////////////////////////////////////
TextureStageManager::
~TextureStageManager() {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::make_stage
//       Access: Published
//  Description: Function that reads out of the datagram (or asks
////////////////////////////////////////////////////////////////////
PT(TextureStage) TextureStageManager::
make_stage (const string &name) {
  return (TextureStage*) NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::make_texcoord
//       Access: Published
//  Description: Function that reads out of the datagram (or asks
////////////////////////////////////////////////////////////////////
PT(TexCoordName) TextureStageManager::
make_texcoord (const string &name) {
  return (TexCoordName*) NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::write
//       Access: Published
//  Description: Function that reads out of the datagram (or asks
////////////////////////////////////////////////////////////////////
void TextureStageManager::
write (ostream &out) const {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::remove_stage
//       Access: Private
//  Description: 
//               This method is only called by the TextureStage
//               destructor.
////////////////////////////////////////////////////////////////////
void TextureStageManager::
remove_stage(TextureStage *stage) {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::remove_texcoord
//       Access: Private
//  Description: 
//               This method is only called by the TexCoordName
//               destructor.
////////////////////////////////////////////////////////////////////
void TextureStageManager::
remove_texcoord(TexCoordName *texcoord) {
}

