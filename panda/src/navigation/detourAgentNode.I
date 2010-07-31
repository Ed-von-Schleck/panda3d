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
DetourAgentNode(const string &name) : PandaNode(name),
  _speed(1.0f),
  _nav_mesh(NULL),
  _auto_direction(true),
  _heads_up(true) {

  PT(AsyncTaskManager) task_manager = AsyncTaskManager::get_global_ptr();
  _task = new GenericAsyncTask("agent-update", &update_task, (void*) this);
  task_manager->add(_task);
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE DetourAgentNode::
DetourAgentNode(const string &name, DetourNavMeshNode *nav_mesh) : PandaNode(name),
  _speed(1.0f),
  _nav_mesh(nav_mesh),
  _auto_direction(true),
  _heads_up(true) {

  PT(AsyncTaskManager) task_manager = AsyncTaskManager::get_global_ptr();
  _task = new GenericAsyncTask("agent-update", &update_task, (void*) this);
  task_manager->add(_task);
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
INLINE DetourAgentNode::
~DetourAgentNode() {
  if (_task != NULL) {
    _task->remove();
  }
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
//     Function: DetourAgentNode::get_auto_direction
//       Access: Public
//  Description: Returns true if the agent will automatically
//               update it's rotation based on the direction in
//               which it is going. The default is true.
////////////////////////////////////////////////////////////////////
INLINE bool DetourAgentNode::
get_auto_direction() const {
  return _auto_direction;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::get_auto_direction
//       Access: Public
//  Description: Set this to true if the agent should automatically
//               update it's rotation based on the direction in
//               which it is going. The default is true.
//               If the heads_up parameter is set to true (also
//               the default), it will use heads_up instead of
//               look_at to update the direction.
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
set_auto_direction(bool on, bool heads_up) {
  _auto_direction = on;
  _heads_up = heads_up;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::get_target_node
//       Access: Public
//  Description: Returns the PandaNode that is currently being
//               moved to. It could be a dummy node, if a point
//               was passed to move_to(). It returns NULL if the
//               agent is not moving, if it reached its end position.
////////////////////////////////////////////////////////////////////
INLINE PT(PandaNode) DetourAgentNode::
get_target_node() const {
  return _target_node;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::stop
//       Access: Public
//  Description: Clears the target that is currently set - this
//               makes the agent stop moving.
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
stop() {
  _target_node = NULL;
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
//     Function: DetourAgentNode::get_speed
//       Access: Public
//  Description: Returns the speed of the model, in units per second.
//               The default value is 1.0.
////////////////////////////////////////////////////////////////////
INLINE float DetourAgentNode::
get_speed() const {
  return _speed;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourAgentNode::set_speed
//       Access: Public
//  Description: Sets the speed of the model, in units per second.
//               The default value is 1.0.
////////////////////////////////////////////////////////////////////
INLINE void DetourAgentNode::
set_speed(float speed) {
  _speed = speed;
}

