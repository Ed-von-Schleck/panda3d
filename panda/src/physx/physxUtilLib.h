// Filename: physxUtilLib.h
// Created by:  enn0x (01Nov09)
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

#ifndef PHYSXUTILLIB_H
#define PHYSXUTILLIB_H

#include "pandabase.h"
#include "lvector3.h"
#include "lpoint3.h"
#include "lmatrix.h"

#include "config_physx.h"

class PhysxBounds3;
class PhysxBox;
class PhysxCapsule;
class PhysxPlane;
class PhysxRay;
class PhysxSegment;
class PhysxSphere;

////////////////////////////////////////////////////////////////////
//       Class : PhysxUtilLib
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxUtilLib {

PUBLISHED:
  INLINE PhysxUtilLib();
  INLINE ~PhysxUtilLib();

  bool box_contains_point(const PhysxBox &box, const LPoint3f &p);
  PhysxBox create_box(const PhysxBounds3 &aabb, const LMatrix4f &mat);
  LVector3f compute_box_world_edge_normal(const PhysxBox &box, unsigned int edge_index);
  PhysxCapsule compute_capsule_around_box(const PhysxBox &box);
  bool is_box_a_inside_box_b(const PhysxBox &a, const PhysxBox &b);
  PhysxBox compute_box_around_capsule(const PhysxCapsule &capsule);
  void set_fpu_exceptions(bool b);
  void set_fpu_precision24();
  void set_fpu_precision53();
  void set_fpu_precision64();
  void set_fpu_rounding_chop();
  void set_fpu_rounding_down();
  void set_fpu_rounding_near();
  void set_fpu_rounding_up();
  int int_ceil(const float &f);
  int int_chop(const float &f);
  int int_floor(const float &f);
  float compute_distance_squared(const PhysxRay &ray, const LPoint3f &point);
  float compute_square_distance(const PhysxSegment &seg, const LPoint3f &point);
  PhysxSphere merge_spheres(const PhysxSphere &sphere0, const PhysxSphere &sphere1);
  void normal_to_tangents(const LVector3f &n, LVector3f &t1, LVector3f &t2);
  LMatrix3f find_rotation_matrix(const LVector3f &x, const LVector3f &b);
  float compute_sphere_mass(float radius, float density);
  float compute_sphere_density(float radius, float mass);
  float compute_box_mass(const LVector3f &extents, float density);
  float compute_box_density(const LVector3f &extents, float mass);
  float compute_ellipsoid_mass(const LVector3f &extents, float density);
  float compute_ellipsoid_density(const LVector3f &extents, float mass);
  float compute_cylinder_mass(float radius, float length, float density);
  float compute_cylinder_density(float radius, float length, float mass);
  float compute_cone_mass(float radius, float length, float density);
  float compute_cone_density(float radius, float length, float mass);
  LVector3f compute_box_inertia_tensor(float mass, float xlength, float ylength, float zlength);
  LVector3f compute_sphere_inertia_tensor(float mass, float radius, bool hollow);
  bool box_box_intersect(const LVector3f &extents0, const LPoint3f &center0, const LMatrix3f &rotation0, const LVector3f &extents1, const LPoint3f &center1, const LMatrix3f &rotation1, bool full_test);
  bool tri_box_intersect(const LPoint3f &vertex0, const LPoint3f &vertex1, const LPoint3f &vertex2, const LPoint3f &center, const LVector3f &extents);
  bool ray_plane_intersect(const PhysxRay &ray, const PhysxPlane &plane, LPoint3f &point_on_plane);
  bool ray_sphere_intersect(const LPoint3f &origin, const LVector3f &dir, float length, const LPoint3f &center, float radius, LPoint3f &hit_pos);
  bool segment_box_intersect(const LPoint3f &p1, const LPoint3f &p2, const LPoint3f &bbox_min, const LPoint3f &bbox_max, LPoint3f &intercept);
  bool ray_aabb_intersect(const LPoint3f &min, const LPoint3f &max, const LPoint3f &origin, const LVector3f &dir, LPoint3f &coord);
  bool segment_obb_intersect(const LPoint3f &p0, const LPoint3f &p1, const LPoint3f &center, const LVector3f &extents, const LMatrix3f &rot);
  bool segment_aabb_intersect(const LPoint3f &p0, const LPoint3f &p1, const LPoint3f &min, const LPoint3f &max);


/*
NOT YET HANDLED:

bool  NxRayOBBIntersect (const NxRay &ray, const NxVec3 &center, const NxVec3 &extents, const NxMat33 &rot)=0 
NxU32  NxRayCapsuleIntersect (const NxVec3 &origin, const NxVec3 &dir, const NxCapsule &capsule, NxReal t[2])=0 
bool  NxSweptSpheresIntersect (const NxSphere &sphere0, const NxVec3 &velocity0, const NxSphere &sphere1, const NxVec3 &velocity1)=0 
bool  NxRayTriIntersect (const NxVec3 &orig, const NxVec3 &dir, const NxVec3 &vert0, const NxVec3 &vert1, const NxVec3 &vert2, float &t, float &u, float &v, bool cull)=0 
bool  NxBuildSmoothNormals (NxU32 nbTris, NxU32 nbVerts, const NxVec3 *verts, const NxU32 *dFaces, const NxU16 *wFaces, NxVec3 *normals, bool flip=false)=0 
bool  NxSweepBoxCapsule (const NxBox &box, const NxCapsule &lss, const NxVec3 &dir, float length, float &min_dist, NxVec3 &normal)=0 
bool  NxSweepBoxSphere (const NxBox &box, const NxSphere &sphere, const NxVec3 &dir, float length, float &min_dist, NxVec3 &normal)=0 
bool  NxSweepCapsuleCapsule (const NxCapsule &lss0, const NxCapsule &lss1, const NxVec3 &dir, float length, float &min_dist, NxVec3 &ip, NxVec3 &normal)=0 
bool  NxSweepSphereCapsule (const NxSphere &sphere, const NxCapsule &lss, const NxVec3 &dir, float length, float &min_dist, NxVec3 &ip, NxVec3 &normal)=0 
bool  NxSweepBoxBox (const NxBox &box0, const NxBox &box1, const NxVec3 &dir, float length, NxVec3 &ip, NxVec3 &normal, float &min_dist)=0 
bool  NxSweepBoxTriangles (NxU32 nb_tris, const NxTriangle *triangles, const NxTriangle *edge_triangles, const NxU32 *edge_flags, const NxBounds3 &box, const NxVec3 &dir, float length, NxVec3 &hit, NxVec3 &normal, float &d, NxU32 &index, NxU32 *cachedIndex=NULL)=0 
bool  NxSweepCapsuleTriangles (NxU32 up_direction, NxU32 nb_tris, const NxTriangle *triangles, const NxU32 *edge_flags, const NxVec3 &center, const float radius, const float height, const NxVec3 &dir, float length, NxVec3 &hit, NxVec3 &normal, float &d, NxU32 &index, NxU32 *cachedIndex=NULL)=0 
float  NxPointOBBSqrDist (const NxVec3 &point, const NxVec3 &center, const NxVec3 &extents, const NxMat33 &rot, NxVec3 *params)=0 
float  NxSegmentOBBSqrDist (const NxSegment &segment, const NxVec3 &c0, const NxVec3 &e0, const NxMat33 &r0, float *t, NxVec3 *p)=0 
*/



/*
PROBLEMS:

bool NxComputeBoxPlanes(const NxBox &box, NxPlane *planes)
bool NxComputeBoxPoints(const NxBox &box, NxVec3 *pts)
bool NxComputeBoxVertexNormals(const NxBox &box, NxVec3 *pts)
const NxU32 *NxGetBoxEdges()
const NxI32 *NxGetBoxEdgesAxes()
const NxU32 *NxGetBoxTriangles()
const NxVec3 *NxGetBoxLocalEdgeNormals()
const NxU32 *NxGetBoxQuads()
const NxU32 *NxBoxVertexToQuad(NxU32 vertexIndex)
NxBSphereMethod NxComputeSphere(NxSphere &sphere, unsigned nb_verts, const NxVec3 *verts)
bool NxFastComputeSphere(NxSphere &sphere, unsigned nb_verts, const NxVec3 *verts)
bool NxDiagonalizeInertiaTensor(const NxMat33 &denseInertia, NxVec3 &diagonalInertia, NxMat33 &rotation)
void NxComputeBounds(NxVec3 &min, NxVec3 &max, NxU32 nbVerts, const NxVec3 *verts)
NxU32 NxCrc32(const void *buffer, NxU32 nbBytes)
NxSepAxis NxSeparatingAxis(const NxVec3 &extents0, const NxVec3 &center0, const NxMat33 &rotation0, const NxVec3 &extents1, const NxVec3 &center1, const NxMat33 &rotation1, bool fullTest=true)
void NxSegmentPlaneIntersect(const NxVec3 &v1, const NxVec3 &v2, const NxPlane &plane, NxReal &dist, NxVec3 &pointOnPlane)
NxU32 NxRayAABBIntersect2(const NxVec3 &min, const NxVec3 &max, const NxVec3 &origin, const NxVec3 &dir, NxVec3 &coord, NxReal &t)
*/













  //bool sweep_box_box(const PhysxBox &box0, const PhysxBox &box1, const LVector3f &dir, float length, LPoint3f &ip, LVector3f &normal, float &min_dist);
  //bool sweep_box_capsule(const PhysxBox &box, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LVector3f &normal);
  //bool sweep_box_sphere(const PhysxBox &box, const PhysxSphere &sphere, const LVector3f &dir, float length, float &min_dist, LVector3f &normal);
  //bool sweep_capsule_capsule(const PhysxCapsule &lss0, const PhysxCapsule &lss1, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal);
  //bool sweep_sphere_capsule(const PhysxSphere &sphere, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal);
  //bool swept_spheres_intersect(const PhysxSphere &sphere0, const LVector3f &velocity0, const PhysxSphere &sphere1, const LVector3f &velocity1);
  //unsigned int ray_aabb_intersect2(const LPoint3f &min, const LPoint3f &max, const LPoint3f &origin, const LVector3f &dir, LPoint3f &coord, float &t);
  //bool ray_obb_intersect(const PhysxRay &ray, const LPoint3f &center, const LVector3f &extents, const LMatrix3f &rot);
  //bool ray_tri_intersect(const LPoint3f &orig, const LVector3f &dir, const LPoint3f &vert0, const LPoint3f &vert1, const LPoint3f &vert2, float &t, float &u, float &v, bool cull);
  //const unsigned int *box_vertex_to_quad(unsigned int vertex_index);
  //void compute_bounds(LPoint3f &min, LPoint3f &max, unsigned int nb_verts, const LPoint3f *verts);
  //bool build_smooth_normals(unsigned int nb_tris, unsigned int nb_verts, const LPoint3f *verts, const unsigned int *d_faces, const unsigned short *w_faces, LVector3f *normals, bool flip);
  //float segment_obb_sqr_dist(const PhysxSegment &segment, const LPoint3f &c0, const LVector3f &e0, const LMatrix3f &r0, float *t, LPoint3f *p);

public:
  NxUtilLib *_ptr;
};

#include "physxUtilLib.I"

#endif // PHYSUTILLIB_H
