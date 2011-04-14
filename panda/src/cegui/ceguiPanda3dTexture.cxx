// Filename: ceguiPanda3dTexture.cxx
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

#include "ceguiPanda3dTexture.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI {

Panda3DTexture::Panda3DTexture() :
  d_size(0, 0),
  d_dataSize(0, 0),
  d_texelScaling(1, 1) {
}

Panda3DTexture::Panda3DTexture(const String& filename,
                               const String& resourceGroup) :
  d_size(0, 0),
  d_dataSize(0, 0),
  d_texelScaling(1, 1) {
  loadFromFile(filename, resourceGroup);
}

Panda3DTexture::Panda3DTexture(const Size& size) :
  d_size(size),
  d_dataSize(size),
  d_texelScaling(1, 1) {
}

Panda3DTexture::~Panda3DTexture() {
  cleanupPanda3DTexture();
}

// static
PT(P3dTexture) Panda3DTexture::getEmptyTexture() {
  if (!d_empty_texture) {
    d_empty_texture = new P3dTexture("empty_texture");
    d_empty_texture->setup_2d_texture(1, 1, // Dimensions
                                      P3dTexture::T_unsigned_byte,
                                      P3dTexture::F_rgba8);
    d_empty_texture->clear();
  }
  return d_empty_texture;
}

const Size& Panda3DTexture::getSize() const {
  return d_size;
}

const Size& Panda3DTexture::getOriginalDataSize() const {
  return d_dataSize;
}

const Vector2& Panda3DTexture::getTexelScaling() const {
//    return Vector2(0, 0);
  return d_texelScaling;
}

void Panda3DTexture::loadFromFile(const String& filename,
                                  const String& resourceGroup) {
  // get and check existence of CEGUI::System object
  System* sys = System::getSingletonPtr();
  if (!sys)
    throw RendererException("Panda3DTexture::loadFromFile - "
                            "CEGUI::System object has not been created!");

  // load file to memory via resource provider
  RawDataContainer texFile;
  sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
                                                   resourceGroup);

  // This calls loadFromMemory
  Texture* res = sys->getImageCodec().load(texFile, this);

  // unload file data buffer
  sys->getResourceProvider()->unloadRawDataContainer(texFile);

  if (!res)
    // It's an error
    throw RendererException("Panda3DTexture::loadFromFile - " +
                            sys->getImageCodec().getIdentifierString()+
                            " failed to load image '" + filename + "'.");
}

void Panda3DTexture::loadFromMemory(const void* buffer,
                                    const Size& size,
                                    PixelFormat pixel_format) {
  d_size = size;
  d_dataSize = size;
  uint pixel_size = 0;
  P3dTexture::Format format;
  string swap_format;
  if (pixel_format == PF_RGB)
  {
    pixel_size = 3;
    format = P3dTexture::F_rgb8;
    swap_format = "RGB";
  } else if (pixel_format ==  PF_RGBA) {
    pixel_size = 4;
    format = P3dTexture::F_rgba8;
    swap_format = "RGBA";
  } else {
    std::cout << "Unrecognized image format.\n";
  }

  uint byteSize = size.d_width * size.d_height * pixel_size * sizeof(char);

  // Panda3d's byte array
  PTA_uchar array = PTA_uchar::empty_array(byteSize);
  // Fake what PTA.set_subdata is doing.
  memcpy(array.p(), buffer, byteSize);
  d_texture = new P3dTexture("cegui_texture");
  d_texture->setup_2d_texture(size.d_width, size.d_height,
                              P3dTexture::T_unsigned_byte,
                              format);
  d_texture->set_ram_image(array);
  // Swap red and blue channels.
  d_texture->set_ram_image(d_texture->get_ram_image_as(swap_format));
  d_texture->set_keep_ram_image(true);
  if (size.d_width == 64) {
    // DEBUG write
    d_texture->write("./tex.tiff");
  }
}

void Panda3DTexture::saveToMemory(void* buffer) {
  // TODO:
}

void Panda3DTexture::cleanupPanda3DTexture() {
}
void Panda3DTexture::updateCachedScaleValues() {
  //
  // calculate what to use for x scale
  //
  const float orgW = d_dataSize.d_width;
  const float texW = d_size.d_width;

  // if texture and original data width are the same, scale is based
  // on the original size.
  // if texture is wider (and source data was not stretched), scale
  // is based on the size of the resulting texture.
  d_texelScaling.d_x = 1.0f / ((orgW == texW) ? orgW : texW);

  //
  // calculate what to use for y scale
  //
  const float orgH = d_dataSize.d_height;
  const float texH = d_size.d_height;

  // if texture and original data height are the same, scale is based
  // on the original size.
  // if texture is taller (and source data was not stretched), scale
  // is based on the size of the resulting texture.
  d_texelScaling.d_y = 1.0f / ((orgH == texH) ? orgH : texH);
}

} // namespace CEGUI
