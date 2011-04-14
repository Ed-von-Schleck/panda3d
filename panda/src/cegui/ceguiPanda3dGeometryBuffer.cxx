// Filename: ceguiPanda3dGeometryBuffer.cxx
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

#include "ceguiPanda3dGeometryBuffer.h"
#include "ceguiPanda3dTexture.h"

#include <iostream>

#include "geomNode.h"
#include "nodePathCollection.h"
#include "planeNode.h"
#include "plane.h"

// Start of CEGUI namespace section
namespace CEGUI {

PT(P3dTexture) Panda3DTexture::d_empty_texture;

Panda3DGeometryBuffer::Panda3DGeometryBuffer(Panda3DRenderer& owner) :
  d_renderer(owner),
  d_activeTexture(0),
  d_translation(0, 0, 0),
  d_rotation(0, 0, 0),
  d_pivot(0, 0, 0),
  d_effect(0),
  d_vdata(GeomVertexData("CeguiVData",
                         GeomVertexFormat::get_v3c4t2(),
                         GeomEnums::UH_static)),
  d_vertex_writer(GeomVertexWriter(&d_vdata,
                                   InternalName::get_vertex())),
  d_color_writer(GeomVertexWriter(&d_vdata,
                                  InternalName::get_color())),
  d_texcoord_writer(GeomVertexWriter(&d_vdata,
                                     InternalName::get_texcoord())),
  d_current_geom(NULL) {
  d_current_np = d_renderer.getNewGuiNode();
  d_clipped_np = d_current_np.attach_new_node("clipped");
  resetGeom();
}

Panda3DGeometryBuffer::~Panda3DGeometryBuffer() {
}

void Panda3DGeometryBuffer::draw() const {
  if (d_current_np.is_empty()) {
    return;
  }
  const_cast<Panda3DGeometryBuffer*>(this)->
    draw_impl();
}

// Non-const version of draw. Needed because set_pos and reparent are non-const.
void Panda3DGeometryBuffer::draw_impl() {
  wrapUpGeom();
  // TODO:
  // Rotations - use pivot and rotation.
  // Render effects.
  d_current_np.set_pos(d_translation);

  Panda3DGeometryBuffer* parent = d_renderer.getParentBuffer();
  if (parent && parent != this) {
    // This window is parented under another window.
    d_current_np.reparent_to(parent->getNodePath());

    if (d_clipRect == d_renderer.getCurrentArea()) {
      // No need to clip this.
      return;
    }

    LVecBase3f size = d_renderer.convertSizeToRender2d(d_clipRect.d_right,
                                                       d_clipRect.d_bottom,
                                                       0);
    LVecBase3f offset = d_renderer.convertSizeToRender2d(
      d_clipRect.d_left, d_clipRect.d_top, 0);

    // Different children get different clipping rectangles.
    // Clipping is in the parent's coordinates. So, every child
    // gets a different parent that is clipped with this child's
    // clipping.
    NodePath clipped = parent->makeClippingNodePath();
    // Reparent to set Z-order.
    clipped.reparent_to(parent->getNodePath());
    clipped.set_scissor(offset, size);
    d_current_np.reparent_to(clipped);
  } else {
    // This is the mouse cursor.
    // Reparent to fix z-order.
    NodePath parent = d_current_np.get_parent();
    d_current_np.detach_node();
    d_current_np.reparent_to(parent);
  }
}

void Panda3DGeometryBuffer::setTranslation(const Vector3& v) {
  d_translation = d_renderer.convertSizeToRender2d(v.d_x, v.d_y, v.d_z);
}

void Panda3DGeometryBuffer::setRotation(const Vector3& r) {
  d_rotation = r;
}

void Panda3DGeometryBuffer::setPivot(const Vector3& p) {
  // TODO
  d_pivot = p;
}

void Panda3DGeometryBuffer::setClippingRegion(const Rect& region) {
  d_clipRect = region;
  // TODO: convert and store converted values.
}

void Panda3DGeometryBuffer::appendVertex(const Vertex& vertex) {
  appendGeometry(&vertex, 1);
}

void Panda3DGeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                           uint vertex_count) {
  for (uint i = 0; i < vertex_count; i++) {
    const Vertex& v = vbuff[i];

    LVecBase3f pos = d_renderer.convertSizeToRender2d(v.position.d_x,
                                                      v.position.d_y,
                                                      v.position.d_z);
    d_vertex_writer.add_data3f(pos);
    d_color_writer.add_data4f(v.colour_val.getRed(),
                              v.colour_val.getGreen(),
                              v.colour_val.getBlue(),
                              v.colour_val.getAlpha()
      );
    if (!d_activeTexture) {
      d_texcoord_writer.add_data2f(0, 0);
    }
    else {
      d_texcoord_writer.add_data2f(
        convertToUV(v.tex_coords.d_x,
                    v.tex_coords.d_y,
                    d_activeTexture->getSize()));
    }
  }
  PT(GeomTristrips) tris = new GeomTristrips(GeomEnums::UH_static);
  int start_index = d_vertex_writer.get_write_row() - vertex_count;
  tris->add_consecutive_vertices(start_index, vertex_count);
  tris->close_primitive();
  d_current_geom->add_primitive(tris);
}

