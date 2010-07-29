// Filename: detourAgentNode.I
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


////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE DetourAgentNode::
DetourAgentNode(const string &name) : PandaNode(name) {
  _nav_mesh = NULL;
  PT(AsyncTaskManager) task_manager = AsyncTaskManager::get_global_ptr();
  task = new GenericAsyncTask("UpdateTask", &update_task, (void*) this);
  task_manager->add(task);
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::update_task
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE AsyncTask::DoneStatus DetourAgentNode::
update_task(GenericAsyncTask* task, void* data) {
  ClockObject *clock = task->get_manager()->get_clock();
  ((DetourAgentNode*)data)->update(clock->get_dt());
  return AsyncTask::DS_cont;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::get_nav_mesh
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PT(DetourNavMeshNode) DetourAgentNode::
get_nav_mesh() const {
  return _nav_mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::set_nav_mesh
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
set_nav_mesh(DetourNavMeshNode *nav_mesh) {
  _nav_mesh = nav_mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::move_to
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
move_to(PandaNode *target) {
  _target_node = target;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::move_to
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
move_to(LPoint3f &target) {
  _target_node = new PandaNode("");
  _target_node->set_transform(TransformState::make_pos(target));
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::set_speed
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
set_speed(float speed) {
  _speed = speed;
}

