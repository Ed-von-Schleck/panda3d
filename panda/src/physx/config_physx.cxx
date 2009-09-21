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
#include "physxBoxShape.h"
#include "physxBoxShapeDesc.h"
#include "physxCapsuleShape.h"
#include "physxCapsuleShapeDesc.h"
#include "physxDebugGeomNode.h"
#include "physxMaterial.h"
#include "physxMaterialDesc.h"
#include "physxObject.h"
#include "physxPlaneShape.h"
#include "physxPlaneShapeDesc.h"
#include "physxScene.h"
#include "physxSceneDesc.h"
#include "physxShape.h"
#include "physxShapeDesc.h"
#include "physxSphereShape.h"
#include "physxSphereShapeDesc.h"

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
  PhysxBoxShape::init_type();
  PhysxBoxShapeDesc::init_type();
  PhysxCapsuleShape::init_type();
  PhysxCapsuleShapeDesc::init_type();
  PhysxDebugGeomNode::init_type();
  PhysxMaterial::init_type();
  PhysxMaterialDesc::init_type();
  PhysxObject::init_type();
  PhysxPlaneShape::init_type();
  PhysxPlaneShapeDesc::init_type();
  PhysxScene::init_type();
  PhysxSceneDesc::init_type();
  PhysxShape::init_type();
  PhysxShapeDesc::init_type();
  PhysxSphereShape::init_type();
  PhysxSphereShapeDesc::init_type();

  PandaSystem *ps = PandaSystem::get_global_ptr();
  ps->add_system("PhysX");
}

