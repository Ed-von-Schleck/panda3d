// Filename: physxContactPair.h
// Created by:  enn0x (19Dec09)
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

#ifndef PHYSXCONTACTPAIR_H
#define PHYSXCONTACTPAIR_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "config_physx.h"

class PhysxShape;

////////////////////////////////////////////////////////////////////
//       Class : PhysxContactPair
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxContactPair : public TypedReferenceCount {

PUBLISHED:
  PT(PhysxActor) get_actor_a() const;
  PT(PhysxActor) get_actor_b() const;

  //void foo();

  INLINE ~PhysxContactPair();

public:
  INLINE PhysxContactPair(const NxContactPair pair);

private:
  NxContactPair _pair;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxContactPair", 
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "physxContactPair.I"

#endif // PHYSXCONTACTPAIR_H
