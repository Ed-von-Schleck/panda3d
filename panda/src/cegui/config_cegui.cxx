// Filename: config_cegui.cxx
// Created by:  Nik (Oct10)
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

#include "config_cegui.h"
#include "dconfig.h"
#include "ceguiInputHandler.h"

Configure(config_cegui);
//NotifyCategoryDef(cegui, "");

ConfigureFn(config_cegui) {
  init_libcegui();
}

void
init_libcegui() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;
  CeguiInputHandler::init_type();
}
