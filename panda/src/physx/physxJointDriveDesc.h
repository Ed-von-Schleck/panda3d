// Filename: physxJointDriveDesc.h
// Created by:  enn0x (01Oct09)
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

#ifndef PHYSXJOINTDRIVEDESC_H
#define PHYSXJOINTDRIVEDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "physxEnums.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxJointDriveDesc
// Description : Used to describe drive properties for a
//               PhysxD6Joint.
////////////////////////////////////////////////////////////////////
class PhysxJointDriveDesc : public TypedReferenceCount, public PhysxEnums {

PUBLISHED:
  INLINE PhysxJointDriveDesc();
  INLINE PhysxJointDriveDesc(float sping, float damping, float forceLimit);
  INLINE ~PhysxJointDriveDesc();

  void set_drive_type(PhysxD6JointDriveType type);
  void set_spring(float spring);
  void set_damping(float damping);
  void set_force_limit(float limit);

  PhysxD6JointDriveType get_drive_type() const;
  float get_spring() const;
  float get_damping() const;
  float get_force_limit() const;

public:
  INLINE PhysxJointDriveDesc(const NxJointDriveDesc &desc);
  INLINE NxJointDriveDesc desc() const;

private:
  NxJointDriveDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxJointDriveDesc", 
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

#include "physxJointDriveDesc.I"

#endif // PHYSXJOINTDRIVEDESC_H
