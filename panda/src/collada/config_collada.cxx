// Filename: config_collada.cxx
// Created by: Xidram (21Dec10)
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

#include "config_collada.h"

#include "dconfig.h"
#include "loaderFileTypeDae.h"
#include "loaderFileTypeRegistry.h"

#include "colladaLoader.h"

ConfigureDef(config_collada);
NotifyCategoryDef(collada, "");

ConfigureFn(config_collada) {
  init_libcollada();
}

////////////////////////////////////////////////////////////////////
//     Function: init_libcollada
//  Description: Initializes the library. This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libcollada() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  ColladaLoader::init_type();
  
  LoaderFileTypeRegistry *reg = LoaderFileTypeRegistry::get_global_ptr();

  reg->register_type(new LoaderFileTypeDae);
}

