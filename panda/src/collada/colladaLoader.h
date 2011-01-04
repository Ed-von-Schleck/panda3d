// Filename: colladaLoader.h
// Created by: Xidram (21Dec10)
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

#ifndef COLLADALOADER_H
#define COLLADALOADER_H

#include "typedReferenceCount.h"
#include "pandabase.h"
#include "pandaNode.h"
#include "pvector.h"

#include "COLLADAFWAnimationList.h"
#include "COLLADAFWCamera.h"
#include "COLLADAFWEffect.h"
#include "COLLADAFWFileInfo.h"
#include "COLLADAFWImage.h"
#include "COLLADAFWIWriter.h"
#include "COLLADAFWGeometry.h"
#include "COLLADAFWLibraryNodes.h"
#include "COLLADAFWLight.h"
#include "COLLADAFWMaterial.h"
#include "COLLADAFWNode.h"
#include "COLLADAFWSkinControllerData.h"
#include "COLLADAFWVisualScene.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaLoader
// Description : Object that loads a COLLADA file.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaLoader : public TypedReferenceCount, public COLLADAFW::IWriter {
PUBLISHED:
  ColladaLoader();
  PT(PandaNode) load_collada_file(const Filename &filename);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaLoader",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  PT(ModelRoot) _root_node;
  pvector<const COLLADAFW::VisualScene*> _visual_scenes;

  virtual bool writeGeometry(const COLLADAFW::Geometry* geom);
  virtual bool writeVisualScene(const COLLADAFW::VisualScene* visualScene);
  virtual void finish();
  void write_node(COLLADAFW::Node* collada_node, PT(PandaNode) parent_node);

  virtual void cancel(const COLLADAFW::String&) {}
  virtual void start() {}
  virtual bool writeGlobalAsset(const COLLADAFW::FileInfo*) {return true;}
  virtual bool writeScene(const COLLADAFW::Scene*) {return true;}
  virtual bool writeLibraryNodes(const COLLADAFW::LibraryNodes*) {return true;}
  virtual bool writeMaterial(const COLLADAFW::Material*) {return true;}
  virtual bool writeEffect(const COLLADAFW::Effect*) {return true;}
  virtual bool writeCamera(const COLLADAFW::Camera*) {return true;}
  virtual bool writeImage(const COLLADAFW::Image*) {return true;}
  virtual bool writeLight(const COLLADAFW::Light*) {return true;}
  virtual bool writeAnimation(const COLLADAFW::Animation*) {return true;}
  virtual bool writeAnimationList(const COLLADAFW::AnimationList*) {return true;}
  virtual bool writeSkinControllerData(const COLLADAFW::SkinControllerData*) {return true;}
  virtual bool writeController(const COLLADAFW::Controller*) {return true;}
  virtual bool writeFormulas(const COLLADAFW::Formulas*) {return true;}
  virtual bool writeKinematicsScene(const COLLADAFW::KinematicsScene*) {return true;}
};

#endif
