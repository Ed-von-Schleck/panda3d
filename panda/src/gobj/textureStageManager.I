// Filename: textureStageManager.I
// Created by:  masad (15Jul04)
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


////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_default_stage
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE TextureStage *TextureStageManager::
get_default_stage() {
  return _default_stage;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_default_texcoord
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE TexCoordName *TextureStageManager::
get_default_texcoord() {
  return _default_texcoord;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_num_stage
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE int TextureStageManager::
get_num_stages() const {
  return _stages.size();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_stage
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE TextureStage *TextureStageManager::
get_stage(int n) const {
  // or we can use the at() to throw exception if n is out of bound
  return _stages.at(n);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_num_texcoords
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE int TextureStageManager::
get_num_texcoords() const {
  return _texcoords.size();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_texcoord
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE TexCoordName *TextureStageManager::
get_texcoord(int n) const {
  // or we can use the at() to throw exception if n is out of bound
  return _texcoords.at(n);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_sort_seq
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE UpdateSeq TextureStageManager::
get_sort_seq() const {
  return _sort_seq;
}


////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::inc_sort_seq
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void TextureStageManager::
inc_sort_seq() {
  ++_sort_seq;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::get_global_ptr
//       Access: Published, Static
//  Description: Returns a pointer to the one global
//               TextureStageManager object.
////////////////////////////////////////////////////////////////////
INLINE TextureStageManager *TextureStageManager::
get_global_ptr() {
  if (_global_ptr == (TextureStageManager *)NULL) {
    _global_ptr = new TextureStageManager;
  }
  return _global_ptr;
}

