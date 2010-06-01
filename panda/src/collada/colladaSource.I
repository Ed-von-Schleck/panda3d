// Filename: colladaSource.I
// Created by:  rdb (01Jun10)
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
//     Function: ColladaSource::has_array
//       Access: Published
//  Description: Returns true if this source contains an array.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaSource::
has_array() const {
  return (_array != NULL);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::get_array
//       Access: Published
//  Description: Returns the array that this source contains, or
//               NULL if this source contains no array. Note that
//               this array is not necessarily the same as the
//               array that this source's accessor points to!
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaArrayBase) ColladaSource::
get_array() const {
  return _array;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::set_array
//       Access: Published
//  Description: Sets the array that this source contains.
////////////////////////////////////////////////////////////////////
INLINE void ColladaSource::
set_array(ColladaArrayBase* array) {
  detach(_array);
  _array = array;
  attach(_array);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::clear_array
//       Access: Published
//  Description: Unassigns the array that this source contained.
////////////////////////////////////////////////////////////////////
INLINE void ColladaSource::
clear_array() {
  detach(_array);
  _array = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::get_array_type
//       Access: Published
//  Description: Convenience method that returns the type of
//               the array that this source contains.
////////////////////////////////////////////////////////////////////
INLINE ColladaArrayType ColladaSource::
get_array_type() const {
  if (has_array()) {
    return get_array()->get_array_type();
  }
  return AT_invalid;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::get_accessor
//       Access: Published
//  Description: Returns the accessor that defines how this
//               source accesses an array.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaAccessor) ColladaSource::
get_accessor() const {
  return _accessor;
}

