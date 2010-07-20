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
#include "geomNode.h"
#include "geomPrimitive.h"
#include "geomTriangles.h"
#include "geomVertexReader.h"

TypeHandle RecastNavMesh::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
RecastNavMesh::
RecastNavMesh() {
  _cell_size = 0;
  _cell_height = 0;
  _walkable_slope_angle = 0;
  _walkable_height = 0;
  _walkable_climb = 0;
  _walkable_radius = 0;
  _max_edge_length = 0;
  _max_simplification_error = 0;
  _min_region_size = 0;
  _merge_region_size = 0;
  _detail_sample_distance = 0;
  _detail_sample_max_error = 0;

  _source = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: RecastNavMesh::rasterize_r
//       Access: Private
//  Description: Recursively rasterizes triangles from the specified
//               node.
////////////////////////////////////////////////////////////////////
void RecastNavMesh::
rasterize_r(CPT(PandaNode) node, LMatrix4f xform) const {
  xform = node->get_transform()->get_mat() * xform;
  if (node->is_geom_node()) {
    CPT(GeomNode) gnode = DCAST(GeomNode, node);
    for (size_t g = 0; g < gnode->get_num_geoms(); ++g) {
      PT(Geom) geom = gnode->get_geom(g)->decompose();
      geom->transform_vertices(xform);
      GeomVertexReader reader (geom->get_vertex_data(), InternalName::get_vertex());
      for (size_t p = 0; p < geom->get_num_primitives(); ++p) {
        CPT(GeomPrimitive) prim = geom->get_primitive(p);
        if (prim->is_of_type(GeomTriangles::get_class_type())) {
          continue;
        }
        for (size_t pr = 0; pr < prim->get_num_primitives(); ++pr) {
          int start = prim->get_primitive_start(pr);
          nassertd(start + 2 == prim->get_primitive_end(pr)) continue;
          reader.set_row(prim->get_vertex(start));
          LVector3f vtx1 = reader.get_data3f();
          reader.set_row(prim->get_vertex(start + 1));
          LVector3f vtx2 = reader.get_data3f();
          reader.set_row(prim->get_vertex(start + 3));
          LVector3f vtx3 = reader.get_data3f();
          cerr << vtx1 << "_" << vtx2 << "_" << vtx3 << "\n";
          //rcRasterizeTriangle();
        }
      }
    }
  }

  for (size_t c = 0; c < node->get_num_children(); ++c) {
    rasterize_r(node->get_child(c), xform);
  }
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

  rasterize_r(_source);
  return true;
}

