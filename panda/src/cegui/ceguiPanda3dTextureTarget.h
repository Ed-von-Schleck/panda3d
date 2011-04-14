// Filename: ceguiPanda3dTextureTarget.h
// Created by:  Nik (Oct10)
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

#ifndef CEGUIPANDA3DTEXTURETARGET_H
#define CEGUIPANDA3DTEXTURETARGET_H

#include "ceguiPanda3dRenderTarget.h"
#include "ceguiPanda3dTexture.h"

#include "cegui_includes.h"

// Start of CEGUI namespace section
namespace CEGUI {
class Panda3DRenderer;

//! Implementation of CEGUI::TextureTarget for Panda3D
class Panda3DTextureTarget : public Panda3DRenderTarget,
public TextureTarget {
public:
  Panda3DTextureTarget(Panda3DRenderer& owner);
  ~Panda3DTextureTarget();
  // implementation of TextureTarget interface
  void clear();
  Texture& getTexture() const;
  void declareRenderSize(const Size& sz);
  bool isRenderingInverted() const;

  bool isImageryCache() {return true;}
  void activate();
  void deactivate();

protected:
  //! Panda3DRenderer that created and owns this TextureTarget.
  Panda3DRenderer& d_renderer;

  Texture* d_texture;
  //! holds defined area for the TextureTarget
  Size d_size;
};

} // namespace CEGUI

#endif  // end of guard CEGUIPANDA3DTEXTURETARGET_H
