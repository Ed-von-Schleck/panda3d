// Filename: physxSceneDesc.h
// Created by:  enn0x (05Sep09)
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

#ifndef PHYSXSCENEDESC_H
#define PHYSXSCENEDESC_H

#include "pandabase.h"
#include "lvector3.h"

#include "physxEnums.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxBounds3;

////////////////////////////////////////////////////////////////////
//       Class : PhysxSceneDesc
// Description : Descriptor for PhysxScene.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxSceneDesc : public PhysxEnums {

PUBLISHED:
  INLINE PhysxSceneDesc();
  INLINE ~PhysxSceneDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_gravity(const LVector3f &gravity);
  void set_flag(PhysxSceneFlag flag, bool value);
  void set_max_bounds(PhysxBounds3 &bounds);

  LVector3f get_gravity() const;
  bool get_flag(PhysxSceneFlag flag) const;
  PhysxBounds3 get_max_bounds() const;

public:
  NxSceneDesc _desc;
};

#include "physxSceneDesc.I"

#endif // PHYSXSCENEDESC_H
