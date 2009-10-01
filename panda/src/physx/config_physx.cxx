// Filename: config_physx.cxx
// Created by:  enn0x (01Sep09)
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

#include "config_physx.h"
#include "pandaSystem.h"

#include "physxActor.h"
#include "physxActorDesc.h"
#include "physxBodyDesc.h"
#include "physxBoxController.h"
#include "physxBoxControllerDesc.h"
#include "physxBoxShape.h"
#include "physxBoxShapeDesc.h"
#include "physxCapsuleController.h"
#include "physxCapsuleControllerDesc.h"
#include "physxCapsuleShape.h"
#include "physxCapsuleShapeDesc.h"
#include "physxController.h"
#include "physxControllerDesc.h"
#include "physxCylindricalJointDesc.h"
#include "physxDebugGeomNode.h"
#include "physxDistanceJointDesc.h"
#include "physxFixedJointDesc.h"
#include "physxJointDesc.h"
#include "physxJointLimitDesc.h"
#include "physxMaterial.h"
#include "physxMaterialDesc.h"
#include "physxMotorDesc.h"
#include "physxObject.h"
#include "physxPlaneShape.h"
#include "physxPlaneShapeDesc.h"
#include "physxPointInPlaneJointDesc.h"
#include "physxPointOnLineJointDesc.h"
#include "physxPrismaticJointDesc.h"
#include "physxPulleyJointDesc.h"
#include "physxRevoluteJointDesc.h"
#include "physxScene.h"
#include "physxSceneDesc.h"
#include "physxShape.h"
#include "physxShapeDesc.h"
#include "physxSphereShape.h"
#include "physxSphereShapeDesc.h"
#include "physxSphericalJointDesc.h"
#include "physxSpringDesc.h"

ConfigureDef(config_physx);
NotifyCategoryDef(physx, "");

ConfigureFn(config_physx) {
  init_libphysx();
}

ConfigVariableBool physx_want_vrd
("physx-want-vrd", false,
PRC_DESC("Specified wether the manager should try to connect to the NVIDIA "
         "PhysX visual debugger or not. Connection is established when "
         "the first instance of PhysxManager is created."));

ConfigVariableString physx_vrd_host
("physx-vrd-host", "localhost",
PRC_DESC("Specified the host where the NVIDIA PhysX visual debugger is running"
         "on. Only used if the config-varibale 'physx-want-visual-debugger' "
         "is set to 'true'."));

ConfigVariableInt physx_vrd_port
("physx-visual-debugger-port", 5425,
PRC_DESC("Specified the port where the NVIDIA PhysX visual debugger is running"
         "on. Only used if the config-varibale 'physx-want-visual-debugger' "
         "is set to 'true'."));

ConfigVariableEnum<PhysxEnums::PhysxUpAxis> physx_up_axis
("physx-up-axis", PhysxEnums::Z_up,
PRC_DESC("Set the up direction for controllers and heightfields."));

////////////////////////////////////////////////////////////////////
//     Function: init_libphysx
//  Description: Initializes the library.  This must be called at
//               least once before any of the functions or classes in
//               this library can be used.  Normally it will be
//               called by the static initializers and need not be
//               called explicitly, but special cases exist.
////////////////////////////////////////////////////////////////////
void
init_libphysx() {
  static bool initialized = false;
  if (initialized) {
    return;
  }
  initialized = true;

  PhysxActor::init_type();
  PhysxActorDesc::init_type();
  PhysxBodyDesc::init_type();
  PhysxBoxController::init_type();
  PhysxBoxControllerDesc::init_type();
  PhysxBoxShape::init_type();
  PhysxBoxShapeDesc::init_type();
  PhysxCapsuleController::init_type();
  PhysxCapsuleControllerDesc::init_type();
  PhysxCapsuleShape::init_type();
  PhysxCapsuleShapeDesc::init_type();
  PhysxController::init_type();
  PhysxControllerDesc::init_type();
  PhysxCylindricalJointDesc::init_type();
  PhysxDebugGeomNode::init_type();
  PhysxDistanceJointDesc::init_type();
  PhysxFixedJointDesc::init_type();
  PhysxJointDesc::init_type();
  PhysxJointLimitDesc::init_type();
  PhysxMaterial::init_type();
  PhysxMaterialDesc::init_type();
  PhysxMotorDesc::init_type();
  PhysxObject::init_type();
  PhysxPlaneShape::init_type();
  PhysxPlaneShapeDesc::init_type();
  PhysxPointInPlaneJointDesc::init_type();
  PhysxPointOnLineJointDesc::init_type();
  PhysxPrismaticJointDesc::init_type();
  PhysxPulleyJointDesc::init_type();
  PhysxRevoluteJointDesc::init_type();
  PhysxScene::init_type();
  PhysxSceneDesc::init_type();
  PhysxShape::init_type();
  PhysxShapeDesc::init_type();
  PhysxSphereShape::init_type();
  PhysxSphereShapeDesc::init_type();
  PhysxSphericalJointDesc::init_type();
  PhysxSpringDesc::init_type();

  PandaSystem *ps = PandaSystem::get_global_ptr();
  ps->add_system("PhysX");
}

