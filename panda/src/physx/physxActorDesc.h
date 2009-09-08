// Filename: physxActorDesc.h
// Created by:  enn0x (05Sep09)
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

#ifndef PHYSXACTORDESC_H
#define PHYSXACTORDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxActorDesc
// Description : Descriptor for PhysxActor.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxActorDesc : public TypedReferenceCount {

PUBLISHED:
  INLINE PhysxActorDesc();
  INLINE ~PhysxActorDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_name(const char *name);
  void set_density(float density);

public:
  INLINE PhysxActorDesc(NxActorDesc &desc);

  virtual NxActorDesc *ptr() { return &_desc; };

private:
  NxActorDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxActorDesc", 
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

#include "physxActorDesc.I"

#endif // PHYSXACTORDESC_H
