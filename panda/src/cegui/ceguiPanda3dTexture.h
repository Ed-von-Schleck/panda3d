// Filename: ceguiPanda3dTexture.h
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

#ifndef CEGUIPANDA3DTEXTURE_H
#define CEGUIPANDA3DTEXTURE_H

// Panda3d imports
#include "texture.h"
typedef Texture P3dTexture;

#include "CEGUITexture.h"

#include "ceguiPanda3dRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI {
//! Implementation of CEGUI::Texture interface using Panda3D.
class Panda3DTexture : public Texture {
public:
  // implement interface for Texture
  const Size& getSize() const;
  const Size& getOriginalDataSize() const;
  const Vector2& getTexelScaling() const;
  void loadFromFile(const String& filename, const String& resourceGroup);
  void loadFromMemory(const void* buffer, const Size& buffer_size,
                      PixelFormat pixel_format);
  void saveToMemory(void* buffer);

  PT(P3dTexture) getPandaTexture() {return d_texture;}
  static PT(P3dTexture) getEmptyTexture();

protected:
  // friends to allow renderer to construct and destroy texture objects
  friend Texture& Panda3DRenderer::createTexture();
  friend Texture& Panda3DRenderer::createTexture(const String&, const String&);
  friend Texture& Panda3DRenderer::createTexture(const Size&);
  friend void Panda3DRenderer::destroyTexture(Texture&);

  //! Basic constructor.
  Panda3DTexture();
  //! Construct texture from file.
  Panda3DTexture(const String& filename,
                 const String& resourceGroup);
  //! Construct texture with given size.
  Panda3DTexture(const Size& size);
  //! Destructor.
  ~Panda3DTexture();

  //! clean up the internal texture.
  void cleanupPanda3DTexture();
  //! updates cached scale value used to map pixels to texture co-ords.
  void updateCachedScaleValues();

  //! Size of the texture.
  Size d_size;
  //! original pixel of size data loaded into texture
  Size d_dataSize;
  //! cached pixel to texel mapping scale values.
  Vector2 d_texelScaling;

  PT(P3dTexture) d_texture;

  // Needed to draw rectangles of fixed color.
  static PT(P3dTexture) d_empty_texture;
};

} // namespace CEGUI

#endif  // end of guard CEGUIPANDA3DTEXTURE_H
