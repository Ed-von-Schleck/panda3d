// Filename: colladaLibrary.h
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

#ifndef COLLADALIBRARY_H
#define COLLADALIBRARY_H

#include "pandabase.h"

#include "config_collada.h"
#include "colladaAssetElement.h"
#include "colladaNode.h"
#include "pvector.h"
#include "pointerTo.h"

class ColladaCamera;
class ColladaEffect;
class ColladaGeometry;
class ColladaLight;
class ColladaMaterial;
class ColladaVisualScene;

////////////////////////////////////////////////////////////////////
//       Class : ColladaLibrary
// Description : This is the template class for libraries in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
template <class T>
class EXPCL_COLLADA ColladaLibrary : public ColladaAssetElement, public pvector<PointerTo<T> > {
PUBLISHED:
  INLINE virtual void clear();
  INLINE virtual bool load_xml(const TiXmlElement *xelement);
  INLINE virtual TiXmlElement *make_xml() const;
  INLINE PT(ColladaElement) get_element_by_id(const string &id) const;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type(const string &type_name = "UndefinedColladaLibrary") {
    ColladaAssetElement::init_type();
    _type_handle = register_dynamic_type
      (type_name, ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    // In this case, we can't do anything, since we don't have the
    // class' type_name.
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

// Specialize for ColladaNode to search recursively
template<> INLINE PT(ColladaElement) ColladaLibrary<ColladaNode>::get_element_by_id(const string &id) const;

// Note: when adding new types, be sure to add them to config_collada.cxx also

EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaCamera>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaEffect>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaGeometry>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaLight>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaMaterial>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaNode>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaLibrary<ColladaVisualScene>);

typedef ColladaLibrary<ColladaCamera> ColladaLibraryCameras;
typedef ColladaLibrary<ColladaEffect> ColladaLibraryEffects;
typedef ColladaLibrary<ColladaGeometry> ColladaLibraryGeometries;
typedef ColladaLibrary<ColladaLight> ColladaLibraryLights;
typedef ColladaLibrary<ColladaMaterial> ColladaLibraryMaterials;
typedef ColladaLibrary<ColladaNode> ColladaLibraryNodes;
typedef ColladaLibrary<ColladaVisualScene> ColladaLibraryVisualScenes;

#include "colladaLibrary.I"

// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma interface
#endif

#endif
