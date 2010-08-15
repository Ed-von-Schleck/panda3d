// Filename: cgEffect.h
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

#ifndef CGEFFECT_H
#define CGEFFECT_H

#include <Cg/cg.h>
#include "config_navigation.h"
#include "effect.h"

////////////////////////////////////////////////////////////////////
//       Class : CgEffect
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_CG CgEffect : public Effect {
PUBLISHED:
  INLINE CgEffect(const string &name);
  bool read(const Filename &fullpath);
};

#include "cgEffect.I"

#endif

