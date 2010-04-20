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
#include "colladaAsset.h"
#include "colladaAssetElement.h"
#include "pvector.h"
#include "pointerTo.h"

class ColladaEffect;
class ColladaGeometry;
class ColladaNode;
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
  INLINE PT(T) get_element_by_id(const string &id) const;
};

typedef ColladaLibrary<ColladaEffect> ColladaLibraryEffects;
typedef ColladaLibrary<ColladaGeometry> ColladaLibraryGeometries;
typedef ColladaLibrary<ColladaMaterial> ColladaLibraryMaterials;
typedef ColladaLibrary<ColladaNode> ColladaLibraryNodes;
typedef ColladaLibrary<ColladaVisualScene> ColladaLibraryVisualScenes;

#include "colladaLibrary.I"

#endif
