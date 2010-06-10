// Filename: colladaInput.I
// Created by:  rdb (02Jun10)
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
//     Function: ColladaInput::get_offset
//       Access: Published
//  Description: Returns the offset into the list of indices that
//               the parent ColladaPrimitive stores.
////////////////////////////////////////////////////////////////////
INLINE int ColladaInput::
get_offset() const {
  return _offset;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::get_semantic
//       Access: Published
//  Description: Returns the semantic string assigned to this input.
//               This defines the meaning of the input connection.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaInput::
get_semantic() const {
  return _semantic;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInput::get_set
//       Access: Published
//  Description: Returns an integer value that defines which inputs
//               to group as a single set.
////////////////////////////////////////////////////////////////////
INLINE int ColladaInput::
get_set() const {
  return _set;
}
