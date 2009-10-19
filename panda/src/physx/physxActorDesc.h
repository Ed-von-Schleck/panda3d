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
#include "lpoint3.h"
#include "lmatrix.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxBodyDesc;
class PhysxShapeDesc;

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

  void add_shape(PhysxShapeDesc &desc);

  void set_name(const char *name);
  void set_density(float density);
  void set_global_pos(const LPoint3f &pos);
  void set_global_mat(const LMatrix4f &mat);
  void set_global_hpr(float h, float p, float r);
  void set_body(PhysxBodyDesc &desc);

  const char *get_name() const;
  float get_density() const;
  LPoint3f get_global_pos() const;
  LMatrix4f get_global_mat() const;
  PT(PhysxBodyDesc) get_body() const;

public:
  NxActorDesc *ptr() { return &_desc; };

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
