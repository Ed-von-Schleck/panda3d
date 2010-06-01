// Filename: colladaAccessor.I
// Created by: rdb (31May10)
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
//     Function: ColladaAccessor::get_count
//       Access: Published
//  Description: Returns the number of times the array is visited.
////////////////////////////////////////////////////////////////////
INLINE int ColladaAccessor::
get_count() const {
  return _count;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::get_offset
//       Access: Published
//  Description: Returns the offset; this value indicates where
//               to start reading in the array.
////////////////////////////////////////////////////////////////////
INLINE int ColladaAccessor::
get_offset() const {
  return _offset;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAccessor::get_stride
//       Access: Published
//  Description: Returns the stride used when accessing the array.
////////////////////////////////////////////////////////////////////
INLINE int ColladaAccessor::
get_stride() const {
  return _stride;
}

