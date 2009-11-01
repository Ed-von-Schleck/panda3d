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

  void set_fpu_exceptions(bool b);
  void set_fpu_precision24();
  void set_fpu_precision53();
  void set_fpu_precision64();
  void set_fpu_rounding_chop();
  void set_fpu_rounding_down();
  void set_fpu_rounding_near();
  void set_fpu_rounding_up();

  bool sweep_box_box(const PhysxBox &box0, const PhysxBox &box1, const LVector3f &dir, float length, LPoint3f &ip, LVector3f &normal, float &min_dist);
  bool sweep_box_capsule(const PhysxBox &box, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LVector3f &normal);
  bool sweep_box_sphere(const PhysxBox &box, const PhysxSphere &sphere, const LVector3f &dir, float length, float &min_dist, LVector3f &normal);
  bool sweep_capsule_capsule(const PhysxCapsule &lss0, const PhysxCapsule &lss1, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal);
  bool sweep_sphere_capsule(const PhysxSphere &sphere, const PhysxCapsule &lss, const LVector3f &dir, float length, float &min_dist, LPoint3f &ip, LVector3f &normal);
  bool swept_spheres_intersect(const PhysxSphere &sphere0, const LVector3f &velocity0, const PhysxSphere &sphere1, const LVector3f &velocity1);
  bool tri_box_intersect(const LPoint3f &vertex0, const LPoint3f &vertex1, const LPoint3f &vertex2, const LPoint3f &center, const LVector3f &extents);

  bool box_box_intersect(const LVector3f &extents0, const LPoint3f &center0, const LMatrix3f &rotation0, const LVector3f &extents1, const LPoint3f &center1, const LMatrix3f &rotation1, bool full_test);
  bool box_contains_point(const PhysxBox &box, const LPoint3f &p);

public:
  //const unsigned int *box_vertex_to_quad(unsigned int vertex_index);
  //void compute_bounds(LPoint3f &min, LPoint3f &max, unsigned int nb_verts, const LPoint3f *verts);

  //bool build_smooth_normals(unsigned int nb_tris, unsigned int nb_verts, const LPoint3f *verts, const unsigned int *d_faces, const unsigned short *w_faces, LVector3f *normals, bool flip);



/*
  void compute_box_around_capsule(const PhysxCapsule &capsule, PhysxBox &box);
  float compute_box_density(const LVecBase3f &extents, float mass);
  void compute_box_inertia_tensor(LVecBase3f &diag_inertia, float mass, float xlength, float ylength, float zlength);
  float compute_box_mass(const LVecBase3f &extents, float density);
  bool compute_box_planes(const PhysxBox &box, PhysxPlane *planes);
  bool compute_box_points(const PhysxBox &box, LVecBase3f *pts);
  bool compute_box_vertex_normals(const PhysxBox &box, LVecBase3f *pts);
  void compute_box_world_edge_normal(const PhysxBox &box, unsigned int edge_index, LVecBase3f &world_normal);
  void compute_capsule_around_box(const PhysxBox &box, PhysxCapsule &capsule);
  float compute_cone_density(float radius, float length, float mass);
  float compute_cone_mass(float radius, float length, float density);
  float compute_cylinder_density(float radius, float length, float mass);
  float compute_cylinder_mass(float radius, float length, float density);
  float compute_distance_squared(const PhysxRay &ray, const LVecBase3f &point, float *t);
  float compute_ellipsoid_density(const LVecBase3f &extents, float mass);
  float compute_ellipsoid_mass(const LVecBase3f &extents, float density);
  float compute_sphere_density(float radius, float mass);
  void compute_sphere_inertia_tensor(LVecBase3f &diag_inertia, float mass, float radius, bool hollow);
  float compute_sphere_mass(float radius, float density);
  float compute_square_distance(const PhysxSegment &seg, const LVecBase3f &point, float *t);
  unsigned int crc32(const void *buffer, unsigned int nb_bytes);
  void create_box(PhysxBox &box, const PhysxBounds3 &aabb, const LMatrix4f &mat);
  bool diagonalize_inertia_tensor(const LMatrix3f &dense_inertia, LVecBase3f &diagonal_inertia, LMatrix3f &rotation);
  bool fast_compute_sphere(PhysxSphere &sphere, unsigned nb_verts, const LVecBase3f *verts);
  void find_rotation_matrix(const LVecBase3f &x, const LVecBase3f &b, LMatrix3f &m);
  const unsigned int *get_box_edges();
  const int *get_box_edges_axes();
  const LVecBase3f *get_box_local_edge_normals();
  const unsigned int *get_box_quads();
  const unsigned int *get_box_triangles();
  int int_ceil(const float &f);
  int int_chop(const float &f);
  int int_floor(const float &f);
  bool is_box_a_inside_box_b(const PhysxBox &a, const PhysxBox &b);
  void joint_desc__set_global_anchor(PhysxJointDesc &dis, const LVecBase3f &ws_anchor);
  void joint_desc__set_global_axis(PhysxJointDesc &dis, const LVecBase3f &ws_axis);
  void merge_spheres(PhysxSphere &merged, const PhysxSphere &sphere0, const PhysxSphere &sphere1);
  void normal_to_tangents(const LVecBase3f &n, LVecBase3f &t1, LVecBase3f &t2);
  bool ray_aabb_intersect(const LVecBase3f &min, const LVecBase3f &max, const LVecBase3f &origin, const LVecBase3f &dir, LVecBase3f &coord);
  unsigned int ray_aabb_intersect2(const LVecBase3f &min, const LVecBase3f &max, const LVecBase3f &origin, const LVecBase3f &dir, LVecBase3f &coord, float &t);
  bool ray_obb_intersect(const PhysxRay &ray, const LVecBase3f &center, const LVecBase3f &extents, const LMatrix3f &rot);
  bool ray_plane_intersect(const PhysxRay &ray, const PhysxPlane &plane, float &dist, LVecBase3f &point_on_plane);
  bool ray_sphere_intersect(const LVecBase3f &origin, const LVecBase3f &dir, float length, const LVecBase3f &center, float radius, float &hit_time, LVecBase3f &hit_pos);
  bool ray_tri_intersect(const LVecBase3f &orig, const LVecBase3f &dir, const LVecBase3f &vert0, const LVecBase3f &vert1, const LVecBase3f &vert2, float &t, float &u, float &v, bool cull);
  bool segment_aabb_intersect(const LVecBase3f &p0, const LVecBase3f &p1, const LVecBase3f &min, const LVecBase3f &max);
  bool segment_box_intersect(const LVecBase3f &p1, const LVecBase3f &p2, const LVecBase3f &bbox_min, const LVecBase3f &bbox_max, LVecBase3f &intercept);
  bool segment_obb_intersect(const LVecBase3f &p0, const LVecBase3f &p1, const LVecBase3f &center, const LVecBase3f &extents, const LMatrix3f &rot);
  float segment_obb_sqr_dist(const PhysxSegment &segment, const LVecBase3f &c0, const LVecBase3f &e0, const LMatrix3f &r0, float *t, LVecBase3f *p);
  void segment_plane_intersect(const LVecBase3f &v1, const LVecBase3f &v2, const PhysxPlane &plane, float &dist, LVecBase3f &point_on_plane);
  PhysxSepAxis separating_axis(const LVecBase3f &extents0, const LVecBase3f &center0, const LMatrix3f &rotation0, const LVecBase3f &extents1, const LVecBase3f &center1, const LMatrix3f &rotation1, bool full_test);
*/

public:
  NxUtilLib *_ptr;
};

#include "physxUtilLib.I"

#endif // PHYSUTILLIB_H
