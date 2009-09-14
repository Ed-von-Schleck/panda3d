// Filename: physxScene.h
// Created by:  enn0x (145Sep09)
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

#ifndef PHYSXSCENE_H
#define PHYSXSCENE_H

#include "pandabase.h"
#include "physxObject.h"

#include "lvector3.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysActor;
class PhysActorDesc;

////////////////////////////////////////////////////////////////////
//       Class : PhysxScene
// Description : A scene is a collection of bodies, constraints,
//               and effectors which can interact.
//
//               The scene simulates the behavior of these objects
//               over time. Several scenes may exist at the same
//               time, but each body, constraint, or effector object
//               is specific to a scene -- they may not be shared.
//
//               For example, attempting to create a joint in one
//               scene and then using it to attach bodies from a
//               different scene results in undefined behavior.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxScene : public PhysxObject {

PUBLISHED:
  INLINE PhysxScene();
  INLINE ~PhysxScene();

  void do_physics(float dt);

  void set_gravity(const LVector3f &gravity);
  LVector3f get_gravity() const;

  unsigned int get_num_actors() const;
  PT(PhysxActor) create_actor(PhysxActorDesc &desc);
  PT(PhysxActor) get_actor(unsigned int idx) const;
  MAKE_SEQ(get_actors, get_num_actors, get_actor);

////////////////////////////////////////////////////////////////////
PUBLISHED:
  void release();

public:
  INLINE NxScene *ptr() const { return _ptr; };

  void link(NxScene *ptr);
  void unlink();

private:
  NxScene *_ptr;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxObject::init_type();
    register_type(_type_handle, "PhysxScene", 
                  PhysxObject::get_class_type());
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

#include "physxScene.I"

#endif // PHYSXSCENE_H
