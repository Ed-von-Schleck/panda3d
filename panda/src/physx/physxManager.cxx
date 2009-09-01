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

#include "physxManager.h"

////////////////////////////////////////////////////////////////////
//     Function : PhysxManager
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
PhysxManager::
PhysxManager() {

  NxSDKCreateError error;
  NxPhysicsSDKDesc desc = NxPhysicsSDKDesc();

  _sdk = NxCreatePhysicsSDK( NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &error );

  cout << "----> " << _sdk << "\n"; // TODO
}

////////////////////////////////////////////////////////////////////
//     Function : ~PhysxManager
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
PhysxManager::
~PhysxManager() {

  NxReleasePhysicsSDK( _sdk );
}

