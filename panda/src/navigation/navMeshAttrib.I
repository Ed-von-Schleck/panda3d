// Filename: navMeshAttrib.I
// Created by:  rdb (26Jul10)
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
//     Function: NavMeshAttrib::Copy Constructor
//       Access: Protected
//  Description: Use NavMeshAttrib::make() to construct a new
//               NavMeshAttrib object.
////////////////////////////////////////////////////////////////////
INLINE NavMeshAttrib::
NavMeshAttrib(const NavMeshAttrib &copy) :
  _off(copy._off),
  _area(copy._area)
{
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::is_off
//       Access: Published
//  Description: Returns true if the NavMeshAttrib specifies that
//               this geometry should not be used for any kind of
//               navigation mesh generation.
////////////////////////////////////////////////////////////////////
INLINE bool NavMeshAttrib::
is_off() const {
  return _off;
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::get_area
//       Access: Published
//  Description: Returns the area code. Zero if this mesh is
//               unwalkable. It is an error to call this
//               if is_off() returns true.
////////////////////////////////////////////////////////////////////
INLINE int NavMeshAttrib::
get_area() const {
  nassertr(!is_off(), 0);
  return _area;
}
