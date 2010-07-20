// Filename: recastNavMesh.h
// Created by:  rdb (20Jul10)
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

#ifndef RECASTNAVMESH_H
#define RECASTNAVMESH_H
#include "config_navigation.h"

////////////////////////////////////////////////////////////////////
//       Class : RecastNavMesh
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_NAVIGATION RecastNavMesh {
PUBLISHED:
  RecastNavMesh();
  INLINE float get_cell_size() const;
  INLINE void set_cell_size(float cell_size);
  INLINE float get_cell_height() const;
  INLINE void set_cell_height(float cell_height);
  INLINE float get_walkable_slope_angle() const;
  INLINE void set_walkable_slope_angle(float walkable_slope_angle);
  INLINE float get_walkable_height() const;
  INLINE void set_walkable_height(float walkable_climb);
  INLINE float get_walkable_climb() const;
  INLINE void set_walkable_climb(float walkable_climb);
  INLINE float get_walkable_radius() const;
  INLINE void set_walkable_radius(float walkable_radius);
  INLINE float get_max_edge_length() const;
  INLINE void set_max_edge_length(float max_edge_length);
  INLINE float get_max_simplification_error() const;
  INLINE void set_max_simplification_error(float max_simplification_error);
  INLINE float get_min_region_size() const;
  INLINE void set_min_region_size(float min_region_size);
  INLINE float get_merge_region_size() const;
  INLINE void set_merge_region_size(float merge_region_size);
  INLINE float get_detail_sample_distance() const;
  INLINE void set_detail_sample_distance(float detail_sample_distance);
  INLINE float get_detail_sample_max_error() const;
  INLINE void set_detail_sample_max_error(float detail_sample_max_error);

private:
  float _cell_size;
  float _cell_height;
  float _walkable_slope_angle;
  float _walkable_height;
  float _walkable_climb;
  float _walkable_radius;
  float _max_edge_length;
  float _max_simplification_error;
  float _min_region_size;
  float _merge_region_size;
  float _detail_sample_distance;
  float _detail_sample_max_error;
};

#include "recastNavMesh.I"

#endif

