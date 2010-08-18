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

#include "bamCacheRecord.h"
#include <Cg/cg.h>
#include "config_cg.h"
#include "effect.h"
#include "renderState.h"

////////////////////////////////////////////////////////////////////
//       Class : CgEffect
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_CG CgEffect : public Effect {
PUBLISHED:
  INLINE CgEffect(const string &name = string());
  INLINE static PT(CgEffect) make_effect();
  virtual bool read(const Filename &fullpath, BamCacheRecord *record = NULL);
};

#include "cgEffect.I"

#endif

