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
// Description : The central interface to the PhysX subsystem.
//               Used e. g. for setting/retrieving global parameters
//               or for creating scenes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxManager {

PUBLISHED:
  PhysxManager();
  ~PhysxManager();

private:
  NxPhysicsSDK *_sdk;

  class PhysxOutputStream : public NxUserOutputStream {
    void reportError(NxErrorCode code, const char *message, const char *file, int line);
    NxAssertResponse reportAssertViolation(const char *message, const char *file, int line);
    void print(const char *message);
    const char *get_error_code_string(NxErrorCode code);
  };
  static PhysxOutputStream _outputStream;

  static const char *get_sdk_error_string(const NxSDKCreateError &error);
};

#include "physxManager.I"

#endif // PHYSXMANAGER_H
