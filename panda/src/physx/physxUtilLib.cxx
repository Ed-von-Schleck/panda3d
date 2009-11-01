// Filename: physxUtilLib.cxx
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

#include "physxUtilLib.h"
#include "physxManager.h"
#include "physxBounds3.h"
#include "physxBox.h"
#include "physxCapsule.h"
#include "physxPlane.h"
#include "physxRay.h"
#include "physxSegment.h"
#include "physxSphere.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_exceptions
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_exceptions(bool b) {

  _ptr->NxSetFPUExceptions(b);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_precision24
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_precision24() {

  _ptr->NxSetFPUPrecision24();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_precision53
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_precision53() {

  _ptr->NxSetFPUPrecision53();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_precision64
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_precision64() {

  _ptr->NxSetFPUPrecision64();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_rounding_chop
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_rounding_chop() {

  _ptr->NxSetFPURoundingChop();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_rounding_down
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_rounding_down() {

  _ptr->NxSetFPURoundingDown();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_rounding_near
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_rounding_near() {

  _ptr->NxSetFPURoundingNear();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::set_fpu_rounding_up
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
set_fpu_rounding_up() {

  _ptr->NxSetFPURoundingUp();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::sweep_box_box
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
sweep_box_box(const PhysxBox &box0, const PhysxBox &box1, const LVector3f &dir, float length, LPoint3f &ip, LVector3f &normal, float &min_dist) {

  nassertr_always(!dir.is_nan(), false);
  nassertr_always(!ip.is_nan(), false);
  nassertr_always(!normal.is_nan(), false);

  return _ptr->NxSweepBoxBox(box0._box, box1._box,
                             PhysxManager::vec3_to_nxVec3(dir),
                             length,
                             PhysxManager::point3_to_nxVec3(ip),
                             PhysxManager::vec3_to_nxVec3(normal),
                             min_dist);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::sweep_box_capsule
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
sweep_box_capsule(const PhysxBox &box, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LVector3f &normal) {

  nassertr_always(!dir.is_nan(), false);
  nassertr_always(!normal.is_nan(), false);

  return _ptr->NxSweepBoxCapsule(box._box, lss._capsule,
                                 PhysxManager::vec3_to_nxVec3(dir),
                                 length, min_dist,
                                 PhysxManager::vec3_to_nxVec3(normal));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::sweep_box_sphere
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
sweep_box_sphere(const PhysxBox &box, const PhysxSphere &sphere, const LVector3f &dir, float length, float &min_dist, LVector3f &normal) {

  nassertr_always(!dir.is_nan(), false);
  nassertr_always(!normal.is_nan(), false);

  return _ptr->NxSweepBoxSphere(box._box, sphere._sphere,
                                PhysxManager::vec3_to_nxVec3(dir),
                                length, min_dist,
                                PhysxManager::vec3_to_nxVec3(normal));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::sweep_capsule_capsule
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
sweep_capsule_capsule(const PhysxCapsule &lss0, const PhysxCapsule &lss1, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal) {

  nassertr_always(!dir.is_nan(), false);
  nassertr_always(!ip.is_nan(), false);
  nassertr_always(!normal.is_nan(), false);

  return _ptr->NxSweepCapsuleCapsule(lss0._capsule, lss1._capsule,
                                     PhysxManager::vec3_to_nxVec3(dir),
                                     length, min_dist,
                                     PhysxManager::point3_to_nxVec3(ip),
                                     PhysxManager::vec3_to_nxVec3(normal));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::sweep_sphere_capsule
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
sweep_sphere_capsule(const PhysxSphere &sphere, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal) {

  nassertr_always(!dir.is_nan(), false);
  nassertr_always(!ip.is_nan(), false);
  nassertr_always(!normal.is_nan(), false);

  return _ptr->NxSweepSphereCapsule(sphere._sphere, lss._capsule,
                                    PhysxManager::vec3_to_nxVec3(dir),
                                    length, min_dist,
                                    PhysxManager::point3_to_nxVec3(ip),
                                    PhysxManager::vec3_to_nxVec3(normal));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::swept_spheres_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
swept_spheres_intersect(const PhysxSphere &sphere0, const LVector3f &velocity0, const PhysxSphere &sphere1, const LVector3f &velocity1) {

  nassertr_always(!velocity0.is_nan(), false);
  nassertr_always(!velocity1.is_nan(), false);

  return _ptr->NxSweptSpheresIntersect(sphere0._sphere,
                                       PhysxManager::vec3_to_nxVec3(velocity0),
                                       sphere1._sphere,
                                       PhysxManager::vec3_to_nxVec3(velocity1));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::tri_box_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
tri_box_intersect(const LPoint3f &vertex0, const LPoint3f &vertex1, const LPoint3f &vertex2, const LPoint3f &center, const LVector3f &extents) {

  nassertr_always(!vertex0.is_nan(), false);
  nassertr_always(!vertex1.is_nan(), false);
  nassertr_always(!vertex2.is_nan(), false);
  nassertr_always(!center.is_nan(), false);
  nassertr_always(!extents.is_nan(), false);

  return _ptr->NxTriBoxIntersect(PhysxManager::point3_to_nxVec3(vertex0),
                                 PhysxManager::point3_to_nxVec3(vertex1),
                                 PhysxManager::point3_to_nxVec3(vertex2),
                                 PhysxManager::point3_to_nxVec3(center),
                                 PhysxManager::point3_to_nxVec3(extents));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::box_box_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
box_box_intersect(const LVector3f &extents0, const LPoint3f &center0, const LMatrix3f &rotation0, const LVector3f &extents1, const LPoint3f &center1, const LMatrix3f &rotation1, bool full_test) {

  nassertr_always(!extents0.is_nan(), false);
  nassertr_always(!center0.is_nan(), false);
  nassertr_always(!rotation0.is_nan(), false);
  nassertr_always(!extents1.is_nan(), false);
  nassertr_always(!center1.is_nan(), false);
  nassertr_always(!rotation1.is_nan(), false);

  return _ptr->NxBoxBoxIntersect(PhysxManager::vec3_to_nxVec3(extents0),
                                 PhysxManager::point3_to_nxVec3(center0),
                                 PhysxManager::mat3_to_nxMat33(rotation0),
                                 PhysxManager::vec3_to_nxVec3(extents1),
                                 PhysxManager::point3_to_nxVec3(center1),
                                 PhysxManager::mat3_to_nxMat33(rotation1), full_test);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::box_contains_point
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
box_contains_point(const PhysxBox &box, const LPoint3f &p) {

  return _ptr->NxBoxContainsPoint(box._box, PhysxManager::point3_to_nxVec3(p));
}





















/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::box_vertex_to_quad
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
const unsigned int *PhysxUtilLib::
box_vertex_to_quad(unsigned int vertex_index) {

  return _ptr->NxBoxVertexToQuad(vertex_index);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_bounds
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_bounds(LPoint3f &min, LPoint3f &max, unsigned int nb_verts, const LPoint3f *verts) {

  _ptr->NxComputeBounds(PhysxManager::point3_to_nxVec3(min), PhysxManager::point3_to_nxVec3(max), nb_verts, &PhysxManager::point3_to_nxVec3(*verts));
}
*/

/*
////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::build_smooth_normals
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
build_smooth_normals(unsigned int nb_tris, unsigned int nb_verts, const LPoint3f *verts, const unsigned int *d_faces, const unsigned short *w_faces, LVector3f *normals, bool flip) {

  return _ptr->NxBuildSmoothNormals(nb_tris, nb_verts, &PhysxManager::lVecBase3_to_nxVec3(*verts), d_faces, w_faces, &PhysxManager::lVecBase3_to_nxVec3(*normals), flip);
}
*/


/*

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_around_capsule
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_box_around_capsule(const PhysxCapsule &capsule, PhysxBox &box) {

  _ptr->NxComputeBoxAroundCapsule(*(capsule.nCapsule), *(box.nBox));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_density
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_box_density(const LVecBase3f &extents, float mass) {

  return _ptr->NxComputeBoxDensity(PhysxManager::lVecBase3_to_nxVec3(extents), mass);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_inertia_tensor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_box_inertia_tensor(LVecBase3f &diag_inertia, float mass, float xlength, float ylength, float zlength) {

  _ptr->NxComputeBoxInertiaTensor(PhysxManager::lVecBase3_to_nxVec3(diag_inertia), mass, xlength, ylength, zlength);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_mass
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_box_mass(const LVecBase3f &extents, float density) {

  return _ptr->NxComputeBoxMass(PhysxManager::lVecBase3_to_nxVec3(extents), density);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_planes
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
compute_box_planes(const PhysxBox &box, PhysxPlane *planes) {

  return _ptr->NxComputeBoxPlanes(*(box.nBox), planes->nPlane);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_points
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
compute_box_points(const PhysxBox &box, LVecBase3f *pts) {

  return _ptr->NxComputeBoxPoints(*(box.nBox), &PhysxManager::lVecBase3_to_nxVec3(*pts));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_vertex_normals
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
compute_box_vertex_normals(const PhysxBox &box, LVecBase3f *pts) {

  return _ptr->NxComputeBoxVertexNormals(*(box.nBox), &PhysxManager::lVecBase3_to_nxVec3(*pts));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_box_world_edge_normal
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_box_world_edge_normal(const PhysxBox &box, unsigned int edge_index, LVecBase3f &world_normal) {

  _ptr->NxComputeBoxWorldEdgeNormal(*(box.nBox), edge_index, PhysxManager::lVecBase3_to_nxVec3(world_normal));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_capsule_around_box
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_capsule_around_box(const PhysxBox &box, PhysxCapsule &capsule) {

  _ptr->NxComputeCapsuleAroundBox(*(box.nBox), *(capsule.nCapsule));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_cone_density
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_cone_density(float radius, float length, float mass) {

  return _ptr->NxComputeConeDensity(radius, length, mass);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_cone_mass
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_cone_mass(float radius, float length, float density) {

  return _ptr->NxComputeConeMass(radius, length, density);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_cylinder_density
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_cylinder_density(float radius, float length, float mass) {

  return _ptr->NxComputeCylinderDensity(radius, length, mass);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_cylinder_mass
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_cylinder_mass(float radius, float length, float density) {

  return _ptr->NxComputeCylinderMass(radius, length, density);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_distance_squared
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_distance_squared(const PhysxRay &ray, const LVecBase3f &point, float *t) {

  return _ptr->NxComputeDistanceSquared(*(ray.nRay), PhysxManager::lVecBase3_to_nxVec3(point), t);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_ellipsoid_density
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_ellipsoid_density(const LVecBase3f &extents, float mass) {

  return _ptr->NxComputeEllipsoidDensity(PhysxManager::lVecBase3_to_nxVec3(extents), mass);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_ellipsoid_mass
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_ellipsoid_mass(const LVecBase3f &extents, float density) {

  return _ptr->NxComputeEllipsoidMass(PhysxManager::lVecBase3_to_nxVec3(extents), density);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_sphere_density
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_sphere_density(float radius, float mass) {

  return _ptr->NxComputeSphereDensity(radius, mass);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_sphere_inertia_tensor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
compute_sphere_inertia_tensor(LVecBase3f &diag_inertia, float mass, float radius, bool hollow) {

  _ptr->NxComputeSphereInertiaTensor(PhysxManager::lVecBase3_to_nxVec3(diag_inertia), mass, radius, hollow);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_sphere_mass
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_sphere_mass(float radius, float density) {

  return _ptr->NxComputeSphereMass(radius, density);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::compute_square_distance
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
compute_square_distance(const PhysxSegment &seg, const LVecBase3f &point, float *t) {

  return _ptr->NxComputeSquareDistance(*(seg.nSegment), PhysxManager::lVecBase3_to_nxVec3(point), t);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::crc32
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
unsigned int PhysxUtilLib::
crc32(const void *buffer, unsigned int nb_bytes) {

  return _ptr->NxCrc32(buffer, nb_bytes);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::create_box
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
create_box(PhysxBox &box, const PhysxBounds3 &aabb, const LMatrix4f &mat) {

  _ptr->NxCreateBox(*(box.nBox), *(aabb.nBounds3), PhysxManager::lMatrix4_to_nxMat34(mat));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::diagonalize_inertia_tensor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
diagonalize_inertia_tensor(const LMatrix3f &dense_inertia, LVecBase3f &diagonal_inertia, LMatrix3f &rotation) {

  return _ptr->NxDiagonalizeInertiaTensor(PhysxManager::lMatrix3_to_nxMat33(dense_inertia), PhysxManager::lVecBase3_to_nxVec3(diagonal_inertia), PhysxManager::lMatrix3_to_nxMat33(rotation));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::fast_compute_sphere
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
fast_compute_sphere(PhysxSphere &sphere, unsigned nb_verts, const LVecBase3f *verts) {

  return _ptr->NxFastComputeSphere(*(sphere.nSphere), nb_verts, &PhysxManager::lVecBase3_to_nxVec3(*verts));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::find_rotation_matrix
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
find_rotation_matrix(const LVecBase3f &x, const LVecBase3f &b, LMatrix3f &m) {

  _ptr->NxFindRotationMatrix(PhysxManager::lVecBase3_to_nxVec3(x), PhysxManager::lVecBase3_to_nxVec3(b), PhysxManager::lMatrix3_to_nxMat33(m));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::get_box_edges
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
const unsigned int *PhysxUtilLib::
get_box_edges() {

  return _ptr->NxGetBoxEdges();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::get_box_edges_axes
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
const int *PhysxUtilLib::
get_box_edges_axes() {

  return _ptr->NxGetBoxEdgesAxes();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::get_box_local_edge_normals
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
const LVecBase3f *PhysxUtilLib::
get_box_local_edge_normals() {

  return &PhysxManager::nxVec3_to_lVecBase3(*_ptr->NxGetBoxLocalEdgeNormals());
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::get_box_quads
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
const unsigned int *PhysxUtilLib::
get_box_quads() {

  return _ptr->NxGetBoxQuads();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::get_box_triangles
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
const unsigned int *PhysxUtilLib::
get_box_triangles() {

  return _ptr->NxGetBoxTriangles();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::int_ceil
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
int PhysxUtilLib::
int_ceil(const float &f) {

  return _ptr->NxIntCeil(f);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::int_chop
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
int PhysxUtilLib::
int_chop(const float &f) {

  return _ptr->NxIntChop(f);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::int_floor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
int PhysxUtilLib::
int_floor(const float &f) {

  return _ptr->NxIntFloor(f);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::is_box_a_inside_box_b
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
is_box_a_inside_box_b(const PhysxBox &a, const PhysxBox &b) {

  return _ptr->NxIsBoxAInsideBoxB(*(a.nBox), *(b.nBox));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::joint_desc__set_global_anchor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
joint_desc__set_global_anchor(PhysxJointDesc &dis, const LVecBase3f &ws_anchor) {

  _ptr->NxJointDesc_SetGlobalAnchor(*(dis.nJointDesc), PhysxManager::lVecBase3_to_nxVec3(ws_anchor));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::joint_desc__set_global_axis
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
joint_desc__set_global_axis(PhysxJointDesc &dis, const LVecBase3f &ws_axis) {

  _ptr->NxJointDesc_SetGlobalAxis(*(dis.nJointDesc), PhysxManager::lVecBase3_to_nxVec3(ws_axis));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::merge_spheres
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
merge_spheres(PhysxSphere &merged, const PhysxSphere &sphere0, const PhysxSphere &sphere1) {

  _ptr->NxMergeSpheres(*(merged.nSphere), *(sphere0.nSphere), *(sphere1.nSphere));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::normal_to_tangents
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
normal_to_tangents(const LVecBase3f &n, LVecBase3f &t1, LVecBase3f &t2) {

  _ptr->NxNormalToTangents(PhysxManager::lVecBase3_to_nxVec3(n), PhysxManager::lVecBase3_to_nxVec3(t1), PhysxManager::lVecBase3_to_nxVec3(t2));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_aabb_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
ray_aabb_intersect(const LVecBase3f &min, const LVecBase3f &max, const LVecBase3f &origin, const LVecBase3f &dir, LVecBase3f &coord) {

  return _ptr->NxRayAABBIntersect(PhysxManager::lVecBase3_to_nxVec3(min), PhysxManager::lVecBase3_to_nxVec3(max), PhysxManager::lVecBase3_to_nxVec3(origin), PhysxManager::lVecBase3_to_nxVec3(dir), PhysxManager::lVecBase3_to_nxVec3(coord));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_aabb_intersect2
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
unsigned int PhysxUtilLib::
ray_aabb_intersect2(const LVecBase3f &min, const LVecBase3f &max, const LVecBase3f &origin, const LVecBase3f &dir, LVecBase3f &coord, float &t) {

  return _ptr->NxRayAABBIntersect2(PhysxManager::lVecBase3_to_nxVec3(min), PhysxManager::lVecBase3_to_nxVec3(max), PhysxManager::lVecBase3_to_nxVec3(origin), PhysxManager::lVecBase3_to_nxVec3(dir), PhysxManager::lVecBase3_to_nxVec3(coord), t);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_obb_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
ray_obb_intersect(const PhysxRay &ray, const LVecBase3f &center, const LVecBase3f &extents, const LMatrix3f &rot) {

  return _ptr->NxRayOBBIntersect(*(ray.nRay), PhysxManager::lVecBase3_to_nxVec3(center), PhysxManager::lVecBase3_to_nxVec3(extents), PhysxManager::lMatrix3_to_nxMat33(rot));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_plane_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
ray_plane_intersect(const PhysxRay &ray, const PhysxPlane &plane, float &dist, LVecBase3f &point_on_plane) {

  return _ptr->NxRayPlaneIntersect(*(ray.nRay), *(plane.nPlane), dist, PhysxManager::lVecBase3_to_nxVec3(point_on_plane));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_sphere_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
ray_sphere_intersect(const LVecBase3f &origin, const LVecBase3f &dir, float length, const LVecBase3f &center, float radius, float &hit_time, LVecBase3f &hit_pos) {

  return _ptr->NxRaySphereIntersect(PhysxManager::lVecBase3_to_nxVec3(origin), PhysxManager::lVecBase3_to_nxVec3(dir), length, PhysxManager::lVecBase3_to_nxVec3(center), radius, hit_time, PhysxManager::lVecBase3_to_nxVec3(hit_pos));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::ray_tri_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
ray_tri_intersect(const LVecBase3f &orig, const LVecBase3f &dir, const LVecBase3f &vert0, const LVecBase3f &vert1, const LVecBase3f &vert2, float &t, float &u, float &v, bool cull) {

  return _ptr->NxRayTriIntersect(PhysxManager::lVecBase3_to_nxVec3(orig), PhysxManager::lVecBase3_to_nxVec3(dir), PhysxManager::lVecBase3_to_nxVec3(vert0), PhysxManager::lVecBase3_to_nxVec3(vert1), PhysxManager::lVecBase3_to_nxVec3(vert2), t, u, v, cull);
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::segment_aabb_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
segment_aabb_intersect(const LVecBase3f &p0, const LVecBase3f &p1, const LVecBase3f &min, const LVecBase3f &max) {

  return _ptr->NxSegmentAABBIntersect(PhysxManager::lVecBase3_to_nxVec3(p0), PhysxManager::lVecBase3_to_nxVec3(p1), PhysxManager::lVecBase3_to_nxVec3(min), PhysxManager::lVecBase3_to_nxVec3(max));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::segment_box_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
segment_box_intersect(const LVecBase3f &p1, const LVecBase3f &p2, const LVecBase3f &bbox_min, const LVecBase3f &bbox_max, LVecBase3f &intercept) {

  return _ptr->NxSegmentBoxIntersect(PhysxManager::lVecBase3_to_nxVec3(p1), PhysxManager::lVecBase3_to_nxVec3(p2), PhysxManager::lVecBase3_to_nxVec3(bbox_min), PhysxManager::lVecBase3_to_nxVec3(bbox_max), PhysxManager::lVecBase3_to_nxVec3(intercept));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::segment_obb_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
bool PhysxUtilLib::
segment_obb_intersect(const LVecBase3f &p0, const LVecBase3f &p1, const LVecBase3f &center, const LVecBase3f &extents, const LMatrix3f &rot) {

  return _ptr->NxSegmentOBBIntersect(PhysxManager::lVecBase3_to_nxVec3(p0), PhysxManager::lVecBase3_to_nxVec3(p1), PhysxManager::lVecBase3_to_nxVec3(center), PhysxManager::lVecBase3_to_nxVec3(extents), PhysxManager::lMatrix3_to_nxMat33(rot));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::segment_obb_sqr_dist
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
float PhysxUtilLib::
segment_obb_sqr_dist(const PhysxSegment &segment, const LVecBase3f &c0, const LVecBase3f &e0, const LMatrix3f &r0, float *t, LVecBase3f *p) {

  return _ptr->NxSegmentOBBSqrDist(*(segment.nSegment), PhysxManager::lVecBase3_to_nxVec3(c0), PhysxManager::lVecBase3_to_nxVec3(e0), PhysxManager::lMatrix3_to_nxMat33(r0), t, &PhysxManager::lVecBase3_to_nxVec3(*p));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::segment_plane_intersect
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void PhysxUtilLib::
segment_plane_intersect(const LVecBase3f &v1, const LVecBase3f &v2, const PhysxPlane &plane, float &dist, LVecBase3f &point_on_plane) {

  _ptr->NxSegmentPlaneIntersect(PhysxManager::lVecBase3_to_nxVec3(v1), PhysxManager::lVecBase3_to_nxVec3(v2), *(plane.nPlane), dist, PhysxManager::lVecBase3_to_nxVec3(point_on_plane));
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxUtilLib::separating_axis
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
PhysxSepAxis PhysxUtilLib::
separating_axis(const LVecBase3f &extents0, const LVecBase3f &center0, const LMatrix3f &rotation0, const LVecBase3f &extents1, const LVecBase3f &center1, const LMatrix3f &rotation1, bool full_test) {
  return (PhysxSepAxis)_ptr->NxSeparatingAxis(PhysxManager::lVecBase3_to_nxVec3(extents0), PhysxManager::lVecBase3_to_nxVec3(center0), PhysxManager::lMatrix3_to_nxMat33(rotation0), PhysxManager::lVecBase3_to_nxVec3(extents1), PhysxManager::lVecBase3_to_nxVec3(center1), PhysxManager::lMatrix3_to_nxMat33(rotation1), full_test);
}
*/
