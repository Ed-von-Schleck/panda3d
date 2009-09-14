// Filename: physxActor.cxx
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

#include "physxActor.h"
#include "physxManager.h"

TypeHandle PhysxActor::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
link(NxActor *actorPtr) {

  // TODO: link shapes

  ref();
  _ptr = actorPtr;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
unlink() {

  // TODO: unlink shapes etc.

  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  _ptr->getScene().releaseActor(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_name
//       Access: Published
//  Description: Sets a name string for the object that can be
//               retrieved with get_name(). 
//               This is for debugging and is not used by the
//               engine.
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_name( const char *name ) {

  nassertv( _error_type == ET_ok );
  _ptr->setName( name );
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_name
//       Access: Published
//  Description: Retrieves the name string.
////////////////////////////////////////////////////////////////////
const char *PhysxActor::
get_name() const {

  nassertr( _error_type == ET_ok, "" );
  return _ptr->getName();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::update_transform
//       Access: Public
//  Description: Updates the transform of an assigned NodePath. If
//               the actor has been created by a PhysxController
//               then this method will update the NodePath's
//               transform from the controller's transform.
////////////////////////////////////////////////////////////////////
void PhysxActor::
update_transform(const LMatrix4f m) {

  // Active transforms are update AFTER scene.fetchResults() has
  // been called, and thus can contain removed objects. So either
  // update transforms after scene.fetchResults() - which means
  // poor performance - or check if an actor has been removed here
  // in this method.
  if (_error_type != ET_ok) return;

  if (_np.is_empty()) return;

  if (true) {
    _np.set_transform(_np.get_top(), TransformState::make_mat(m));
  }

/*
  if (_controller) {
    LVector3f hpr(_controller->get_h(), 0.0f, 0.0f);
    LPoint3f pos = _controller->get_pos();

    _np.set_transform(_np.get_top(), TransformState::make_pos_hpr(pos, hpr));

  } else {
    _np.set_transform(_np.get_top(), TransformState::make_mat(m));
  }
*/
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_global_pos
//       Access: Published
//  Description: Retrieves the actors world space position.
////////////////////////////////////////////////////////////////////
LPoint3f PhysxActor::
get_global_pos() const {

  nassertr(_error_type == ET_ok, LPoint3f::zero());
  return PhysxManager::nxVec3_to_point3(_ptr->getGlobalPosition());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_global_mat
//       Access: Published
//  Description: Retrieves the actors world space transform.
////////////////////////////////////////////////////////////////////
LMatrix4f PhysxActor::
get_global_mat() const {

  nassertr(_error_type == ET_ok, LMatrix4f::zeros_mat());
  return PhysxManager::nxMat34_to_mat4(_ptr->getGlobalPose());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_global_quat
//       Access: Published
//  Description: Retrieves the actors world space orientation.
////////////////////////////////////////////////////////////////////
LQuaternionf PhysxActor::
get_global_quat() const {

  nassertr(_error_type == ET_ok, LQuaternionf::zero());
  return PhysxManager::nxQuat_to_quat(_ptr->getGlobalOrientation());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_global_pos
//       Access: Published
//  Description: Method for setting a dynamic actor's position in
//               the world. Please see set_global_mat for some
//               caveats.
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_global_pos(const LPoint3f &pos) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!pos.is_nan());

  _ptr->setGlobalPosition(PhysxManager::point3_to_nxVec3(pos));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_global_mat
//       Access: Published
//  Description: Method for setting a dynamic actor's transform
//               matrix in the world. 
//
//               This method instantaneously changes the actor space
//               to world space transformation.
//
//               One should exercise restraint in making use of
//               these methods.
//
//               Static actors should not be moved at all. There are
//               various internal data structures for static actors
//               which may need to be recomputed when one moves.
//               Also, moving static actors will not interact
//               correctly with dynamic actors or joints. If you
//               would like to directly control an actor's position
//               and would like to have it correctly interact with
//               dynamic bodies and joints, you should create a
//               dynamic body with the BF_kinematic flag, and then
//               use the move_lobal_*() commands to move it along
//               a path!
//
//               When briefly moving dynamic actors, one should not:
//               - Move actors into other actors, thus causing
//                 interpenetration (an invalid physical state).
//               - Move an actor that is connected by a joint to
//                 another away from the other (thus causing joint
//                 error).
//               - When moving jointed actors the joints' cached
//                 transform information is destroyed and recreated
//                 next frame; thus this call is expensive for
//                 jointed actors. 
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_global_mat(const LMatrix4f &mat) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!mat.is_nan());

  _ptr->setGlobalPose(PhysxManager::mat4_to_nxMat34(mat));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_global_hpr
//       Access: Published
//  Description: Method for setting a dynamic actor's orientation in
//               the world. Please see set_global_mat for some
//               caveats.
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_global_hpr(float h, float p, float r) {

  nassertv(_error_type == ET_ok);

  LQuaternionf q;
  q.set_hpr(LVector3f(h, p, r));
  _ptr->setGlobalOrientationQuat(PhysxManager::quat_to_nxQuat(q));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::attach_node_path
//       Access: Published
//  Description: Attaches a node path to this actor. The node
//               path's transform will be updated automatically if
//               the actor's transform changes (and only then).
//
//               Note: any non-uniform scale or shear set on the
//               NodePath's transform will be overwritten at the
//               time of the first update.
////////////////////////////////////////////////////////////////////
void PhysxActor::
attach_node_path(const NodePath &np) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!np.is_empty());

  _np = NodePath(np);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::detach_node_path
//       Access: Published
//  Description: Detaches a previously assigned NodePath from this
//               actor. The NodePath's transform will no longer
//               be updated from the actor's transform.
////////////////////////////////////////////////////////////////////
void PhysxActor::
detach_node_path() {

  nassertv(_error_type == ET_ok);

  _np = NodePath();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_node_path
//       Access: Published
//  Description: Retrieves a previously attached NodePath. An empty
//               NodePath will be returned if no NodePath has been
//               attached to this actor.
////////////////////////////////////////////////////////////////////
NodePath PhysxActor::
get_node_path() const {

  nassertr(_error_type == ET_ok, NodePath::fail());

  return _np;
}

