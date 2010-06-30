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
ColladaMaterial(PT(ColladaInstanceEffect) effect) {
  clear();
  set_instance_effect(effect);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaMaterial to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaMaterial::
clear() {
  ColladaAssetElement::clear();
  _instance_effect = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::get_instance_effect
//       Access: Public
//  Description: Returns the ColladaInstanceEffect associated with
//               this ColladaMaterial.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaInstanceEffect) ColladaMaterial::
get_instance_effect() const {
  return _instance_effect;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaMaterial::set_instance_effect
//       Access: Public
//  Description: Sets the ColladaInstanceEffect associated with this
//               ColladaMaterial.
////////////////////////////////////////////////////////////////////
INLINE void ColladaMaterial::
set_instance_effect(PT(ColladaInstanceEffect) instance_effect) {
  detach(_instance_effect);
  _instance_effect = instance_effect;
  attach(_instance_effect);
}

