// Filename: ceguiPanda3dGeometryBuffer.h
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

#ifndef CEGUIPANDA3DGEOMETRYBUFFER_H
#define CEGUIPANDA3DGEOMETRYBUFFER_H

#include "nodePath.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geomVertexWriter.h"
#include "geomTristrips.h"

#include "cegui_includes.h"

namespace CEGUI {

class Panda3DRenderer;
class Panda3DTexture;

// Implemetation of CEGUI::GeometryBuffer for Panda3D.
class Panda3DGeometryBuffer : public GeometryBuffer {
public:
  // Constructor.
  Panda3DGeometryBuffer(Panda3DRenderer& owner);
  // Destructor
  ~Panda3DGeometryBuffer();

  // Implement GeometryBuffer interface.
  void draw() const;
  void setTranslation(const Vector3& v);
  void setRotation(const Vector3& r);
  void setPivot(const Vector3& p);
  void setClippingRegion(const Rect& region);
  void appendVertex(const Vertex& vertex);
  void appendGeometry(const Vertex* const vbuff, uint vertex_count);
  void setActiveTexture(Texture* texture);
  void reset();
  Texture* getActiveTexture() const;
  uint getVertexCount() const;
  uint getBatchCount() const;
  void setRenderEffect(RenderEffect* effect);
  RenderEffect* getRenderEffect();
  NodePath getNodePath();
  NodePath makeClippingNodePath();
  void wrapUpGeom();

protected:
  void draw_impl();
  static LVecBase2f convertToUV(float x_pix, float y_pix,
                                const Size& texture_size);

  void resetGeom();

  // Panda3DRenderer object that created and owns this GeometryBuffer.
  Panda3DRenderer& d_renderer;
  // last texture that was set as active
  Panda3DTexture* d_activeTexture;
  // rectangular clip region
  Rect d_clipRect;
  // translation vector
  LVecBase3f d_translation;
  // rotation vector
  Vector3 d_rotation;
  // pivot point for rotation
  Vector3 d_pivot;
  // RenderEffect that will be used by the GeometryBuffer
  RenderEffect* d_effect;

  // All batches are parented under the top NodePath.
  // Translations (and rotations?) are applied to this node.
  //NodePath d_top_np;
  NodePath d_current_np;
  // All clipping nodes go under here.
  NodePath d_clipped_np;

  // Machinery to create geometry.
  // Order here affects the order of initialization.
  GeomVertexData d_vdata;
  GeomVertexWriter d_vertex_writer;
  GeomVertexWriter d_color_writer;
  GeomVertexWriter d_texcoord_writer;

  // Current geom that takes primitives.
  PT(Geom) d_current_geom;
};

} // namespace CEGUI

#endif  // CEGUIPANDA3DGEOMETRYBUFFER_H
