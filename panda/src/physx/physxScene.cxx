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

  _cm = NxCreateControllerManager(NxGetPhysicsSDKAllocator());
  nassertv_always(_cm);

  // Link materials
  NxMaterial *materials[5];
  NxU32 iterator = 0;

  while (NxU32 i=_ptr->getMaterialArray(materials, 5, iterator)) {
    while(i--) {
      PT(PhysxMaterial) material = new PhysxMaterial();
      material->link(materials[i]);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxScene::
unlink() {

  // Unlink controllers TODO
  NxU32 nControllers = _cm->getNbControllers();

  for (NxU32 i=0; i < nControllers; i++) {
    NxController *controllerPtr = _cm->getController(i);
    PT(PhysxController) controller = (PhysxController *)controllerPtr->getUserData();
    controller->unlink();
  }

  // Unlink actors
  NxActor **actors = _ptr->getActors();
  NxU32 nActors = _ptr->getNbActors();

  for (NxU32 i=0; i < nActors; i++) {
    PT(PhysxActor) actor = (PhysxActor *)actors[i]->userData;

    // Actor could have already been unlinked by controller
    if (actor) {
      actor->unlink();
    }
  }

  // Unlink joints TODO
  // Unlink forcefields TODO
  // Unlink cloths TODO
  // Unlink softbodies TODO
  // Unlink materials TODO

  // Unlink materials
  NxMaterial *materials[5];
  NxU32 iterator = 0;

  while (NxU32 i=_ptr->getMaterialArray(materials, 5, iterator)) {
    while(i--) {
      PT(PhysxMaterial) material = (PhysxMaterial *)materials[i]->userData;
      material->unlink();
    }
  }

  // Unlink self
  NxReleaseControllerManager(_cm);
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

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::enable_contact_reporting
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxScene::
enable_contact_reporting(bool enabled) {

  nassertv(_error_type == ET_ok);

  if (enabled) {
    _ptr->setUserContactReport(&_contact_report);
    _contact_report.enable();
  }
  else {
    _ptr->setUserContactReport(NULL);
    _contact_report.disable();
  }
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::is_contact_reporting_enabled
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxScene::
is_contact_reporting_enabled() const {

  nassertr(_error_type == ET_ok, false);

  return _contact_report.is_enabled();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::enable_trigger_reporting
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxScene::
enable_trigger_reporting(bool enabled) {

  nassertv(_error_type == ET_ok);

  if (enabled) {
    _ptr->setUserTriggerReport(&_trigger_report);
    _trigger_report.enable();
  }
  else {
    _ptr->setUserTriggerReport(NULL);
    _trigger_report.disable();
  }
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::is_trigger_reporting_enabled
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxScene::
is_trigger_reporting_enabled() const {

  nassertr(_error_type == ET_ok, false);

  return _trigger_report.is_enabled();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::enable_controller_reporting
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxScene::
enable_controller_reporting(bool enabled) {

  nassertv(_error_type == ET_ok);

  if (enabled) {
    _controller_report.enable();
  }
  else {
    _controller_report.disable();
  }
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxScene::is_controller_reporting_enabled
//       Access : Published
//  Description :
////////////////////////////////////////////////////////////////////
bool PhysxScene::
is_controller_reporting_enabled() const {

  nassertr(_error_type == ET_ok, false);

  return _controller_report.is_enabled();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_num_materials
//       Access: Published
//  Description: Return the number of materials in the scene. 
//
//               Note that the returned value is not related to
//               material indices. Those may not be allocated
//               continuously, and its values may be higher than
//               get_num_materials(). This will also include the
//               default material which exists without having to
//               be created.
////////////////////////////////////////////////////////////////////
unsigned int PhysxScene::
get_num_materials() const {

  nassertr(_error_type == ET_ok, -1);
  return _ptr->getNbMaterials();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::create_material
//       Access: Published
//  Description: Creates a new PhysxMaterial.
//
//               The material library consists of an array of
//               material objects. Each material has a well defined
//               index that can be used to refer to it. If an object
//               references an undefined material, the default
//               material with index 0 is used instead.
////////////////////////////////////////////////////////////////////
PT(PhysxMaterial) PhysxScene::
create_material(PhysxMaterialDesc &desc) {

  nassertr(_error_type == ET_ok, NULL);
  nassertr(desc.is_valid(),NULL);

  PT(PhysxMaterial) material = new PhysxMaterial();
  nassertr(material, NULL);

  NxMaterial *materialPtr = _ptr->createMaterial(*desc.ptr());
  nassertr(materialPtr, NULL);

  material->link(materialPtr);

  return material;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::create_material
//       Access: Published
//  Description: Creates a new PhysxMaterial using the default
//               settings of PhysxMaterialDesc.
////////////////////////////////////////////////////////////////////
PT(PhysxMaterial) PhysxScene::
create_material() {

  nassertr(_error_type == ET_ok, NULL);

  PT(PhysxMaterial) material = new PhysxMaterial();
  nassertr(material, NULL);

  NxMaterialDesc desc;
  desc.setToDefault();
  NxMaterial *materialPtr = _ptr->createMaterial(desc);
  nassertr(materialPtr, NULL);

  material->link(materialPtr);

  return material;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_hightest_material_index
//       Access: Published
//  Description: Returns current highest valid material index.
//
//               Note that not all indices below this are valid if
//               some of them belong to meshes that have beed
//               freed.
////////////////////////////////////////////////////////////////////
unsigned int PhysxScene::
get_hightest_material_index() const {

  nassertr(_error_type == ET_ok, -1);
  return _ptr->getHighestMaterialIndex();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_material_from_index
//       Access: Published
//  Description: Retrieves the material with the given material
//               index. 
//
//               There is always at least one material in the Scene,
//               the default material (index 0). If the specified
//               material index is out of range (larger than
//               get_hightest_material_index) or belongs to a
//               material that has been released, then the default
//               material is returned, but no error is reported.
////////////////////////////////////////////////////////////////////
PT(PhysxMaterial) PhysxScene::
get_material_from_index(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);

  NxMaterial *materialPtr = _ptr->getMaterialFromIndex(idx);

  return (PhysxMaterial *)(materialPtr->userData);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_material
//       Access: Published
//  Description: Retrieves the n-th material from the array of
//               materials. See also get_material_from_index,
//               which retrieves a material by it's material index.
////////////////////////////////////////////////////////////////////
PT(PhysxMaterial) PhysxScene::
get_material(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);
  nassertr_always(idx < _ptr->getNbMaterials(), NULL);

  NxU32 n = _ptr->getNbMaterials();
  NxMaterial **materials = new NxMaterial *[n];
  NxU32 materialCount;
  NxU32 iterator = 0;

  materialCount = _ptr->getMaterialArray(materials, n, iterator);
  nassertr((materialCount == n), NULL);

  NxMaterial *materialPtr = materials[idx];
  delete[] materials;

  return (PhysxMaterial *)(materialPtr->userData);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_num_controllers
//       Access: Published
//  Description: Return the number of controllers in the scene. 
////////////////////////////////////////////////////////////////////
unsigned int PhysxScene::
get_num_controllers() const {

  nassertr(_error_type == ET_ok, -1);
  return _cm->getNbControllers();
}


////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::create_controller
//       Access: Published
//  Description: Creates a new character controller.
////////////////////////////////////////////////////////////////////
PT(PhysxController) PhysxScene::
create_controller(PhysxControllerDesc &desc) {

  nassertr(_error_type == ET_ok, NULL);
  nassertr(desc.is_valid(),NULL);

  PT(PhysxController) controller = PhysxController::factory(desc.ptr()->getType());
  nassertr(controller, NULL);

  desc.ptr()->callback = &_controller_report;
  desc.ptr()->userData = controller;

  NxController *controllerPtr = _cm->createController(_ptr,*desc.ptr());
  nassertr(controllerPtr, NULL);

  controller->link(controllerPtr);
  controllerPtr->getActor()->setName("");

  return controller;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxScene::get_controller
//       Access: Published
//  Description: Retrieves the n-th controller within the scene.
////////////////////////////////////////////////////////////////////
PT(PhysxController) PhysxScene::
get_controller(unsigned int idx) const {

  nassertr(_error_type == ET_ok, NULL);
  nassertr_always(idx < _cm->getNbControllers(), NULL);

  NxController *controllerPtr = _cm->getController(idx);
  PhysxController *controller = (PhysxController *)(controllerPtr->getUserData());

  return controller;
}

