// Filename: effect.I
// Created by: Xidram (13Aug10)
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
//     Function: Effect::Constructor
//       Access: Published
//  Description: Constructs a new Effect that can be placed in the
//               scene graph.
////////////////////////////////////////////////////////////////////
INLINE Effect::
Effect(const string &name) : PandaNode(name) {
  set_cull_callback();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::has_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE bool Effect::
has_technique(CPT(InternalName) name) {
  int index = _techniques.find(name);
  return (index != -1);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::get_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE CPT(Technique) Effect::
get_technique(CPT(InternalName) name) const {
  int index = _techniques.find(name);
  nassertr(index != -1, NULL);
  return _techniques.get_data(index);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::set_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void Effect::
set_technique(CPT(InternalName) name, PT(Technique) technique) {
  _techniques[name] = technique;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::remove_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void Effect::
remove_technique(CPT(InternalName) name) {
  _techniques.remove(name);
}

