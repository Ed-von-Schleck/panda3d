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
//               of named TextureStages in the world.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA TextureStageManager {
private:
  TextureStageManager();
  ~TextureStageManager();

PUBLISHED:
  INLINE TextureStage *get_default_stage();
  PT(TextureStage) make_stage(const string &name);

  INLINE int get_num_stages() const;
  INLINE TextureStage *get_stage(int n) const;

  INLINE UpdateSeq get_sort_seq() const;

  static TextureStageManager *get_global_ptr();

private:
  // This method is only called by the TextureStage destructor.
  void remove_stage(TextureStage *stage);

  // This method is called by TextureStage::set_sort().
  INLINE void inc_sort_seq();

private:
  PT(TextureStage) _default_stage;

  // With the exception of the default stage, above, we don't store
  // reference-counting pointers to the TextureStage objects within
  // this class.  Instead, we let the TextureStage objects remove
  // themselves from the TextureStageManager as they destruct.
  typedef pvector<TextureStage *> Stages;
  Stages _stages;

  typedef pset<string, TextureStage *> StagesByName;
  StagesByName _stages_by_name;

  UpdateSeq _sort_seq;

  static TextureStageManager *_global_ptr;

  friend class TextureStage;
};

#include "textureStageManager.I"

#endif
