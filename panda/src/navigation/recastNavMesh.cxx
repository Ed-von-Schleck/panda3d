// Filename: recastNavMesh.cxx
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

#include "recastNavMesh.h"
#include "dcast.h"
#include "coordinateSystem.h"
#include "geomNode.h"
#include "geomPrimitive.h"
#include "geomTriangles.h"
#include "geomVertexReader.h"

#include <Recast.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshBuilder.h>

TypeHandle RecastNavMesh::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
RecastNavMesh::
RecastNavMesh() {
  _cell_size = 0.3;
  _cell_height = 0.2;
  _walkable_slope_angle = 45;
  _walkable_height = 1;
  _walkable_climb = 1;
  _walkable_radius = 1;
  _max_edge_length = 12;
  _max_simplification_error = 1.3;
  _min_region_size = 0;
  _merge_region_size = 0;
  _max_verts_per_poly = 6;
  _detail_sample_distance = 0;
  _detail_sample_max_error = 0;

  _source = NULL;
  _node = new DetourNavMeshNode("");
}

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::rasterize_r
//       Access: Private
//  Description: Recursively rasterizes triangles from the specified
//               node.
////////////////////////////////////////////////////////////////////
void RecastNavMesh::
rasterize_r(rcHeightfield &heightfield, CPT(PandaNode) node, LMatrix4f xform) const {
  xform = node->get_transform()->get_mat() * xform;
  if (node->is_geom_node()) {
    // Calculate this now, to avoid doing this more often than necessary.
    const float threshold = cosf(_walkable_slope_angle / 180.0f * (float) M_PI);
    const LMatrix4f &conv = LMatrix4f::convert_mat(CS_default, CS_yup_right);

    CPT(GeomNode) gnode = DCAST(GeomNode, node);
    for (size_t g = 0; g < gnode->get_num_geoms(); ++g) {
      PT(Geom) geom = gnode->get_geom(g)->decompose();
      geom->transform_vertices(xform);
      GeomVertexReader reader (geom->get_vertex_data(), InternalName::get_vertex());
      for (size_t p = 0; p < geom->get_num_primitives(); ++p) {
        CPT(GeomPrimitive) prim = geom->get_primitive(p);
        if (!prim->is_of_type(GeomTriangles::get_class_type())) {
          continue;
        }
        for (size_t pr = 0; pr < prim->get_num_primitives(); ++pr) {
          int start = prim->get_primitive_start(pr);
          nassertd(start + 3 == prim->get_primitive_end(pr)) continue;
          reader.set_row(prim->get_vertex(start));
          LPoint3f vtx1 = conv.xform_point(reader.get_data3f());
          reader.set_row(prim->get_vertex(start + 1));
          LPoint3f vtx2 = conv.xform_point(reader.get_data3f());
          reader.set_row(prim->get_vertex(start + 2));
          LPoint3f vtx3 = conv.xform_point(reader.get_data3f());

          // Figure out if this triangle is walkable, based on the slope
          unsigned char area = 0;
          float e0[3], e1[3], norm[3];
          rcVsub(e0, vtx2._v.data, vtx1._v.data);
          rcVsub(e1, vtx3._v.data, vtx1._v.data);
          rcVcross(norm, e0, e1);
          rcVnormalize(norm);
          if (norm[1] > threshold) {
            area = RC_WALKABLE_AREA;
          }
          rcRasterizeTriangle(vtx1._v.data, vtx2._v.data, vtx3._v.data,
             area, heightfield, (int) floorf(_walkable_climb / _cell_height));
        }
      }
    }
  }

  for (size_t c = 0; c < node->get_num_children(); ++c) {
    rasterize_r(heightfield, node->get_child(c), xform);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::config
//       Access: Published
//  Description: Sets configuration variables.
////////////////////////////////////////////////////////////////////
rcConfig RecastNavMesh::
configure() const {
  rcConfig config;

  memset(&config, 0, sizeof(config));

  config.cs = _cell_size;
  config.ch = _cell_height;
  config.walkableSlopeAngle = _walkable_slope_angle;
  config.walkableHeight = (int)ceilf(_walkable_height / config.ch);
  config.walkableClimb = (int)floorf(_walkable_climb / config.ch);
  config.walkableRadius = (int)ceilf(_walkable_radius / config.cs);
  config.maxEdgeLen = (int)(_max_edge_length / _cell_size);
  config.maxSimplificationError = _max_simplification_error;
  config.minRegionSize = (int)rcSqr(_min_region_size);
  config.mergeRegionSize = (int)rcSqr(_merge_region_size);
  config.maxVertsPerPoly = (int)_max_verts_per_poly;
  min(config.maxVertsPerPoly, DT_VERTS_PER_POLYGON);
  config.detailSampleDist = _detail_sample_distance < 0.9f ? 0 : _cell_size * _detail_sample_distance;
  config.detailSampleMaxError = _cell_height * _detail_sample_max_error;

  LPoint3f bmin, bmax;
  bool found = false;
  _source->calc_tight_bounds(bmin, bmax, found, TransformState::make_identity(), Thread::get_current_thread());

  const LMatrix4f &conv = LMatrix4f::convert_mat(CS_default, CS_yup_right);
  bmin = conv.xform_point(bmin);
  bmax = conv.xform_point(bmax);

  config.bmin[0] = min(bmin[0], bmax[0]);
  config.bmin[1] = min(bmin[1], bmax[1]);
  config.bmin[2] = min(bmin[2], bmax[2]);
  config.bmax[0] = max(bmin[0], bmax[0]);
  config.bmax[1] = max(bmin[1], bmax[1]);
  config.bmax[2] = max(bmin[2], bmax[2]);
  rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);

  return config;
}

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::build
//       Access: Published
//  Description: Builds the source data into a navmesh.
////////////////////////////////////////////////////////////////////
bool RecastNavMesh::
build() const {
  if (_source == NULL) {
    navigation_cat.error()
      << "No source set to build a navmesh from!\n";
    return false;
  }

  rcConfig config = configure();

  rcHeightfield *heightfield = rcAllocHeightfield();
  if (!heightfield) {
    navigation_cat.error()
      << "Failed to allocate heightfield. Out of memory?\n";
    return false;
  }

  if (!rcCreateHeightfield(*heightfield, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch)) {
    navigation_cat.error()
      << "Failed to create solid heightfield.\n";
    return false;
  }

  rasterize_r(*heightfield, _source);

  rcFilterLowHangingWalkableObstacles(config.walkableClimb, *heightfield);
  rcFilterLedgeSpans(config.walkableHeight, config.walkableClimb, *heightfield);
  rcFilterWalkableLowHeightSpans(config.walkableHeight, *heightfield);

  rcCompactHeightfield *compact_heightfield = rcAllocCompactHeightfield();
  if (!compact_heightfield) {
    navigation_cat.error()
      << "Failed to allocate compact heightfield. Out of memory?\n";
    rcFreeHeightField(heightfield);
    return false;
  }

  // Convert our heightfield into a compact heightfield.
  if (!rcBuildCompactHeightfield(config.walkableHeight, config.walkableClimb, *heightfield, *compact_heightfield)) {
    navigation_cat.error()
      << "Failed to build compact heightfield data.\n";
    rcFreeHeightField(heightfield);
    rcFreeCompactHeightfield(compact_heightfield);
    return false;
  }

  rcFreeHeightField(heightfield);

  if (config.walkableRadius > 0) {
    if (!rcErodeWalkableArea(config.walkableRadius, *compact_heightfield)) {
      navigation_cat.error()
        << "Failed to erode walkable area.\n";
      rcFreeCompactHeightfield(compact_heightfield);
      return false;
    }
  }

  if (!rcBuildDistanceField(*compact_heightfield)) {
    navigation_cat.error()
      << "Failed to build distance field.\n";
    rcFreeCompactHeightfield(compact_heightfield);
    return false;
  }

  if (!rcBuildRegions(*compact_heightfield, config.borderSize, config.minRegionSize, config.mergeRegionSize)) {
    navigation_cat.error()
      << "Failed to build regions.\n";
  }

  rcContourSet *contour_set = rcAllocContourSet();
  if (!contour_set) {
    navigation_cat.error()
      << "Failed to allocate contour set. Out of memory?\n";
    rcFreeCompactHeightfield(compact_heightfield);
    return false;
  }

  if (!rcBuildContours(*compact_heightfield, config.maxSimplificationError, config.maxEdgeLen, *contour_set)) {
    navigation_cat.error()
      << "Failed to create contours.";
    rcFreeCompactHeightfield(compact_heightfield);
    rcFreeContourSet(contour_set);
    return false;
  }

  rcPolyMesh *poly_mesh = rcAllocPolyMesh();
  if (!poly_mesh) {
    navigation_cat.error()
      << "Failed to allocate polygon mesh. Out of memory?\n";
    rcFreeCompactHeightfield(compact_heightfield);
    rcFreeContourSet(contour_set);
    return false;
  }

  if (!rcBuildPolyMesh(*contour_set, config.maxVertsPerPoly, *poly_mesh)) {
    navigation_cat.error()
      << "Failed to triangulate contours.";
    rcFreeCompactHeightfield(compact_heightfield);
    rcFreeContourSet(contour_set);
    return false;
  }

  rcPolyMeshDetail *detail_poly_mesh = rcAllocPolyMeshDetail();
  if (!detail_poly_mesh) {
    navigation_cat.error()
      << "Failed to allocate detail polygon mesh. Out of memory?\n";
    rcFreeCompactHeightfield(compact_heightfield);
    rcFreeContourSet(contour_set);
    return false;
  }

  if (!rcBuildPolyMeshDetail(*poly_mesh, *compact_heightfield, config.detailSampleDist, config.detailSampleMaxError, *detail_poly_mesh)) {
    navigation_cat.error()
      << "Failed to build detail mesh.\n";
  }

  rcFreeCompactHeightfield(compact_heightfield);
  rcFreeContourSet(contour_set);

  unsigned char *nav_data = 0;
  int nav_data_size = 0;

  dtNavMeshCreateParams params;
  memset(&params, 0, sizeof(params));
  params.verts = poly_mesh->verts;
  params.vertCount = poly_mesh->nverts;
  params.polys = poly_mesh->polys;
  params.polyAreas = poly_mesh->areas;
  params.polyFlags = poly_mesh->flags;
  params.polyCount = poly_mesh->npolys;
  params.nvp = poly_mesh->nvp;
  params.detailMeshes = detail_poly_mesh->meshes;
  params.detailVerts = detail_poly_mesh->verts;
  params.detailVertsCount = detail_poly_mesh->nverts;
  params.detailTris = detail_poly_mesh->tris;
  params.detailTriCount = detail_poly_mesh->ntris;
  params.walkableHeight = _walkable_height;
  params.walkableRadius = _walkable_radius;
  params.walkableClimb = _walkable_climb;
  rcVcopy(params.bmin, poly_mesh->bmin);
  rcVcopy(params.bmax, poly_mesh->bmax);
  params.cs = config.cs;
  params.ch = config.ch;

  if (!dtCreateNavMeshData(&params, &nav_data, &nav_data_size)) {
    navigation_cat.error()
      << "Failed to build Detour navigation mesh.\n";
      return false;
  }

  if (!_node) {
    dtFree(nav_data);
    navigation_cat.error()
      << "Failed to create Detour navigation mesh node.\n";
      return false;
  }

  if (!_node->init(nav_data, nav_data_size, DT_TILE_FREE_DATA, 2048)) {
    dtFree(nav_data);
    navigation_cat.error()
      << "Failed to initialize Detour navigation mesh node.\n";
      return false;
  }

  return true;
}

