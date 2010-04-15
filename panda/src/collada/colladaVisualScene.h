// Filename: colladaVisualScene.h
// Created by: rdb (13Apr10)
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

#ifndef COLLADAVISUALSCENE_H
#define COLLADAVISUALSCENE_H

#include "typedReferenceCount.h"
#include "config_collada.h"
#include "colladaAssetElement.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaVisualScene
// Description : Object that represents the <visual_scene> COLLADA tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaVisualScene : public ColladaAssetElement, public TypedReferenceCount {
PUBLISHED:
  ColladaVisualScene();
  INLINE virtual void clear();

public:
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement * make_xml() const;
  PT(PandaNode) make_node() const;

  // Needed by ColladaLibrary to validate the element names
  static const string _element_name;
  static const string _library_name;

private:
  PT(ColladaAsset) _asset;
  pvector<PT(ColladaNode)> _nodes;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaVisualScene",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaVisualScene.I"

#endif
