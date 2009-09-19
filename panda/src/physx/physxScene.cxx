// Filename: physxScene.cxx
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

#include "physxScene.h"
#include "physxManager.h"

TypeHandle PhysxScene::_type_handle;

PStatCollector PhysxScene::_pcollector_fetch_results("App:PhysX:Fetch Results");
PStatCollector PhysxScene::_pcollector_update_transforms("App:PhysX:Update Transforms");
PStatCollector PhysxScene::_pcollector_debug_renderer("App:PhysX:Debug Renderer");
PStatCollector PhysxScene::_pcollector_simulate("App:PhysX:Simulate");

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
link(NxScene *scenePtr) {

  // Link self
  ref();
  _ptr = scenePtr;
  _ptr->userData = this;
  _error_type = ET_ok;

  // Link materials TODO
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
unlink() {

  // Unlink controllers TODO

  // Unlink actors
  NxActor **actors = _ptr->getActors();
  NxU32 nActors = _ptr->getNbActors();

  for (NxU32 i=0; i < nActors; i++) {
    PT(PhysxActor) actor = (PhysxActor *)actors[i]->userData;
    if (actor) {
      actor->unlink();
    }
  }

  // Unlink joints TODO
  // Unlink forcefields TODO
  // Unlink cloths TODO
  // Unlink softbodies TODO
  // Unlink materials TODO

  // Unlink self
  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  NxPhysicsSDK *sdk = NxGetPhysicsSDK();
  sdk->releaseScene(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::simulate
//       Access: Published
//  Description: Advances the simulation by an elapsedTime time.
//               The elapsed time has to be in the range (0, inf).
//
//               It is not allowed to modify the physics scene in
//               between the simulate(dt) and the fetch_results
//               calls!  But it is allowed to read from the scene
//               and do additional computations, e. g. AI, in
//               between these calls.
////////////////////////////////////////////////////////////////////
void PhysxScene::
simulate(float dt) {

  nassertv(_error_type == ET_ok);

  _pcollector_simulate.start();
  _ptr->simulate(dt);
  _ptr->flushStream();
  _pcollector_simulate.stop();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::fetch_results
//       Access: Published
//  Description: Waits until the simulation has finished, and then
//               updates the scene graph with with simulation
//               results.
//
//               It is not allowed to modify the physics scene in
//               between the simulate(dt) and the fetch_results
//               calls!  But it is allowed to read from the scene
//               and do additional computations, e. g. AI, in
//               between these calls.
////////////////////////////////////////////////////////////////////
void PhysxScene::
fetch_results() {

  nassertv(_error_type == ET_ok);

  _pcollector_fetch_results.start();
  _ptr->fetchResults(NX_RIGID_BODY_FINISHED, true);
  _pcollector_fetch_results.stop();

  // Update node transforms
  _pcollector_update_transforms.start();

  NxU32 nbTransforms = 0;
  NxActiveTransform *activeTransforms = _ptr->getActiveTransforms(nbTransforms);

  if (nbTransforms && activeTransforms) {
    for (NxU32 i=0; i<nbTransforms; ++i) {

      // Objects created by the Visual Remote Debugger might not have
      // user data. So check if user data ist set.
      void *userData = activeTransforms[i].userData;
      if (userData) {
        LMatrix4f m = PhysxManager::nxMat34_to_mat4(activeTransforms[i].actor2World);
        PhysxActor *actor = (PhysxActor *)userData;
        actor->update_transform(m);
      }
    }
  }
  _pcollector_update_transforms.stop();

  // Update debug node
  _pcollector_debug_renderer.start();
  _debugNode->update(_ptr);
  _pcollector_debug_renderer.stop();

  nassertv(_ptr->isWritable());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::set_gravity
//       Access: Published
//  Description: Sets a constant gravity for the entire scene.
////////////////////////////////////////////////////////////////////
void PhysxScene::
set_gravity(const LVector3f &gravity) {

  nassertv(_error_type == ET_ok);
  nassertv_always(!gravity.is_nan());

  _ptr->setGravity(PhysxManager::vec3_to_nxVec3(gravity));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_gravity
//       Access: Published
//  Description: Retrieves the current gravity setting.
////////////////////////////////////////////////////////////////////
LVector3f PhysxScene::
get_gravity() const {

  nassertr(_error_type == ET_ok, LVector3f::zero());

  NxVec3 gravity;
  _ptr->getGravity(gravity);
  return PhysxManager::nxVec3_to_vec3(gravity);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_num_actors
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
unsigned int PhysxScene::
get_num_actors() const {

  nassertr(_error_type == ET_ok,-1);

  return _ptr->getNbActors();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::create_actor
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxScene::
create_actor(PhysxActorDesc &desc) {

  nassertr(_error_type == ET_ok, NULL);
  nassertr(desc.is_valid(),NULL);

  PT(PhysxActor) actor = new PhysxActor();
  nassertr(actor, NULL);

  NxActor *actorPtr = _ptr->createActor(*desc.ptr());
  nassertr(actorPtr, NULL);

  actor->link(actorPtr);

  return actor;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_actor
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxActor) PhysxScene::
get_actor(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);
  nassertr_always(idx < _ptr->getNbActors(), NULL);

  NxActor *actorPtr = _ptr->getActors()[idx];
  PhysxActor *actor = (PhysxActor *)(actorPtr->userData);

  return actor;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_debug_node
//       Access: Published
//  Description: Retrieves the debug geom node for this scene. The
//               debug geom node is used to visualize information
//               about the physical scene which can be useful for
//               debugging an application.
//
//               The debug geom node geometry is generated in global
//               coordinates. In order to see correct information
//               it is important not to dislocate the debug node.
//               Reparent it to render and leave position at
//               (0,0,0).
////////////////////////////////////////////////////////////////////
PT(PhysxDebugGeomNode) PhysxScene::
get_debug_geom_node() {

  nassertr(_error_type == ET_ok, NULL);
  return _debugNode;
}

