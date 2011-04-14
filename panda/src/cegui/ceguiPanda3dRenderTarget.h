// Filename: ceguiPanda3dRenderTarget.h
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

#ifndef CEGUIPANDA3DRENDERTARGET_H
#define CEGUIPANDA3DRENDERTARGET_H

#include "CEGUIRenderTarget.h"
#include "CEGUIRect.h"

// Start of CEGUI namespace section
namespace CEGUI {
class Panda3DRenderer;

//! Implementation of CEGUI::RenderTarget for Panda3D
class Panda3DRenderTarget : public virtual RenderTarget {
public:
  //! Constructor.
  Panda3DRenderTarget();

  // Implement RenderTarget interface
  virtual void draw(const GeometryBuffer& buffer);
  void draw(const RenderQueue& queue);
  void setArea(const Rect& area);
  const Rect& getArea() const;
  virtual bool isImageryCache() const;
  void activate();
  void deactivate();
  void unprojectPoint(const GeometryBuffer& buff,
                      const Vector2& p_in, Vector2& p_out) const;

protected:
  //! holds defined area for the RenderTarget
  Rect d_area;
};

} // namespace CEGUI

#endif  // CEGUIPANDA3DRENDERTARGET_H
