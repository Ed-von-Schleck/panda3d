// Filename: textureStageManager.h
// Created by:  drose (14Jul04)
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

#ifndef TEXTURESTAGEMANAGER_H
#define TEXTURESTAGEMANAGER_H

#include "pandabase.h"
#include "textureStage.h"
#include "pointerTo.h"
#include "pmap.h"
#include "pvector.h"
#include "updateSeq.h"

////////////////////////////////////////////////////////////////////
//       Class : TextureStageManager
// Description : This is a global object that maintains the collection
//               of TextureStages and TexCoordNames in the world.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA TextureStageManager {
private:
  TextureStageManager();
  ~TextureStageManager();

PUBLISHED:
  INLINE TextureStage *get_default_stage();
  INLINE TexCoordName *get_default_texcoord();

  PT(TextureStage) make_stage(const string &name);
  PT(TexCoordName) make_texcoord(const string &name);

  INLINE int get_num_stages() const;
  INLINE TextureStage *get_stage(int n) const;
  INLINE int get_num_texcoords() const;
  INLINE TexCoordName *get_texcoord(int n) const;

  INLINE UpdateSeq get_sort_seq() const;

  void write(ostream &out) const;

  INLINE static TextureStageManager *get_global_ptr();

private:
  // This method is only called by the TextureStage destructor.
  void remove_stage(TextureStage *stage);
  // This method is only called by the TexCoordName destructor.
  void remove_texcoord(TexCoordName *texcoord);

  // This method is called by TextureStage::set_sort().
  INLINE void inc_sort_seq();

private:
  PT(TextureStage) _default_stage;
  PT(TexCoordName) _default_texcoords;

  // With the exception of the default stage, above, we don't store
  // reference-counting pointers to the TextureStage objects within
  // this class.  Instead, we let the TextureStage objects remove
  // themselves from the TextureStageManager as they destruct.
  typedef pvector<TextureStage *> Stages;
  Stages _stages;
  typedef pmap<string, TextureStage *> StagesByName;
  StagesByName _stages_by_name;

  typedef pvector<TexCoordName *> TexCoords;
  TexCoords _texcoords;
  typedef pmap<string, TexCoordName *> TexCoordsByName;
  TexCoordsByName _texcoords_by_name;

  UpdateSeq _sort_seq;

  static TextureStageManager *_global_ptr;

  friend class TextureStage;
};

#include "textureStageManager.I"

#endif
