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
#include "config_cg.h"
#include "effect.h"
#include "renderState.h"

////////////////////////////////////////////////////////////////////
//       Class : CgEffect
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_CG CgEffect : public Effect {
PUBLISHED:
  CgEffect(const string &name = string());
  virtual ~CgEffect();

  INLINE static PT(Effect) make_effect();
  virtual bool read(const Filename &fullpath, BamCacheRecord *record = NULL);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    Effect::init_type();
    register_type(_type_handle, "CgEffect",
                  Effect::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "cgEffect.I"

#endif

