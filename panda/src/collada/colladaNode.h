// Filename: colladaNode.h
// Created by: rdb (13Apr10)
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

#ifndef COLLADANODE_H
#define COLLADANODE_H

#include "typedReferenceCount.h"
#include "config_collada.h"
#include "lmatrix.h"
#include "pandaNode.h"
#include "colladaAssetElement.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaNode
// Description : Object that represents the <node> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaNode : public ColladaAssetElement {
PUBLISHED:
  ColladaNode();
  INLINE virtual void clear();

  enum NodeType {
    NT_default,
    NT_node,
    NT_joint
  };

public:
  virtual bool load_xml(const TiXmlElement *xelement, const CoordinateSystem cs = CS_default);
  virtual TiXmlElement *make_xml() const;
  PT(PandaNode) make_node() const;

  virtual PT(ColladaElement) get_element_by_id(const string &id) const;

  // Needed by ColladaLibrary to validate the element names.
  static const string _element_name;
  static const string _library_name;

private:
  NodeType _node_type;
  pvector<PT(ColladaNode)> _nodes;

  // The following ifndef is a workaround to a major Interrogate issue.
  // Without it, Interrogate starts leaking virtual biochemical waste all over
  // memory like it has a severe gastrointestinal disorder. Memory gets filled,
  // the system comes to a crawl, and we don't get to eat Pandette Scout treats.
  #ifndef CPPPARSER
    pvector<PT(ColladaInstanceCamera)> _instance_cameras;
    pvector<PT(ColladaInstanceGeometry)> _instance_geometries;
    pvector<PT(ColladaInstanceLight)> _instance_lights;
    pvector<PT(ColladaInstanceNode)> _instance_nodes;
  #endif
  LMatrix4d _transform;

  // Used for instancing purposes
  PT(PandaNode) _cached_node;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaNode",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaNode.I"

#endif
