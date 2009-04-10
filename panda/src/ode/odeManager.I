// Filename: odeManager.I
// Created by:  pro-rsoft (10Apr09)
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
//    Function : get_world
//      Access : Public
// Description : Returns the OdeWorld that is managed by this
//               OdeManager.
////////////////////////////////////////////////////////////////////
INLINE OdeWorld OdeManager::
get_world() const {
  return _world;
}


////////////////////////////////////////////////////////////////////
//    Function : get_global_ptr
//      Access : Public
// Description : Returns a pointer to the global OdeManager.
////////////////////////////////////////////////////////////////////
INLINE OdeManager *OdeManager::
get_global_ptr() {
  // We're creating the global manager here, and not at static init
  // time, because I want to avoid any static-init issues.
  if (_global_ptr == NULL) {
    _global_ptr = new OdeManager();
  }
  return _global_ptr;
}

