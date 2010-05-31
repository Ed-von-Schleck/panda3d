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
  INLINE virtual void clear();

  INLINE PT(ColladaAsset) get_asset() const;
  INLINE void set_asset(PT(ColladaAsset) asset);

  virtual bool load_xml(const TiXmlElement *element);
  virtual TiXmlElement *make_xml() const;

  INLINE virtual PT(ColladaElement) get_element_by_id(const string &id) const;

protected:
  PT(ColladaAsset) _asset;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaAssetElement",
                  ColladaElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaAssetElement.I"

#endif

