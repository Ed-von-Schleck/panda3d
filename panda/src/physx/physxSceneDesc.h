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

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxSceneDesc
// Description : Descriptor for PhysxScene.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxSceneDesc {

PUBLISHED:
  INLINE PhysxSceneDesc();
  INLINE ~PhysxSceneDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_gravity(const LVector3f &gravity);

  LVector3f get_gravity() const;

public:
  NxSceneDesc _desc;
};

#include "physxSceneDesc.I"

#endif // PHYSXSCENEDESC_H
