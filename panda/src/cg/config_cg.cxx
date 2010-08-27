// Filename: config_cg.cxx
// Created by: Xidram (15Aug10)
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

#include "config_cg.h"
#include "loaderFileTypeCgFX.h"

#include <Cg/cg.h>
#include "effectPool.h"
#include "loaderFileTypeRegistry.h"
#include "virtualFileSystem.h"
#include "dconfig.h"

ConfigureDef(config_cg);
NotifyCategoryDef(cg, "");


ConfigureFn(config_cg) {
  init_libcg();
}

////////////////////////////////////////////////////////////////////
//     Function: cg_error_handler
//  Description: Will be called by Cg whenever an error occurs.
////////////////////////////////////////////////////////////////////
void
cg_error_handler(CGcontext context, CGerror error, void* data) {
  const char* error_string = cgGetErrorString(error);
  if (strncmp(error_string, "CG ERROR : ", 11) == 0) {
    cg_cat.error() << (const char*) (error_string + 11) << "\n";
  } else {
    cg_cat.error() << error_string << "\n";
  }

  if (error == CG_COMPILER_ERROR) {
    cg_cat.error(false) << cgGetLastListing(context) << "\n";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: init_libcg
//  Description: Initializes the library. This must be called at
//               least once before any of the functions or classes in
//               this library can be used. Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libcg() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  cgSetErrorHandler(&cg_error_handler, NULL);
  if (Thread::is_threading_supported()) {
    // Less overhead in case we're not using threading
    cgSetLockingPolicy(CG_NO_LOCKS_POLICY);
  }

  LoaderFileTypeCgFX::init_type();

  LoaderFileTypeRegistry *reg = LoaderFileTypeRegistry::get_global_ptr();
  reg->register_type(new LoaderFileTypeCgFX);
}

