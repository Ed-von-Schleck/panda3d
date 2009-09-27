// Filename: physxActor.h
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

#ifndef PHYSXACTOR_H
#define PHYSXACTOR_H

#include "pandabase.h"
#include "nodePath.h"
#include "lmatrix.h"
#include "lvector3.h"
#include "lpoint3.h"
#include "lquaternion.h"

#include "physxObject.h"
#include "physxEnums.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxController;
class PhysxScene;
class PhysxShape;
class PhysxShapeDesc;

////////////////////////////////////////////////////////////////////
//       Class : PhysxActor
// Description : Actors are the main simulation objects. Actors
//               are owned by a scene (PhysxScene).
//
//               An actor may optionally encapsulate a dynamic rigid
//               body by setting the body member of the actor's
//               descriptor when it is created. Otherwise the actor
//               will be static (fixed in the world).
//
//               Instances of PhysxActor are created by calling
//               PhysxScene::create_actor() and destroyed by calling
//               PhysxActor::release().
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxActor : public PhysxObject, public PhysxEnums {

PUBLISHED:
  INLINE PhysxActor();
  INLINE ~PhysxActor();

  void set_name(const char *name);
  void set_global_pos(const LPoint3f &pos);
  void set_global_mat(const LMatrix4f &mat);
  void set_global_hpr(float h, float p, float r);
  void set_body_flag(PhysxBodyFlag flag, bool value);

  const char *get_name() const;
  LPoint3f get_global_pos() const;
  LMatrix4f get_global_mat() const;
  LQuaternionf get_global_quat() const;
  bool get_body_flag(PhysxBodyFlag flag) const;

  PT(PhysxScene) get_scene() const;

  void attach_node_path(const NodePath &np);
  void detach_node_path();
  NodePath get_node_path() const;

  unsigned int get_num_shapes() const;
  PT(PhysxShape) create_shape(PhysxShapeDesc &desc);
  PT(PhysxShape) get_shape(unsigned int idx) const;
  PT(PhysxShape) get_shape_by_name(const char *name) const;
  MAKE_SEQ(get_shapes, get_num_shapes, get_shape);

  // Forces
  void add_force(const LVector3f force, 
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_force_at_pos(const LVector3f force, const LPoint3f &pos,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_force_at_local_pos(const LVector3f force, const LPoint3f &pos,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_torque(const LVector3f torque,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_local_force(const LVector3f force,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_local_force_at_pos(const LVector3f force, const LPoint3f &pos,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_local_force_at_local_pos(const LVector3f force, const LPoint3f &pos,
     PhysxForceMode mode=FM_force, bool wakeup=true);
  void add_local_torque(const LVector3f torque,
     PhysxForceMode mode=FM_force, bool wakeup=true);

public:
  void update_transform(const LMatrix4f m);

////////////////////////////////////////////////////////////////////
PUBLISHED:
  void release();

public:
  INLINE NxActor *ptr() const { return _ptr; };

  void link_controller(PT(PhysxController) controller);
  void link(NxActor *ptr);
  void unlink();

private:
  NxActor *_ptr;
  NodePath _np;
  PT(PhysxController) _controller;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxObject::init_type();
    register_type(_type_handle, "PhysxActor", 
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

#include "physxActor.I"

#endif // PHYSXACTOR_H
