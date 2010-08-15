// Filename: effect.h
// Created by: Xidram (13Aug10)
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

#ifndef EFFECT_H
#define EFFECT_H

#include "internalName.h"
#include "pandaNode.h"
#include "simpleHashMap.h"
#include "technique.h"

////////////////////////////////////////////////////////////////////
//       Class : Effect
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH Effect : public PandaNode {
PUBLISHED:
  INLINE Effect(const string &name);
  virtual ~Effect() {};

  INLINE bool has_technique(CPT(InternalName) name);
  INLINE CPT(Technique) get_technique(CPT(InternalName) name) const;
  INLINE void set_technique(CPT(InternalName) name, PT(Technique) technique);
  INLINE void remove_technique(CPT(InternalName) name);

public:
  virtual bool cull_callback(CullTraverser *trav, CullTraverserData &data);

private:
  SimpleHashMap<CPT(InternalName), PT(Technique)> _techniques;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "Effect",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "effect.I"

#endif

