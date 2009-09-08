// Filename: physxBodyDesc.h
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

#ifndef PHYSXBODYDESC_H
#define PHYSXBODYDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxBodyDesc
// Description : Descriptor for the optional rigid body dynamic
//               state of PhysxActor.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxBodyDesc : public TypedReferenceCount {

PUBLISHED:
  INLINE PhysxBodyDesc();
  INLINE ~PhysxBodyDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_mass(float mass);

public:
  INLINE PhysxBodyDesc(NxBodyDesc &desc);

  virtual NxBodyDesc *ptr() { return &_desc; };

private:
  NxBodyDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxBodyDesc", 
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

#include "physxBodyDesc.I"

#endif // PHYSXBODYDESC_H
