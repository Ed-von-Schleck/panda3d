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
//  Description: Create default TexCoordName, Texture Stage and an
//               initial sort sequence
////////////////////////////////////////////////////////////////////
TextureStageManager::
TextureStageManager() {
  // It's important to define the _default_texcoord pointer before the
  // _default_stage pointer, because the TextureStage constructor
  // wants to get the _default_texcoord value.
  _default_texcoord = make_texcoord("default");
  _default_stage = make_stage("default");
  _sort_seq = UpdateSeq::initial();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::Destructor
//       Access: Private
//  Description: Does nothing currently
////////////////////////////////////////////////////////////////////
TextureStageManager::
~TextureStageManager() {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::make_stage
//       Access: Published
//  Description: Function that creates a TextuerStage and fills in 
//               the map of StagesByName
////////////////////////////////////////////////////////////////////
PT(TextureStage) TextureStageManager::
make_stage (const string &name) {
  // First look at the __stages_by_name, if found return the
  // TexCoordName, else insert into the map and _texcoords
  StagesByName::iterator it = _stages_by_name.find(name);
  
  //found, so return pointer
  if (it != _stages_by_name.end())
    return (*it).second;

  // not found, add
  PT(TextureStage) stage = new TextureStage(name, _default_texcoord);
  _stages_by_name[name] = stage;
  _stages.push_back(stage);
  return stage;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::make_texcoord
//       Access: Published
//  Description: Function that creates a TexCoordName and fills in 
//               the map of TexCoordsByName
////////////////////////////////////////////////////////////////////
PT(TexCoordName) TextureStageManager::
make_texcoord (const string &name) {
  // First look at the _texcoords_by_name, if found return the
  // TexCoordName, else insert into the map and _texcoords
  TexCoordsByName::iterator it = _texcoords_by_name.find(name);
  
  //found, so return pointer
  if (it != _texcoords_by_name.end())
    return (*it).second;

  // not found, add
  PT(TexCoordName) texcoord = new TexCoordName(name);
  _texcoords_by_name[name] = texcoord;
  _texcoords.push_back(texcoord);
  return texcoord;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::write
//       Access: Published
//  Description: Function that writes members of this class to out
////////////////////////////////////////////////////////////////////
void TextureStageManager::
write (ostream &out) const {
  _default_stage->write(out);
  _default_texcoord->output(out);

  out << "Number of TextureStages = " << _stages.size() << ", sort seq = " << get_sort_seq() << "\n";
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::remove_stage
//       Access: Private
//  Description: This method is only called by the TextureStage
//               destructor.
////////////////////////////////////////////////////////////////////
void TextureStageManager::
remove_stage(TextureStage *stage) {
  // by now TextureStageManager has determined that all reference count
  // to this stage has been eliminated. Hence we can erase it
  
  StagesByName::iterator it = _stages_by_name.find(stage->get_name());
  if (it != _stages_by_name.end()) {
    _stages_by_name.erase(it);
  }
  else {
    // The stage was not in the texture stage manager to begin with
    nassertv(false);
  }

  Stages::iterator si = find(_stages.begin(), _stages.end(), stage);
  nassertv(si != _stages.end());
  _stages.erase(si);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStageManager::remove_texcoord
//       Access: Private
//  Description: This method is only called by the TexCoordName
//               destructor.
////////////////////////////////////////////////////////////////////
void TextureStageManager::
remove_texcoord(TexCoordName *texcoord) {
  // by now TextureStageManager has determined that all reference count
  // to this texcoord has been eliminated. Hence we can erase it

  TexCoordsByName::iterator it = _texcoords_by_name.find(texcoord->get_name());
  if (it != _texcoords_by_name.end()) {
    _texcoords_by_name.erase(it);
  }
  else {
    // The texcoord was not in the texture stage manager to begin with
    nassertv(false);
  }

  TexCoords::iterator ti = find(_texcoords.begin(), _texcoords.end(), texcoord);
  nassertv(ti != _texcoords.end());
  _texcoords.erase(ti);
}

