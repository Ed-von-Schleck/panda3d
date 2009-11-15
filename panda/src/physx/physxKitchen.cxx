// Filename: physxKitchen.cxx
// Created by:  enn0x (12Oct09)
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

#include "physxKitchen.h"
#include "physxConvexMesh.h"
#include "physxConvexMeshDesc.h"
#include "physxTriangleMesh.h"
#include "physxTriangleMeshDesc.h"
#include "physxFileStream.h"
#include "physxMemoryReadBuffer.h"
#include "physxMemoryWriteBuffer.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxKitchen::cook_convex_mesh
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
bool PhysxKitchen::
cook_convex_mesh(PhysxConvexMeshDesc meshDesc, const Filename &filename) {

  nassertr_always(!filename.empty(), false);
  nassertr_always(filename.touch(), false);
  nassertr_always(meshDesc.is_valid(), false);

  PhysxFileStream stream = PhysxFileStream(filename.c_str(), false);
  return _cooking->NxCookConvexMesh(meshDesc.get_desc(), stream);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxKitchen::cook_triangle_mesh
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
bool PhysxKitchen::
cook_triangle_mesh(PhysxTriangleMeshDesc meshDesc, const Filename &filename) {

  nassertr_always(!filename.empty(), false);
  nassertr_always(filename.touch(), false);
  nassertr_always(meshDesc.is_valid(), false);

  PhysxFileStream stream = PhysxFileStream(filename.c_str(), false);
  return _cooking->NxCookTriangleMesh(meshDesc.get_desc(), stream);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxKitchen::cook_convex_mesh
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxConvexMesh) PhysxKitchen::
cook_convex_mesh(PhysxConvexMeshDesc meshDesc) {

  nassertr_always(meshDesc.is_valid(), false);

  PhysxMemoryWriteBuffer buffer;
  bool status = _cooking->NxCookConvexMesh(meshDesc.get_desc(), buffer);
  nassertr(status, NULL);

  NxPhysicsSDK *sdk = NxGetPhysicsSDK();
  nassertr(sdk, NULL);

  PT(PhysxConvexMesh) mesh = new PhysxConvexMesh();
  nassertr(mesh, NULL);

  NxConvexMesh *meshPtr = sdk->createConvexMesh(PhysxMemoryReadBuffer(buffer.data));
  nassertr(meshPtr, NULL);

  mesh->link(meshPtr);

  return mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxKitchen::cook_triangle_mesh
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
PT(PhysxTriangleMesh) PhysxKitchen::
cook_triangle_mesh(PhysxTriangleMeshDesc meshDesc) {

  nassertr_always(meshDesc.is_valid(), false);

  PhysxMemoryWriteBuffer buffer;
  bool status = _cooking->NxCookTriangleMesh(meshDesc.get_desc(), buffer);
  nassertr(status, NULL);

  NxPhysicsSDK *sdk = NxGetPhysicsSDK();
  nassertr(sdk, NULL);

  PT(PhysxTriangleMesh) mesh = new PhysxTriangleMesh();
  nassertr(mesh, NULL);

  NxTriangleMesh *meshPtr = sdk->createTriangleMesh(PhysxMemoryReadBuffer(buffer.data));
  nassertr(meshPtr, NULL);

  mesh->link(meshPtr);

  return mesh;
}

