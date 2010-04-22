// Filename: colladaRoot.h
// Created by:  rdb (21Apr10)
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

#ifndef COLLADAROOT_H
#define COLLADAROOT_H

#include "pandabase.h"

#include "config_collada.h"
#include "colladaDocument.h"
#include "colladaInstance.h"
#include "colladaLibrary.h"
#include "pnotify.h"
#include "dSearchPath.h"
#include "typedReferenceCount.h"
#include "pointerTo.h"
#include "pandaNode.h"

class ColladaDocument;

////////////////////////////////////////////////////////////////////
//       Class : ColladaRoot
// Description : Represents the root <COLLADA> element of the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaRoot : public ColladaAssetElement {
PUBLISHED:
  INLINE ColladaRoot(PT(ColladaDocument) document);
  virtual void clear();

  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement * make_xml() const;
  virtual PT(PandaNode) make_node() const;

  INLINE virtual PT(ColladaDocument) get_document() const;

private:
  PT(ColladaDocument) _document;

  Filename _filename;
  PT(ColladaInstanceVisualScene) _instance_visual_scene;
  ColladaLibraryEffects _library_effects;
  ColladaLibraryGeometries _library_geometries;
  ColladaLibraryMaterials _library_materials;
  ColladaLibraryNodes _library_nodes;
  ColladaLibraryVisualScenes _library_visual_scenes;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaRoot",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  friend class ColladaDocument;
};

#include "colladaRoot.I"

#endif

