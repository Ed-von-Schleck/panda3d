// Filename: colladaMaterial.I
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


////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaMaterial::
ColladaMaterial() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaMaterial::
ColladaMaterial(ColladaInstanceEffect &effect) {
  clear();
  set_effect(effect);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::clear
//       Access: Public
//  Description: Resets the ColladaMaterial to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaMaterial::
clear() {
  ColladaAssetElement::clear();
  _effect.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::get_effect
//       Access: Public
//  Description: Returns the ColladaInstanceEffect associated with this
//               ColladaMaterial.
////////////////////////////////////////////////////////////////////
INLINE const ColladaInstanceEffect &ColladaMaterial::
get_effect() const {
  return _effect;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::set_effect
//       Access: Public
//  Description: Sets the ColladaInstanceEffect associated with this
//               ColladaMaterial.
////////////////////////////////////////////////////////////////////
INLINE void ColladaMaterial::
set_effect(const ColladaInstanceEffect &effect) {
  _effect = effect;
}

