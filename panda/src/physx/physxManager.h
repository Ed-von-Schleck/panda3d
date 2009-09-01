// Filename: physxManager.h
// Created by:  enn0x (01Sep09)
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

#ifndef PHYSXMANAGER_H
#define PHYSXMANAGER_H

#include "pandabase.h"
#include "pointerTo.h"
#include "luse.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxManager
// Description : TODO
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxManager {

PUBLISHED:
  PhysxManager();
  ~PhysxManager();

private:
  NxPhysicsSDK *_sdk;

};

#include "physxManager.I"

#endif // PHYSXMANAGER_H
