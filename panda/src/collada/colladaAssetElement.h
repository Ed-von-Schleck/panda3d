// Filename: colladaAssetElement.h
// Created by: Xidram (15Apr10)
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

#ifndef COLLADAASSETELEMENT_H
#define COLLADAASSETELEMENT_H

#include "colladaAsset.h"
#include "colladaElement.h"
#include "pointerTo.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaAssetElement
// Description : Element that can have an <asset> tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaAssetElement : public ColladaElement {
PUBLISHED:
  INLINE PT(ColladaAsset) get_asset() const;
  INLINE void set_asset(PT(ColladaAsset) asset);

protected:
  PT(ColladaAsset) _asset;
};

#include "colladaAssetElement.I"

#endif

