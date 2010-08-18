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
#include "cgEffect.h"

#include "effectPool.h"

#include "dconfig.h"

ConfigureDef(config_cg);
NotifyCategoryDef(cg, "");


ConfigureFn(config_cg) {
  init_libcg();
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

  CgEffect::init_type();

  EffectPool *ts = EffectPool::get_global_ptr();
  ts->register_effect_type(CgEffect::make_effect, "cgfx");
}

