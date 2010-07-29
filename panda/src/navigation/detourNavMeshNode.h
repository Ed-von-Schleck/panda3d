// Filename: detourNavMeshNode.h
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

#ifndef DETOURNAVMESHNODE_H
#define DETOURNAVMESHNODE_H

#include "config_navigation.h"
#include "pandaNode.h"
#include "geomNode.h"

#include <DetourNavMesh.h>

class DetourAgentNode;

////////////////////////////////////////////////////////////////////
//       Class : DetourNavMeshNode
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_NAVIGATION DetourNavMeshNode : public PandaNode {
PUBLISHED:
  DetourNavMeshNode(const string &name);
  virtual ~DetourNavMeshNode();

  INLINE float get_area_cost(const int area);
  INLINE void set_area_cost(const int area, float cost);

  friend class RecastNavMesh;

public:
  virtual bool cull_callback(CullTraverser *trav, CullTraverserData &data);
  virtual bool is_renderable() const;

protected:
  virtual void compute_internal_bounds(CPT(BoundingVolume) &internal_bounds,
                                       int &internal_vertices,
                                       int pipeline_stage,
                                       Thread *current_thread) const;

private:
  PT(GeomNode) _viz_geom;
#ifndef CPPPARSER
  dtNavMesh *_nav_mesh;
#endif

  friend class DetourAgentNode;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PandaNode::init_type();
    register_type(_type_handle, "DetourNavMeshNode",
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "detourNavMeshNode.I"

#endif

