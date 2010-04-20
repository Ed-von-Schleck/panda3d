// Filename: colladaData.h
// Created by:  rdb (13Apr10)
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

#ifndef COLLADADATA_H
#define COLLADADATA_H

#include "pandabase.h"

#include "colladaAsset.h"
#include "colladaEffect.h"
#include "colladaMaterial.h"
#include "colladaNode.h"
#include "colladaInstance.h"
#include "colladaLibrary.h"
#include "colladaVisualScene.h"
#include "config_collada.h"
#include "filename.h"
#include "coordinateSystem.h"
#include "pnotify.h"
#include "dSearchPath.h"
#include "typedReferenceCount.h"
#include "pointerTo.h"

class ColladaLoader;

////////////////////////////////////////////////////////////////////
//       Class : ColladaData
// Description : This is the primary interface into all the data,
//               and the root of the collada file structure.  A
//               ColladaData structure corresponds exactly with
//               a collada file on the disk.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaData : public ColladaAssetElement, public TypedReferenceCount {
PUBLISHED:
  INLINE ColladaData();

  static bool resolve_dae_filename(Filename &dae_filename,
                                   const DSearchPath &searchpath = DSearchPath());

  virtual void clear();

  bool read(Filename filename, string display_name = string());
  bool read(istream &in);

  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement * make_xml() const;
  virtual PT(PandaNode) make_node() const;

  bool write_dae(Filename filename) const;
  bool write_dae(ostream &out) const;

  INLINE CoordinateSystem get_coordinate_system() const;

  INLINE void set_filename(const Filename &filename);
  INLINE const Filename &get_filename() const;

public:
  template<class T> INLINE PT(T) resolve_instance(CPT(ColladaInstance<T>) inst) const;

private:
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
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaData",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  friend class ColladaLoader;
};

#include "colladaData.I"

#endif

