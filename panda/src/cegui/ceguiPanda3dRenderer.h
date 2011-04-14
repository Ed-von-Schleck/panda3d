// Filename: ceguiPanda3dRenderer.h
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

#ifndef CEGUIPANDA3DRENDERER_H
#define CEGUIPANDA3DRENDERER_H

#include "CEGUIRect.h"
#include "CEGUIRenderer.h"
#include "CEGUISize.h"
#include "CEGUIVector.h"
#include <vector>

#include "nodePath.h"


class NodePath;

namespace CEGUI {
class Panda3DTexture;
class Panda3DGeometryBuffer;

class Panda3DRenderer : public Renderer {
public:
  Panda3DRenderer(NodePath parent, int win_width, int win_height);
  // Implementation of Renderer interface.
  RenderingRoot& getDefaultRenderingRoot();
  GeometryBuffer& createGeometryBuffer();
  void destroyGeometryBuffer(const GeometryBuffer& buffer);
  void destroyAllGeometryBuffers();
  TextureTarget* createTextureTarget();
  void destroyTextureTarget(TextureTarget* target);
  void destroyAllTextureTargets();
  Texture& createTexture();
  Texture& createTexture(const String& filename, const String& resourceGroup);
  Texture& createTexture(const Size& size);
  void destroyTexture(Texture& texture);
  void destroyAllTextures();
  void beginRendering();
  void endRendering();
  void setDisplaySize(const Size& sz);
  const Size& getDisplaySize() const;
  const Vector2& getDisplayDPI() const;
  uint getMaxTextureSize() const;
  const String& getIdentifierString() const;

  NodePath getNewGuiNode();
  NodePath getGuiRoot();
  Panda3DGeometryBuffer* getBuffer(TextureTarget* textureTarget);
  void onTextureTargetActivated(TextureTarget* target);
  void onTextureTargetDeactivated(TextureTarget* target);
  Panda3DGeometryBuffer* getParentBuffer();
  const Rect& getCurrentArea();
  LVecBase3f convertSizeToRender2d(float x_pix, float y_pix, float z);

protected:
  //! Constructor.
  Panda3DRenderer();
  //! Destructor.
  ~Panda3DRenderer();

  // Usually, render2d.
  NodePath d_gui_root;
    
  //! String holding the renderer identification text.
  static String d_rendererID;
  //! What the renderer considers to be the current display size.
  Size d_displaySize;
  //! What the renderer considers to be the current display DPI resolution.
  Vector2 d_displayDPI;
  //! The default RenderTarget (used by d_defaultRoot)
  RenderTarget* d_defaultTarget;
  //! The default rendering root object
  RenderingRoot* d_defaultRoot;
  //! container type used to hold TextureTargets we create.
  typedef std::vector<TextureTarget*> TextureTargetList;
  //! Container used to track texture targets.
  TextureTargetList d_textureTargets;
  //! container type used to hold GeometryBuffers we create.
  typedef std::vector<Panda3DGeometryBuffer*> GeometryBufferList;
  //! Container used to track geometry buffers.
  GeometryBufferList d_geometryBuffers;
  //! container type used to hold Textures we create.
  typedef std::vector<Panda3DTexture*> TextureList;
  //! Container used to track textures.
  TextureList d_textures;
  // True if we just created a TextureTarget, and are expecting
  // a request for a GeometryBuffer. The buffer will be linked
  // to the TextureTarget. See d_target_buffer_map.
  bool d_waiting_for_geom_buffer;
  typedef std::map<TextureTarget*, Panda3DGeometryBuffer*> BufferMap;
  // Maps a TextureTarget to a GeometryBuffer (Hack alert).
  BufferMap d_target_buffer_map;
  // When a TextureTarget is activated, we set this to the buffer that
  // backs this target.
  Panda3DGeometryBuffer* d_current_parent_buffer;
  // This has the area of the currently active buffer.
  TextureTarget* d_current_target;
  Rect d_current_target_area;
};

} // namespace CEGUI

#endif  // CEGUIPANDA3DRENDERER_H
