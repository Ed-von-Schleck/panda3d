// Filename: ceguiPanda3dRenderer.cxx
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

#include "ceguiPanda3dRenderer.h"
#include "ceguiPanda3dTexture.h"
#include "ceguiPanda3dTextureTarget.h"
#include "ceguiPanda3dGeometryBuffer.h"
#include "ceguiPanda3dRenderTarget.h"

#include "nodePath.h"
#include "planeNode.h"

// Start of CEGUI namespace section
namespace CEGUI {

String Panda3DRenderer::d_rendererID(
  "CEGUI::Panda3DRenderer - Official Panda3D based 2nd generation renderer "
  "module.");

Panda3DRenderer::
Panda3DRenderer(NodePath parent,
                int win_width, int win_height) :
  d_current_target(NULL),
  d_displayDPI(96, 96) {
  d_defaultTarget = new Panda3DRenderTarget();
  d_defaultRoot = new RenderingRoot(*d_defaultTarget);
  d_displaySize.d_width = win_width;
  d_displaySize.d_height = win_height;
  d_gui_root = parent.attach_new_node("gui_root");
  d_gui_root.set_pos(-1, 0, 1);
  d_waiting_for_geom_buffer = false;
  d_current_parent_buffer = NULL;
}

Panda3DRenderer::~Panda3DRenderer() {
  destroyAllTextureTargets();
  destroyAllTextures();
  destroyAllGeometryBuffers();

  delete d_defaultRoot;
  delete d_defaultTarget;
}

RenderingRoot& Panda3DRenderer::getDefaultRenderingRoot() {
  return *d_defaultRoot;
}

GeometryBuffer& Panda3DRenderer::createGeometryBuffer() {
  Panda3DGeometryBuffer* b = new Panda3DGeometryBuffer(*this);
  d_geometryBuffers.push_back(b);
  if (d_waiting_for_geom_buffer) {
    // Should be the last one on the list.
    TextureTarget* t = d_textureTargets[d_textureTargets.size() - 1];
    d_target_buffer_map[t] = b;
    d_waiting_for_geom_buffer = false;
  }
  return *b;
}

void Panda3DRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer) {
  GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                             d_geometryBuffers.end(),
                                             &buffer);

  if (d_geometryBuffers.end() != i)
  {
    d_geometryBuffers.erase(i);
    delete &buffer;
  }
}

void Panda3DRenderer::destroyAllGeometryBuffers() {
  while (!d_geometryBuffers.empty())
    destroyGeometryBuffer(**d_geometryBuffers.begin());
}

TextureTarget* Panda3DRenderer::createTextureTarget() {
  TextureTarget* t = new Panda3DTextureTarget(*this);
  d_textureTargets.push_back(t);

  if (d_waiting_for_geom_buffer)
    std::cout << "Already waiting for buffer. Something went wrong.\n";
  d_waiting_for_geom_buffer = true;
  return t;
}

void Panda3DRenderer::destroyTextureTarget(TextureTarget* target) {
  TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                            d_textureTargets.end(),
                                            target);

  if (d_textureTargets.end() != i)
  {
    d_textureTargets.erase(i);
    delete target;
  }
}

void Panda3DRenderer::destroyAllTextureTargets() {
  while (!d_textureTargets.empty())
    destroyTextureTarget(*d_textureTargets.begin());
}

Texture& Panda3DRenderer::createTexture() {
  Panda3DTexture* tex = new Panda3DTexture();
  d_textures.push_back(tex);
  return *tex;
}

Texture& Panda3DRenderer::createTexture(const String& filename,
                                        const String& resourceGroup) {
  Panda3DTexture* tex = new Panda3DTexture(filename,
                                           resourceGroup);
  d_textures.push_back(tex);
  return *tex;
}

Texture& Panda3DRenderer::createTexture(const Size& size) {
  Panda3DTexture* tex = new Panda3DTexture(size);
  d_textures.push_back(tex);
  return *tex;
}

void Panda3DRenderer::destroyTexture(Texture& texture) {
  TextureList::iterator i = std::find(d_textures.begin(),
                                      d_textures.end(),
                                      &texture);

  if (d_textures.end() != i)
  {
    d_textures.erase(i);
    delete &static_cast<Panda3DTexture&>(texture);
  }
}

void Panda3DRenderer::destroyAllTextures() {
  while (!d_textures.empty())
    destroyTexture(**d_textures.begin());
}

void Panda3DRenderer::beginRendering() {
}

void Panda3DRenderer::endRendering() {
}

void Panda3DRenderer::setDisplaySize(const Size& sz) {
  if (sz != d_displaySize)
  {
    d_displaySize = sz;

    // FIXME: This is probably not the right thing to do in all cases.
    Rect area(d_defaultTarget->getArea());
    area.setSize(sz);
    d_defaultTarget->setArea(area);
  }

}

const Size& Panda3DRenderer::getDisplaySize() const {
  return d_displaySize;
}

const Vector2& Panda3DRenderer::getDisplayDPI() const {
  return d_displayDPI;
}

uint Panda3DRenderer::getMaxTextureSize() const {
  return 2048;
}

const String& Panda3DRenderer::getIdentifierString() const {
  return d_rendererID;
}

// Attach a new node under the current top and return it.
NodePath Panda3DRenderer::getNewGuiNode() {
  NodePath new_gui_node = d_gui_root.attach_new_node("cegui_node");
  return new_gui_node;
}

NodePath Panda3DRenderer::getGuiRoot() {
  return d_gui_root;
}

Panda3DGeometryBuffer* Panda3DRenderer::getBuffer(
  TextureTarget* textureTarget) {
  return d_target_buffer_map[textureTarget];
}

void Panda3DRenderer::onTextureTargetActivated(TextureTarget* target) {
  d_current_target = target;
  d_current_parent_buffer = d_target_buffer_map[target];
  if (d_current_parent_buffer == NULL) {
    std::cout << "Error: No buffer for this TextureTarget.\n";
    return;
  }
  // Removes the white rectangle.
  d_current_parent_buffer->wrapUpGeom();
  d_current_parent_buffer->reset();
  NodePath np = d_current_parent_buffer->getNodePath();
  // Reattach the node. The GUI node is in the 'unsorted' bin,
  // so the last attached node will be on top.
  np.detach_node();
  np.reparent_to(d_gui_root);
  d_current_target_area = target->getArea();
}

void Panda3DRenderer::onTextureTargetDeactivated(TextureTarget* target) {
  d_current_parent_buffer = NULL;
}

Panda3DGeometryBuffer* Panda3DRenderer::getParentBuffer() {
  return d_current_parent_buffer;
}

const Rect& Panda3DRenderer::getCurrentArea() {
  return d_current_target->getArea();
}

LVecBase3f Panda3DRenderer::convertSizeToRender2d(float x,
                                                  float y,
                                                  float z) {
  nassertr_always(d_displaySize.d_width, LVecBase3f());
  nassertr_always(d_displaySize.d_height, LVecBase3f());
  float x_scaled = x * 2 / d_displaySize.d_width;
  float y_scaled = y * 2 / d_displaySize.d_height;

  // In Panda3d Y means Z.
  // Flip y.
  LVecBase3f result = LVecBase3f(x_scaled, z, -y_scaled);
  return result;
}


} // namespace CEGUI
