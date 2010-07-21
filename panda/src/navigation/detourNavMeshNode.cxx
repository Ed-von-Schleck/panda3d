// Filename: detourNavMeshNode.cxx
// Created by:  rdb (21Jul10)
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

#include "detourNavMeshNode.h"
#include "cullTraverserData.h"
#include "geomVertexWriter.h"
#include "geomTriangles.h"
#include "geomTristrips.h"
#include "boundingBox.h"
#include "colorAttrib.h"
#include "depthOffsetAttrib.h"
#include "transparencyAttrib.h"

TypeHandle DetourNavMeshNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
DetourNavMeshNode::
DetourNavMeshNode(const string &name) : PandaNode(name) {
  _viz_geom = NULL;
  set_cull_callback();

  // DetourNavMeshNodes are hidden by default.
  set_overall_hidden(true);
}

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::cull_callback
//       Access: Public, Virtual
//  Description: This function will be called during the cull
//               traversal to perform any additional operations that
//               should be performed at cull time.  This may include
//               additional manipulation of render state or additional
//               visible/invisible decisions, or any other arbitrary
//               operation.
//
//               Note that this function will *not* be called unless
//               set_cull_callback() is called in the constructor of
//               the derived class.  It is necessary to call
//               set_cull_callback() to indicated that we require
//               cull_callback() to be called.
//
//               By the time this function is called, the node has
//               already passed the bounding-volume test for the
//               viewing frustum, and the node's transform and state
//               have already been applied to the indicated
//               CullTraverserData object.
//
//               The return value is true if this node should be
//               visible, or false if it should be culled.
////////////////////////////////////////////////////////////////////
bool DetourNavMeshNode::
cull_callback(CullTraverser *trav, CullTraverserData &data) {
  if (_viz_geom == NULL) {
    _viz_geom = new GeomNode("viz");
    PT(GeomVertexData) vdata = new GeomVertexData("viz", GeomVertexFormat::get_v3(), Geom::UH_static);
    GeomVertexWriter writer (vdata, InternalName::get_vertex());
    PT(GeomTriangles) triangles = new GeomTriangles(Geom::UH_static);
    const LMatrix4f &conv = LMatrix4f::convert_mat(CS_yup_right, CS_default);

    for (int t = 0; t < getMaxTiles(); ++t) {
      dtMeshHeader *header = getTile(t)->header;

      // Add the vertices
      float *verts = getTile(t)->verts;
      for (int v = 0; v < header->vertCount; ++v) {
        LPoint3f vtx (verts[v * 3], verts[v * 3 + 1], verts[v * 3 + 2]);
        vtx = conv.xform_point(vtx);
        writer.add_data3f(vtx);
      }

      // Add the polygons
      dtPoly *polys = getTile(t)->polys;
      for (int p = 0; p < header->polyCount; ++p) {
        if (polys[p].vertCount > 3) {
          // Fan from first vertex
          for (int v = 2; v < polys[p].vertCount; ++v) {
            triangles->add_vertex(polys[p].verts[0]);
            triangles->add_vertex(polys[p].verts[v - 1]);
            triangles->add_vertex(polys[p].verts[v]);
            triangles->close_primitive();
          }
        } else {
          triangles->add_vertex(polys[p].verts[0]);
          triangles->add_vertex(polys[p].verts[1]);
          triangles->add_vertex(polys[p].verts[2]);
          triangles->close_primitive();
        }
      }
    }

    // Create a geom and add the primitives to it.
    PT(Geom) geom = new Geom(vdata);
    geom->add_primitive(triangles);
    _viz_geom->add_geom(geom);
    _viz_geom->set_state(RenderState::make(DepthOffsetAttrib::make(1),
               TransparencyAttrib::make(TransparencyAttrib::M_alpha),
               ColorAttrib::make_flat(LVecBase4f(0, 0.75, 1, 0.25))));
  }

  CullTraverserData next_data(data, _viz_geom);

  // We don't want to inherit the render state from above for this.
  next_data._state = RenderState::make_empty();
  trav->traverse(next_data);

  // Now carry on to render our child nodes.
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::is_renderable
//       Access: Public, Virtual
//  Description: Returns true if there is some value to visiting this
//               particular node during the cull traversal for any
//               camera, false otherwise.  This will be used to
//               optimize the result of get_net_draw_show_mask(), so
//               that any subtrees that contain only nodes for which
//               is_renderable() is false need not be visited.
////////////////////////////////////////////////////////////////////
bool DetourNavMeshNode::
is_renderable() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::compute_internal_bounds
//       Access: Protected, Virtual
//  Description: Called when needed to recompute the node's
//               _internal_bound object.  Nodes that contain anything
//               of substance should redefine this to do the right
//               thing.
////////////////////////////////////////////////////////////////////
void DetourNavMeshNode::
compute_internal_bounds(CPT(BoundingVolume) &internal_bounds,
                        int &internal_vertices,
                        int pipeline_stage,
                        Thread *current_thread) const {

  LPoint3f bmin_all, bmax_all;
  for (int i = 0; i < getMaxTiles(); ++i) {
    float* tmin = getTile(i)->header->bmin;
    float* tmax = getTile(i)->header->bmax;

    if (i == 0) {
      bmin_all.set(tmin[0], tmin[1], tmin[2]);
      bmax_all.set(tmax[0], tmax[1], tmax[2]);
      continue;
    }
    bmin_all[0] = min(tmin[0], bmin_all[0]);
    bmin_all[1] = min(tmin[1], bmin_all[1]);
    bmin_all[2] = min(tmin[2], bmin_all[2]);
    bmax_all[0] = max(tmax[0], bmax_all[0]);
    bmax_all[1] = max(tmax[1], bmax_all[1]);
    bmax_all[2] = max(tmax[2], bmax_all[2]);
  }

  const LMatrix4f &conv = LMatrix4f::convert_mat(CS_yup_right, CS_default);
  bmin_all = conv.xform_point(bmin_all);
  bmax_all = conv.xform_point(bmax_all);
  LPoint3f bmin_new, bmax_new;
  bmin_new[0] = min(bmin_all[0], bmax_all[0]);
  bmin_new[1] = min(bmin_all[1], bmax_all[1]);
  bmin_new[2] = min(bmin_all[2], bmax_all[2]);
  bmax_new[0] = max(bmin_all[0], bmax_all[0]);
  bmax_new[1] = max(bmin_all[1], bmax_all[1]);
  bmax_new[2] = max(bmin_all[2], bmax_all[2]);

  internal_bounds = new BoundingBox(bmin_new, bmax_new);
  internal_vertices = 0;
}

