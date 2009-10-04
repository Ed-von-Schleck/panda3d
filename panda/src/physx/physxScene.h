// Filename: physxScene.h
// Created by:  enn0x (14Sep09)
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
#include "lvector3.h"

#include "physxObject.h"
#include "physxContactReport.h"
#include "PhysxControllerReport.h"
#include "physxTriggerReport.h"

#include "NoMinMax.h"
#include "NxPhysics.h"
#include "NxControllerManager.h"

class PhysxActor;
class PhysxActorDesc;
class PhysxMaterial;
class PhysxMaterialDesc;
class PhysxController;
class PhysxControllerDesc;
class PhysxJoint;
class PhysxJointDesc;
class PhysxDebugGeomNode;

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

  void simulate(float dt);
  void fetch_results();

  PT(PhysxDebugGeomNode) get_debug_geom_node();

  void enable_contact_reporting(bool enabled);
  bool is_contact_reporting_enabled() const;
  void enable_trigger_reporting(bool enabled);
  bool is_trigger_reporting_enabled() const;
  void enable_controller_reporting(bool enabled);
  bool is_controller_reporting_enabled() const;

  void set_gravity(const LVector3f &gravity);
  LVector3f get_gravity() const;

  unsigned int get_num_actors() const;
  PT(PhysxActor) create_actor(PhysxActorDesc &desc);
  PT(PhysxActor) get_actor(unsigned int idx) const;
  MAKE_SEQ(get_actors, get_num_actors, get_actor);

  unsigned int get_num_joints() const;
  PT(PhysxJoint) create_joint(PhysxJointDesc &desc);
  PT(PhysxJoint) get_joint(unsigned int idx) const;
  MAKE_SEQ(get_joints, get_num_joints, get_joint);

  unsigned int get_num_materials() const;
  unsigned int get_hightest_material_index() const;
  PT(PhysxMaterial) create_material(PhysxMaterialDesc &desc);
  PT(PhysxMaterial) create_material();
  PT(PhysxMaterial) get_material(unsigned int idx) const;
  PT(PhysxMaterial) get_material_from_index(unsigned int idx) const;
  MAKE_SEQ(get_materials, get_num_materials, get_material);

  unsigned int get_num_controllers() const;
  PT(PhysxController) create_controller(PhysxControllerDesc &controllerDesc);
  PT(PhysxController) get_controller(unsigned int idx) const;
  MAKE_SEQ(get_controllers, get_num_controllers, get_controller);

////////////////////////////////////////////////////////////////////
PUBLISHED:
  void release();

public:
  INLINE NxScene *ptr() const { return _ptr; };
  INLINE NxControllerManager *cm() const { return _cm; };

  void link(NxScene *ptr);
  void unlink();

private:
  NxScene *_ptr;
  NxControllerManager *_cm;
  PT(PhysxDebugGeomNode) _debugNode;

  PhysxContactReport _contact_report;
  PhysxControllerReport _controller_report;
  PhysxTriggerReport _trigger_report;

  static PStatCollector _pcollector_fetch_results;
  static PStatCollector _pcollector_update_transforms;
  static PStatCollector _pcollector_debug_renderer;
  static PStatCollector _pcollector_simulate;

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
