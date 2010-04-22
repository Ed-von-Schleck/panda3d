// Filename: colladaLight.h
// Created by: Xidram (20Apr10)
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

#ifndef COLLADALIGHT_H
#define COLLADALIGHT_H

#include "colladaAssetElement.h"
#include "typedReferenceCount.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotlight.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaLight
// Description : Object that represents the <light> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaLight : public ColladaAssetElement {
PUBLISHED:
  ColladaLight();
  INLINE virtual void clear();

  enum LightType {
    LT_invalid,
    LT_ambient,
    LT_directional,
    LT_point,
    LT_spot
  };

public:
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;
  virtual PT(PandaNode) make_node() const;

  // Needed by ColladaLibrary to validate the element names.
  static const string _element_name;
  static const string _library_name;

  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaLight",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  LightType _light_type;
  static TypeHandle _type_handle;
};

#include "colladaLight.I"

#endif
