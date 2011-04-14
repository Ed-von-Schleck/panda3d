// Filename: ceguiPanda3dRenderTarget.cxx
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

#include "ceguiPanda3dRenderTarget.h"
#include "ceguiPanda3dRenderer.h"
#include "ceguiPanda3dGeometryBuffer.h"
#include "CEGUIRenderQueue.h"

#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI {
Panda3DRenderTarget::Panda3DRenderTarget() :
  d_area(0, 0, 0, 0) {
}

void Panda3DRenderTarget::draw(const GeometryBuffer& buffer) {
  buffer.draw();
}

void Panda3DRenderTarget::draw(const RenderQueue& queue) {
  queue.draw();
}

void Panda3DRenderTarget::setArea(const Rect& area) {
  d_area = area;
}

const Rect& Panda3DRenderTarget::getArea() const {
  return d_area;
}

bool Panda3DRenderTarget::isImageryCache() const {
  return false;
}

void Panda3DRenderTarget::activate() {
}

void Panda3DRenderTarget::deactivate() {
}

void Panda3DRenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                         const Vector2& p_in, Vector2& p_out) const {
  // Never called.
  std::cout << "unprojectPoint not implemented\n";
  p_out = p_in;
}

} // namespace CEGUI