void Panda3DGeometryBuffer::wrapUpGeom() {
  if (!d_current_geom || !d_current_geom->get_num_primitives())
    return;
  PT(GeomNode) gnode = new GeomNode("ceguiGeomNode");
  gnode->add_geom(d_current_geom);
  resetGeom();

  NodePath result = d_current_np.attach_new_node(gnode);

  if (!d_activeTexture || !d_activeTexture->getPandaTexture())
    result.set_texture(Panda3DTexture::getEmptyTexture());
  else
    result.set_texture(d_activeTexture->getPandaTexture());
  result.set_bin("transparent", 30);
  result.set_transparency(TransparencyAttrib::M_alpha);
}

void Panda3DGeometryBuffer::resetGeom() {
  d_current_geom = new Geom(&d_vdata);
}

// Convert pixel coordinates into the Panda3d texture UV coordinates
// (0,0 is bottom left, 1,1 top right)
LVecBase2f Panda3DGeometryBuffer::convertToUV(float x_pix, float y_pix,
                                              const Size& texture_size) {
  float x = x_pix / texture_size.d_width;
  float y = y_pix / texture_size.d_height;
  return LVecBase2f(x, y);
}

void Panda3DGeometryBuffer::setActiveTexture(Texture* texture) {
  if (texture != d_activeTexture) {
    wrapUpGeom();
  }
  Panda3DTexture* tex = dynamic_cast<Panda3DTexture*>(texture);
  if (tex && tex->getPandaTexture()) {
    d_activeTexture = tex;
  }
}

void Panda3DGeometryBuffer::reset() {
  d_activeTexture = 0;
  d_current_np.get_children().detach();
}

Texture* Panda3DGeometryBuffer::getActiveTexture() const {
  return d_activeTexture;
}

uint Panda3DGeometryBuffer::getVertexCount() const {
  // Never called.
  std::cout << "getVertexCount not implemented.\n";
  return -1;  //TODO
}

uint Panda3DGeometryBuffer::getBatchCount() const {
  // Never called.
  std::cout << "getBatchCount not implemented.\n";
  return -1; //TODO
}

void Panda3DGeometryBuffer::setRenderEffect(RenderEffect* effect) {
  // TODO
  d_effect = effect;
}

RenderEffect* Panda3DGeometryBuffer::getRenderEffect() {
  return d_effect;
}

NodePath Panda3DGeometryBuffer::getNodePath() {
  return d_current_np;
}

NodePath Panda3DGeometryBuffer::makeClippingNodePath() {
  return d_current_np.attach_new_node("clipped");
}

} // namespace CEGUI
