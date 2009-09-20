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

  // Link self
  ref();
  _ptr = actorPtr;
  _ptr->userData = this;
  _error_type = ET_ok;

  // Link shapes
  NxShape * const *shapes = _ptr->getShapes();
  NxU32 nShapes = _ptr->getNbShapes();

  for (NxU32 i=0; i < nShapes; i++) {
    if (shapes[i]->getName() == NULL) shapes[i]->setName("");

    PT(PhysxShape) shape = PhysxShape::factory(shapes[i]->getType());
    shape->link(shapes[i]);
  }

}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
unlink() {

  // Unlink shapes
  NxShape * const *shapes = _ptr->getShapes();
  NxU32 nShapes = _ptr->getNbShapes();

  for (NxU32 i=0; i < nShapes; i++) {
    PT(PhysxShape) shape = (PhysxShape *)shapes[i]->userData;
    shape->unlink();
  }

  // Unlink self
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
set_name(const char *name) {

  nassertv(_error_type == ET_ok);
  _ptr->setName(name);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_name
//       Access: Published
//  Description: Retrieves the name string.
////////////////////////////////////////////////////////////////////
const char *PhysxActor::
get_name() const {

  nassertr(_error_type == ET_ok, "");
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
  TODO when controllers are implemented...

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

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_scene
//       Access: Published
//  Description: Retrieves the scene which this actor belongs to.
////////////////////////////////////////////////////////////////////
PT(PhysxScene) PhysxActor::
get_scene() const {

  nassertr(_error_type == ET_ok, NULL);

  NxScene *scenePtr = &(_ptr->getScene());
  PhysxScene *scene = (PhysxScene *)(scenePtr->userData);

  return scene;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_num_shapes
//       Access: Published
//  Description: Returns the number of shapes assigned to the
//               actor.
////////////////////////////////////////////////////////////////////
unsigned int PhysxActor::
get_num_shapes() const {

  nassertr(_error_type == ET_ok, -1);

  return _ptr->getNbShapes();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::create_shape
//       Access: Published
//  Description: Creates a new shape and adds it to the list of
//               shapes of this actor. 
//
//               Mass properties of dynamic actors will not
//               automatically be recomputed to reflect the new mass
//               distribution implied by the shape. Follow this call
//               with a call to update_mass_from_shapes() to do
//               that.
////////////////////////////////////////////////////////////////////
PT(PhysxShape) PhysxActor::
create_shape(PhysxShapeDesc &desc) {

  nassertr(_error_type == ET_ok, NULL);
  nassertr(desc.is_valid(),NULL);

  PT(PhysxShape) shape = PhysxShape::factory(desc.ptr()->getType());
  nassertr(shape, NULL);

  NxShape *shapePtr = _ptr->createShape(*desc.ptr());
  nassertr(shapePtr, NULL);

  shape->link(shapePtr);

  return shape;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_shape
//       Access: Published
//  Description: Retrieves an individual shape from the actor's
//               array of shapes. Index must be in the range from
//               zero to (number-of-shapes minus 1).
////////////////////////////////////////////////////////////////////
PT(PhysxShape) PhysxActor::
get_shape(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);
  nassertr_always(idx < _ptr->getNbShapes(), NULL);

  NxShape * const *shapes = _ptr->getShapes();
  NxShape *shapePtr = shapes[idx];
  PhysxShape *shape = (PhysxShape *)(shapePtr->userData);

  return shape;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_shape_by_name
//       Access: Published
//  Description: Retrieves an individual shape from the actor's
//               array of shapes. The first shape for which the
//               shape's name matches the specified name is
//               returned, or NULL if no shape has a matching name.
////////////////////////////////////////////////////////////////////
PT(PhysxShape) PhysxActor::
get_shape_by_name(const char *name) const {

  nassertr(_error_type == ET_ok, NULL);

  NxShape * const *shapes = _ptr->getShapes();
  NxShape *shapePtr = NULL;
  NxU32 nShapes = _ptr->getNbShapes();

  for (NxU32 i=0; i < nShapes; i++) {
    shapePtr = shapes[i];

    if (strcmp(shapePtr->getName(), name) == 0) {
      return (PhysxShape *) shapePtr->userData;
    }
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_body_flag
//       Access: Published
//  Description: Raise or lower individual PhysxBodyFlag flags. 
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_body_flag(PhysxBodyFlag flag, bool value) {

  nassertv(_error_type == ET_ok);

  if (value == true) {
    _ptr->raiseBodyFlag((NxBodyFlag)flag);
  } else {
    _ptr->clearBodyFlag((NxBodyFlag)flag);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_body_flag
//       Access: Published
//  Description: Returns the specified PhysxBodyFlag flag.
////////////////////////////////////////////////////////////////////
bool PhysxActor::
get_body_flag(PhysxBodyFlag flag) const {

  nassertr(_error_type == ET_ok, false);

  return ptr()->readBodyFlag((NxBodyFlag)flag);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_force
//       Access: Published
//  Description: Applies a force (or impulse) defined in the global
//               coordinate frame to the actor.
//
//               This will not induce a torque.
//
//               The PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_force(const LVector3f force, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());

  _ptr->addForce(PhysxManager::vec3_to_nxVec3(force), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_force_at_pos
//       Access: Published
//  Description: Applies a force (or impulse) defined in the global
//               coordinate frame, acting at a particular point in
//               global coordinates, to the actor. 
//
//               Note that if the force does not act along the
//               center of mass of the actor, this will also add the
//               corresponding torque. Because forces are reset at
//               the end of every timestep, you can maintain a total
//               external force on an object by calling this once
//               every frame.
//
//               PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_force_at_pos(const LVector3f force, const LPoint3f &pos, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());
  nassertv_always(!pos.is_nan());

  _ptr->addForceAtPos(PhysxManager::vec3_to_nxVec3(force), PhysxManager::point3_to_nxVec3(pos), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_force_at_local_pos
//       Access: Published
//  Description: Applies a force (or impulse) defined in the global
//               coordinate frame, acting at a particular point in
//               local coordinates, to the actor. 
//
//               Note that if the force does not act along the
//               center of mass of the actor, this will also add
//               the corresponding torque. Because forces are reset
//               at the end of every timestep, you can maintain a
//               total external force on an object by calling this
//               once every frame.
//
//               PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_force_at_local_pos(const LVector3f force, const LPoint3f &pos, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());
  nassertv_always(!pos.is_nan());

  _ptr->addForceAtLocalPos(PhysxManager::vec3_to_nxVec3(force), PhysxManager::point3_to_nxVec3(pos), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_torque
//       Access: Published
//  Description: Applies an impulsive torque defined in the global
//               coordinate frame to the actor. 
//
//               The PhysxForceMode determines if the torque is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_torque(const LVector3f torque, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!torque.is_nan());

  _ptr->addTorque(PhysxManager::vec3_to_nxVec3(torque), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_local_force
//       Access: Published
//  Description: Applies a force (or impulse) defined in the actor
//               local coordinate frame to the actor. 
//               This will not induce a torque.
//
//               PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_local_force(const LVector3f force, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());

  _ptr->addLocalForce(PhysxManager::vec3_to_nxVec3(force), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_local_force_at_pos
//       Access: Published
//  Description: Applies a force (or impulse) defined in the actor
//               local coordinate frame, acting at a particular
//               point in global coordinates, to the actor. 
//
//               Note that if the force does not act along the
//               center of mass of the actor, this will also add
//               the corresponding torque. Because forces are reset
//               at the end of every timestep, you can maintain a
//               total external force on an object by calling this
//               once every frame.
//
//               PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_local_force_at_pos(const LVector3f force, const LPoint3f &pos, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());
  nassertv_always(!pos.is_nan());

  _ptr->addLocalForceAtPos(PhysxManager::vec3_to_nxVec3(force), PhysxManager::point3_to_nxVec3(pos), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_local_force_at_local_pos
//       Access: Published
//  Description: Applies a force (or impulse) defined in the actor
//               local coordinate frame, acting at a particular
//               point in local coordinates, to the actor. 
//
//               Note that if the force does not act along the
//               center of mass of the actor, this will also add the
//               corresponding torque. Because forces are reset at
//               the end of every timestep, you can maintain a total
//               external force on an object by calling this once
//               every frame.
//
//               PhysxForceMode determines if the force is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_local_force_at_local_pos(const LVector3f force, const LPoint3f &pos, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!force.is_nan());
  nassertv_always(!pos.is_nan());

  _ptr->addLocalForceAtLocalPos(PhysxManager::vec3_to_nxVec3(force), PhysxManager::point3_to_nxVec3(pos), (NxForceMode)mode, wakeup);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::add_local_torque
//       Access: Published
//  Description: Applies an impulsive torque defined in the actor
//               local coordinate frame to the actor.
//
//               PhysxForceMode determines if the torque is to be
//               conventional or impulsive.
//
//               The actor must be dynamic.
//               This call wakes the actor if it is sleeping and the
//               wakeup parameter is true (default).
////////////////////////////////////////////////////////////////////
void PhysxActor::
add_local_torque(const LVector3f torque, PhysxForceMode mode, bool wakeup) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!torque.is_nan());

  _ptr->addLocalTorque(PhysxManager::vec3_to_nxVec3(torque), (NxForceMode)mode, wakeup);
}

