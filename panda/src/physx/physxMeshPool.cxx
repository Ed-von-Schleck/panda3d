// Filename: physxMeshPool.cxx
// Created by:  enn0x (14Oct09)
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

#include "physxMeshPool.h"
#include "physxConvexMesh.h"
#include "physxTriangleMesh.h"
#include "physxFileStream.h"

PhysxMeshPool::ConvexMeshes PhysxMeshPool::_convex_meshes;
PhysxMeshPool::TriangleMeshes PhysxMeshPool::_triangle_meshes;
//PhysxMeshPool::ClothMeshes PhysxMeshPool::_cloth_meshes;
//PhysxMeshPool::SoftbodyMeshes PhysxMeshPool::_softbody_meshes;

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::prepare_filename
//       Access: Private
//  Description: Checks if the filename is valid, then resolves the
//               filename on the Panda3D model search patch, and
//               finally checks if the filename exists.
////////////////////////////////////////////////////////////////////
bool PhysxMeshPool::
prepare_filename(Filename &fn) {

  if (fn.empty()) {
    // Invalid filename.
    physx_cat.error() << "Invalid filename\n";
    return false;
  }

  fn.resolve_filename(get_model_path());

  if (!fn.exists()) {
    // Non-existent filename.
    physx_cat.error() << "Invalid filename\n";
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::load_convex_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PT(PhysxConvexMesh) PhysxMeshPool::
load_convex_mesh(const Filename &filename) {

  Filename fn(filename);
  if (!prepare_filename(fn)) return NULL;

  PT(PhysxConvexMesh) mesh;

  ConvexMeshes::iterator it = _convex_meshes.find(fn);
  if (it == _convex_meshes.end()) {
    // Not found; load mesh.
    NxConvexMesh *meshPtr;
    PhysxFileStream stream = PhysxFileStream(fn.to_os_specific().c_str(), true);

    mesh = new PhysxConvexMesh();
    nassertr_always(mesh, NULL);

    NxPhysicsSDK *sdk = NxGetPhysicsSDK();
    nassertr_always(sdk, NULL);

    meshPtr = sdk->createConvexMesh(stream);
    nassertr_always(meshPtr, NULL);

    mesh->link(meshPtr);

    _convex_meshes.insert(ConvexMeshes::value_type(fn, mesh));
  }
  else {
    // Found; return previously loaded mesh.
    mesh = (*it).second;
  }

  return mesh;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxMeshPool::load_triangle_mesh
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PT(PhysxTriangleMesh) PhysxMeshPool::
load_triangle_mesh(const Filename &filename) {

  Filename fn(filename);
  if (!prepare_filename(fn)) return NULL;

  PT(PhysxTriangleMesh) mesh;

  TriangleMeshes::iterator it = _triangle_meshes.find(fn);
  if (it == _triangle_meshes.end()) {
    // Not found; load mesh.
    NxTriangleMesh *meshPtr;
    PhysxFileStream stream = PhysxFileStream(fn.to_os_specific().c_str(), true);

    mesh = new PhysxTriangleMesh();
    nassertr_always(mesh, NULL);

    NxPhysicsSDK *sdk = NxGetPhysicsSDK();
    nassertr_always(sdk, NULL);

    meshPtr = sdk->createTriangleMesh(stream);
    nassertr_always(meshPtr, NULL);

    mesh->link(meshPtr);

    _triangle_meshes.insert(TriangleMeshes::value_type(fn, mesh));
  }
  else {
    // Found; return previously loaded mesh.
    mesh = (*it).second;
  }

  return mesh;
}
