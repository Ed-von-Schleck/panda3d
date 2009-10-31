// Filename: physxBox.h
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

#ifndef PHYSXBOX_H
#define PHYSXBOX_H

#include "pandabase.h"
#include "luse.h"

#include "config_physx.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxBox
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxBox {

PUBLISHED:
  INLINE PhysxBox();
  INLINE ~PhysxBox();
  PhysxBox(const LPoint3f &center, const LVector3f &extents, const LMatrix3f &rot);

  bool is_valid() const;
  void rotate(const LMatrix4f &m, PhysxBox &obb) const;
  void set_empty();

  LPoint3f get_center() const;
  LVector3f get_extents() const;
  LMatrix3f get_rot() const;

  void set_center(LPoint3f center);
  void set_extents(LVector3f extents);
  void set_rot(LMatrix3f rot);

public:
  NxBox _box;
};

#include "physxBox.I"

#endif // PHYSBOX_H
