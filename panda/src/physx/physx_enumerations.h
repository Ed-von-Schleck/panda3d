// Filename: physxEnumerations.h
// Created by:  enn0x (20Sep09)
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

#ifndef PHYSX_ENUMERATIONS_H
#define PHYSX_ENUMERATIONS_H

#include "NoMinMax.h"
#include "NxPhysics.h"

#ifdef CPPPARSER

// PhysxBodyFlag
#define NX_BF_DISABLE_GRAVITY 1<<0
#define NX_BF_FROZEN_POS_X 1<<1
#define NX_BF_FROZEN_POS_Y 1<<2
#define NX_BF_FROZEN_POS_Z 1<<3
#define NX_BF_FROZEN_ROT_X 1<<4
#define NX_BF_FROZEN_ROT_Y 1<<5
#define NX_BF_FROZEN_ROT_Z 1<<6
#define NX_BF_FROZEN_POS 256
#define NX_BF_FROZEN_ROT 131072
#define NX_BF_FROZEN 131328
#define NX_BF_KINEMATIC 1<<7
#define NX_BF_VISUALIZATION 1<<8
#define NX_BF_FILTER_SLEEP_VEL 1<<10
#define NX_BF_ENERGY_SLEEP_TEST 1<<11

// PhysxForceMode
#define NX_FORCE 0
#define NX_IMPULSE 1
#define NX_VELOCITY_CHANGE 2
#define NX_SMOOTH_IMPULSE 3
#define NX_SMOOTH_VELOCITY_CHANGE 4
#define NX_ACCELERATION 5

// PhysxShapeFlag
#define NX_TRIGGER_ON_ENTER 1<<0
#define NX_TRIGGER_ON_LEAVE 1<<1
#define NX_TRIGGER_ON_STAY 1<<2
#define NX_TRIGGER_ENABLE 7
#define NX_SF_VISUALIZATION 1<<3
#define NX_SF_DISABLE_COLLISION 1<<4
#define NX_SF_FEATURE_INDICES 1<<5
#define NX_SF_DISABLE_RAYCASTING 1<<6
#define NX_SF_POINT_CONTACT_FORCE 1<<7
#define NX_SF_FLUID_DRAIN 1<<8
#define NX_SF_FLUID_DISABLE_COLLISION 1<<10
#define NX_SF_FLUID_TWOWAY 1<<11
#define NX_SF_DISABLE_RESPONSE 1<<12
#define NX_SF_DYNAMIC_DYNAMIC_CCD 1<<13
#define NX_SF_DISABLE_SCENE_QUERIES 1<<14
#define NX_SF_CLOTH_DRAIN 1<<15
#define NX_SF_CLOTH_DISABLE_COLLISION 1<<16
#define NX_SF_CLOTH_TWOWAY 1<<17
#define NX_SF_SOFTBODY_DRAIN 1<<18
#define NX_SF_SOFTBODY_DISABLE_COLLISION 1<<19
#define NX_SF_SOFTBODY_TWOWAY 1<<20

#endif // CPPPARSER

enum PhysxBodyFlag {
  BF_disable_gravity   = NX_BF_DISABLE_GRAVITY,
  Bf_frozen_pos_x      = NX_BF_FROZEN_POS_X,
  BF_frozen_pos_y      = NX_BF_FROZEN_POS_Y,
  BF_frozen_pos_z      = NX_BF_FROZEN_POS_Z,
  BF_frozen_rot_x      = NX_BF_FROZEN_ROT_X,
  BF_frozen_rot_y      = NX_BF_FROZEN_ROT_Y,
  BF_frozen_rot_z      = NX_BF_FROZEN_ROT_Z,
  BF_frozen_pos        = NX_BF_FROZEN_POS,
  BF_frozen_rot        = NX_BF_FROZEN_ROT,
  BF_frozen            = NX_BF_FROZEN,
  BF_kinematic         = NX_BF_KINEMATIC,
  BF_visualization     = NX_BF_VISUALIZATION,
  BF_filter_sleep_vel  = NX_BF_FILTER_SLEEP_VEL,
  BF_energy_sleep_test = NX_BF_ENERGY_SLEEP_TEST
};

enum PhysxForceMode {
  FM_force                  = NX_FORCE,
  FM_impulse                = NX_IMPULSE,
  FM_velocity_change        = NX_VELOCITY_CHANGE,
  FM_smooth_impulse         = NX_SMOOTH_IMPULSE,
  FM_smooth_velocity_change = NX_SMOOTH_VELOCITY_CHANGE,
  FM_acceleration           = NX_ACCELERATION
};

enum PhysxShapeFlag {
  SF_trigger_on_enter           = NX_TRIGGER_ON_ENTER,
  SF_trigger_on_leave           = NX_TRIGGER_ON_LEAVE,
  SF_trigger_on_stay            = NX_TRIGGER_ON_STAY,
  SF_trigger_enable             = NX_TRIGGER_ENABLE,
  SF_visualization              = NX_SF_VISUALIZATION,
  SF_disable_collision          = NX_SF_DISABLE_COLLISION,
  SF_disable_raycasting         = NX_SF_DISABLE_RAYCASTING,
  SF_disable_response           = NX_SF_DISABLE_RESPONSE,
  SF_disable_scene_queries      = NX_SF_DISABLE_SCENE_QUERIES,
  SF_point_contact_force        = NX_SF_POINT_CONTACT_FORCE,
  SF_feature_indices            = NX_SF_FEATURE_INDICES,
  SF_dynamic_dynamic_ccd        = NX_SF_DYNAMIC_DYNAMIC_CCD,
  SF_fluid_drain                = NX_SF_FLUID_DRAIN,
  SF_fluid_disable_collision    = NX_SF_FLUID_DISABLE_COLLISION,
  SF_fluid_twoway               = NX_SF_FLUID_TWOWAY,
  SF_cloth_drain                = NX_SF_CLOTH_DRAIN,
  SF_cloth_disable_collision    = NX_SF_CLOTH_DISABLE_COLLISION,
  SF_cloth_twoway               = NX_SF_CLOTH_TWOWAY,
  SF_softbody_drain             = NX_SF_SOFTBODY_DRAIN,
  SF_softbody_disable_collision = NX_SF_SOFTBODY_DISABLE_COLLISION,
  SF_softbody_twoway            = NX_SF_SOFTBODY_TWOWAY
};

#endif // PHYSX_ENUMERATIONS_H
