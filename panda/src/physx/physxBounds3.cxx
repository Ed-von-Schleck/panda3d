// Filename: physxBounds3.cxx
// Created by:  enn0x (31Oct09)
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

#include "physxBounds3.h"
#include "physxManager.h"

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::get_max
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LPoint3f PhysxBounds3::
get_max() const {

  return PhysxManager::nxVec3_to_point3(_bounds.max);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::get_min
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LPoint3f PhysxBounds3::
get_min() const {

  return PhysxManager::nxVec3_to_point3(_bounds.min);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set_max
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set_max(LPoint3f value) {

  _bounds.max = PhysxManager::point3_to_nxVec3(value);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set_min
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set_min(LPoint3f value) {

  _bounds.min = PhysxManager::point3_to_nxVec3(value);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::bounds_of_obb
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
bounds_of_obb(const LMatrix3f &orientation, const LPoint3f &translation, const LVector3f &half_dims) {

  _bounds.boundsOfOBB(PhysxManager::mat3_to_nxMat33(orientation),
                      PhysxManager::point3_to_nxVec3(translation),
                      PhysxManager::vec3_to_nxVec3(half_dims));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::combine
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
combine(const PhysxBounds3 &b2) {

  _bounds.combine(b2._bounds);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::contain
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxBounds3::
contain(const LPoint3f &p) const {

  return _bounds.contain(PhysxManager::point3_to_nxVec3(p));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::fatten
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
fatten(float distance) {

  _bounds.fatten(distance);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::get_dimensions
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
LVector3f PhysxBounds3::
get_dimensions() const {

  NxVec3 dims;
  _bounds.getDimensions(dims);

  return PhysxManager::nxVec3_to_vec3(dims);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::include
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
include(const LPoint3f &p) {

  _bounds.include(PhysxManager::point3_to_nxVec3(p));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::intersects
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxBounds3::
intersects(const PhysxBounds3 &b) const {

  return _bounds.intersects(b._bounds);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::intersects2d
//       Access : Published
//  Description : Indicates whether the intersection of this and b
//                is empty or not in the plane orthogonal to the
//                axis passed (X = 0, Y = 1 or Z = 2). 
////////////////////////////////////////////////////////////////////
bool PhysxBounds3::
intersects2d(const PhysxBounds3 &b, unsigned axis_to_ignore) const {

  return _bounds.intersects2D(b._bounds, axis_to_ignore);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::is_empty
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxBounds3::
is_empty() const {

  return _bounds.isEmpty();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::scale
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
scale(float scale) {

  _bounds.scale(scale);
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set(const LPoint3f &min, const LPoint3f &max) {

  _bounds.set(PhysxManager::point3_to_nxVec3(min),
              PhysxManager::point3_to_nxVec3(max));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set_center_extents
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set_center_extents(const LPoint3f &center, const LVector3f &extents) {

  _bounds.setCenterExtents(PhysxManager::point3_to_nxVec3(center),
                           PhysxManager::vec3_to_nxVec3(extents));
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set_empty
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set_empty() {

  _bounds.setEmpty();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::set_infinite
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
set_infinite() {

  _bounds.setInfinite();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxBounds3::transform
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxBounds3::
transform(const LMatrix3f &orientation, const LPoint3f &translation) {

  _bounds.transform(PhysxManager::mat3_to_nxMat33(orientation),
                    PhysxManager::point3_to_nxVec3(translation));
}

