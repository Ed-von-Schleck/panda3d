// Filename: physxKitchen.h
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

#ifndef PHYSXKITCHEN_H
#define PHYSXKITCHEN_H

#include "pandabase.h"
#include "filename.h"

#include "NoMinMax.h"
#include "NxPhysics.h"
#include "NxCooking.h"

class PhysxConvexMesh;
class PhysxConvexMeshDesc;
class PhysxTriangleMesh;
class PhysxTriangleMeshDesc;

////////////////////////////////////////////////////////////////////
//       Class : PhysxKitchen
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxKitchen {

PUBLISHED:
  INLINE PhysxKitchen();
  INLINE ~PhysxKitchen();

  bool cook_convex_mesh(PhysxConvexMeshDesc meshDesc, const Filename &filename);
  bool cook_triangle_mesh(PhysxTriangleMeshDesc meshDesc, const Filename &filename);

  PT(PhysxConvexMesh) cook_convex_mesh(PhysxConvexMeshDesc meshDesc);
  PT(PhysxTriangleMesh) cook_triangle_mesh(PhysxTriangleMeshDesc meshDesc);

private:
  NxCookingInterface *_cooking;
};

#include "physxKitchen.I"

#endif // PHYSXKITCHEN_H
