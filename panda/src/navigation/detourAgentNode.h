// Filename: detourAgentNode.h
// Created by: Xidram (28Jul10)
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

#ifndef DETOURAGENTNODE_H
#define DETOURAGENTNODE_H

#include "asyncTaskManager.h"
#include "config_navigation.h"
#include "detourNavMeshNode.h"
#include "genericAsyncTask.h"
#include "lpoint3.h"
#include "pandaNode.h"
#include "pointerTo.h"
#include "transformState.h"

////////////////////////////////////////////////////////////////////
//       Class : DetourAgentNode
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_NAVIGATION DetourAgentNode : public PandaNode {
PUBLISHED:
  INLINE DetourAgentNode(const string &name);
  INLINE DetourAgentNode(const string &name, DetourNavMeshNode* nav_mesh);
  INLINE virtual ~DetourAgentNode();

  INLINE PT(DetourNavMeshNode) get_nav_mesh() const;
  INLINE void set_nav_mesh(DetourNavMeshNode *nav_mesh);

  INLINE void move_to(PandaNode *target);
  INLINE void move_to(LPoint3f &target);
  INLINE void set_speed(float speed);

  void update(float dt);

private:
  PT(DetourNavMeshNode) _nav_mesh;
  PT(PandaNode) _target_node;
  PT(GenericAsyncTask) _task;
  float _speed;

  static INLINE AsyncTask::DoneStatus update_task(GenericAsyncTask* task, void* data);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PandaNode::init_type();
    register_type(_type_handle, "DetourAgentNode",
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "detourAgentNode.I"

#endif

