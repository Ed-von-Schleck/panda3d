// Filename: cgEffect.cxx
// Created by: Xidram (15Aug10)
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

#include "cgEffect.h"
#include "pointerTo.h"
#include "renderPass.h"
#include "technique.h"
#include "virtualFileSystem.h"

////////////////////////////////////////////////////////////////////
//     Function: CgEffect::read
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool CgEffect::
read(const Filename &fullpath) {
  string body;
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vfs->read_file(fullpath, body, true)) {
    cg_cat.error() << "Could not read CgFX file: " << fullpath << "\n";
    return NULL;
  }

  CGcontext cg_context = cgCreateContext();
  CGeffect cg_effect = cgCreateEffect(cg_context, body.c_str(), NULL);

  CGtechnique cg_technique = cgGetFirstTechnique(cg_effect);
  while (cg_technique) {
    PT(Technique) technique = new Technique();
    set_technique(InternalName::make(cgGetTechniqueName(cg_technique)), technique);

    CGpass cg_pass = cgGetFirstPass(cg_technique);
    while (cg_pass) {
      PT(RenderPass) pass = new RenderPass(cgGetPassName(cg_pass));
      technique->add_pass(pass);
      cg_pass = cgGetNextPass(cg_pass);
    }

    cg_technique = cgGetNextTechnique(cg_technique);
  }
}

