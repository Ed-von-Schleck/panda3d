// Filename: ceguiPanda3dTextureTarget.cxx
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

#include "ceguiPanda3dTextureTarget.h"

#include "ceguiPanda3dGeometryBuffer.h"
#include "ceguiPanda3dRenderer.h"
#include "CEGUIRenderQueue.h"

#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI {

Panda3DTextureTarget::Panda3DTextureTarget(Panda3DRenderer& renderer) :
  d_renderer(renderer),
  Panda3DRenderTarget() {
  d_texture = &d_renderer.createTexture(d_size);
}

Panda3DTextureTarget::~Panda3DTextureTarget() { 
  d_renderer.destroyTexture(*d_texture);
}

void Panda3DTextureTarget::clear() {
  Panda3DGeometryBuffer* buffer = d_renderer.getBuffer(this);
  if (buffer)
    buffer->reset();
}

Texture& Panda3DTextureTarget::getTexture() const {
  return *d_texture;
}

void Panda3DTextureTarget::declareRenderSize(const Size& sz) {
  Panda3DGeometryBuffer* geometry = d_renderer.getBuffer(this);

  // exit if current size is enough
  if ((d_area.getWidth() >= sz.d_width) &&
      (d_area.getHeight() >=sz.d_height))
    return;

  d_size = sz;
  d_renderer.destroyTexture(*d_texture);
  d_texture = &d_renderer.createTexture(d_size);
  setArea(Rect(d_area.getPosition(), sz));
}

bool Panda3DTextureTarget::isRenderingInverted() const {
  return false;
}

void Panda3DTextureTarget::activate() {
  d_renderer.onTextureTargetActivated(this);
  Panda3DRenderTarget::activate();
}

void Panda3DTextureTarget::deactivate() {
  d_renderer.onTextureTargetDeactivated(this);
  Panda3DRenderTarget::deactivate();
}

} // namespace CEGUI
