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
#include "boundingBox.h"

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
  /*if (_viz_geom != NULL) {
    CullTraverserData next_data(data, node);

    // We don't want to inherit the render state from above for these
    // guys, except for the depth offset - and we add one to that.
    CPT(DepthOffsetAttrib) offset = data._state.get_attrib()
    next_data._state = RenderState::make_empty();
    trav->traverse(next_data);
  }*/

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

  internal_bounds = new BoundingBox(bmin_all, bmax_all);
  internal_vertices = 0;
}

