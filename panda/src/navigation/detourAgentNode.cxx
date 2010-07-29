// Filename: detourAgentNode.cxx
// Created by: Xidram (28Jul10)
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

#include "detourAgentNode.h"

#include <Recast.h>

TypeHandle DetourAgentNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: DetourNavMeshNode::update
//       Access: Published
//  Description: Updates the position of the agent based on the
//               configured parameters. The passed parameter is
//               the time elapsed since the last frame.
//               Note that you don't need to call this unless
//               you explicitly instructed the agent not to
//               automatically add a task to the task manager.
////////////////////////////////////////////////////////////////////
void DetourAgentNode::
update(float dt) {
  if (_target_node == NULL || _speed == 0.0) {
    return;
  }

  const LMatrix4f &conv = LMatrix4f::convert_mat(CS_default, CS_yup_right);
  const LMatrix4f &conv_back = LMatrix4f::convert_mat(CS_yup_right, CS_default);

  const dtNavMesh &nav_mesh = *_nav_mesh->_nav_mesh;

  dtPolyRef start (0);
  dtPolyRef end (0);
  float search_box[3] = {2, 4, 2};
  dtQueryFilter filter;
  filter.includeFlags = 0xFFFF;
  filter.excludeFlags = 0;

  const float* start_point = conv.xform_point(get_transform()->get_pos())._v.data;
  const float* end_point = conv.xform_point(_target_node->get_transform()->get_pos())._v.data;

  start = nav_mesh.findNearestPoly(start_point, search_box, &filter, 0);
  //TODO: transform into coordinate space of nav mesh
  end = nav_mesh.findNearestPoly(end_point, search_box, &filter, 0);

#define MAX_POLYS 256
  dtPolyRef polygons[MAX_POLYS];
  int polycount = nav_mesh.findPath(start, end, start_point, end_point, &filter, polygons, MAX_POLYS);

  if (polycount == 0) {
    return;
  }

  // In case the end point is not accessible, go to the closest point.
  float new_end_point[3];
  if (polygons[polycount - 1] == end) {
    rcVcopy(new_end_point, end_point);
  } else {
    nav_mesh.closestPointOnPoly(polygons[polycount - 1], end_point, new_end_point);
  }

  float path_points[MAX_POLYS * 3];

  int cur_point = 0;
  float distance = _speed * dt;

  int pathlen = nav_mesh.findStraightPath(start_point, new_end_point, polygons, polycount,
                                          path_points, NULL, NULL, MAX_POLYS);

  while (distance > 0.0f && cur_point < MAX_POLYS) {
    LPoint3f next_point;
    rcVcopy(next_point._v.data, path_points + cur_point * 3);
    next_point = conv_back.xform_point(next_point);
    //TODO: coordinate system conversion, remember?
    LVector3f steer = next_point - get_transform()->get_pos();

    if (distance * distance <= steer.length_squared()) {
      // Our new pos will be between the current pos and the next point
      steer.normalize();
      steer *= distance;
      CPT(TransformState) transform = get_transform();
      set_transform(transform->set_pos(transform->get_pos() + steer));
      distance = 0.0f;
    } else {
      set_transform(get_transform()->set_pos(next_point));
      distance -= steer.length();
    }
    cur_point += 1;
  }
}

