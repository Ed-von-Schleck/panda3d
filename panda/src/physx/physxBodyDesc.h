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
#include "lvector3.h"
#include "lmatrix.h"

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
  void set_linear_damping(float damping);
  void set_angular_damping(float damping);
  void set_linear_velocity(const LVector3f &velocity);
  void set_angular_velocity(const LVector3f &velocity);
  void set_max_angular_velocity(float maximum);
  void set_sleep_linear_velocity(float velocity);
  void set_sleep_angular_velocity(float velocity);
  void set_solver_iteration_count(unsigned int count);
  void set_sleep_energy_threshold(float threshold);
  void set_sleep_damping(float damping);
  void set_mass_local_mat(const LMatrix4f mat);
  void set_mass_space_inertia(const LVector3f inertia);

  float get_mass() const;
  float get_linear_damping() const;
  float get_angular_damping() const;
  LVector3f get_linear_velocity() const;
  LVector3f get_angular_velocity() const;
  float get_max_angular_velocity() const;
  float get_sleep_linear_velocity() const;
  float get_sleep_angular_velocity() const;
  unsigned int get_solver_iteration_count() const;
  float get_sleep_energy_threshold() const;
  float get_sleep_damping() const;
  LMatrix4f get_mass_local_mat() const;
  LVector3f get_mass_space_inertia() const;

/*
  enum BodyFlag {
    BF_disable_gravity    = (1<<0),   // NX_BF_DISABLE_GRAVITY
    Bf_frozen_pos_x       = (1<<1),   // NX_BF_FROZEN_POS_X
    BF_frozen_pos_y       = (1<<2),   // NX_BF_FROZEN_POS_Y
    BF_frozen_pos_z       = (1<<3),   // NX_BF_FROZEN_POS_Z
    BF_frozen_rot_x       = (1<<4),   // NX_BF_FROZEN_ROT_X
    BF_frozen_rot_y       = (1<<5),   // NX_BF_FROZEN_ROT_Y
    BF_frozen_rot_z       = (1<<6),   // NX_BF_FROZEN_ROT_Z
    BF_kinematic          = (1<<7),   // NX_BF_KINEMATIC
    BF_visualization      = (1<<8),   // NX_BF_VISUALIZATION
    BF_filter_sleep_vel   = (1<<10),  // NX_BF_FILTER_SLEEP_VEL
    BF_energy_sleep_test  = (1<<11),  // NX_BF_ENERGY_SLEEP_TEST
  };

  void set_flag( BodyFlag flag, bool value );
*/

public:
  INLINE PhysxBodyDesc(const NxBodyDesc *desc);

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
