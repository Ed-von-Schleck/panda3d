// Filename: colladaAssetElement.I
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


////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::clear
//       Access: Public
//  Description: Resets the ColladaAssetElement to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAssetElement::
clear() {
  ColladaElement::clear();
  _asset = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::get_asset
//       Access: Public
//  Description: Returns the pointer to the <asset> (ColladaAsset)
//               of this element.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaAsset) ColladaAssetElement::
get_asset() const {
  return _asset;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::set_asset
//       Access: Public
//  Description: Sets the value of the <asset> of this element.
//               Returns true if successful.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAssetElement::
set_asset(PT(ColladaAsset) asset) {
  detach(_asset);
  _asset = asset;
  attach(_asset);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAssetElement::get_element_by_id
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaElement) ColladaAssetElement::
get_element_by_id(const string &id) const {
  if (_asset) {
    if (_asset->get_id() == id) {
      return DCAST(ColladaElement, _asset);
    }
    return _asset->get_element_by_id(id);
  }
  return NULL;
}
