// Filename: colladaLoader.cxx
// Created by: Xidram (21Dec10)
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

#include "dcast.h"
#include "geom.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geomVertexWriter.h"
#include "lmatrix.h"
#include "modelRoot.h"

#include "COLLADAFWArrayPrimitiveType.h"
#include "COLLADAFWInstanceGeometry.h"
#include "COLLADAFWMesh.h"
#include "COLLADAFWRoot.h"
#include "COLLADASaxFWLLoader.h"
#include "COLLADAFWStableHeaders.h"

#include "colladaLoader.h"

TypeHandle ColladaLoader::_type_handle;

ColladaLoader::
ColladaLoader() {
}
////////////////////////////////////////////////////////////////////
//     Function: ColladaLoader::load_collada_file
//  Description: Loads the indicated COLLADA file and returns the
//               root of a scene graph. Returns NULL if the file
//               cannot be read for some reason.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaLoader::
load_collada_file(const Filename &filename) {
  COLLADASaxFWL::Loader loader;
  COLLADAFW::Root root(&loader, this);

  _root_node = new ModelRoot(filename.get_basename());

  // Load scene graph
  if (!root.loadDocument(filename.get_fullpath()))
    return NULL;

  return DCAST(PandaNode, _root_node);
}

bool ColladaLoader::
writeGeometry(const COLLADAFW::Geometry* geom) {
  COLLADAFW::Mesh *collada_mesh = (COLLADAFW::Mesh*)geom;

  //TODO: Use a different geom vertex format as more features are added.
  CPT(GeomVertexFormat) format = GeomVertexFormat::get_v3();
  PT(GeomVertexData) data = new GeomVertexData(collada_mesh->getName(), format, Geom::UH_static);
  GeomVertexWriter vertex = GeomVertexWriter(data, "vertex");

  // Write the vertices.
  COLLADAFW::MeshVertexData& pos = collada_mesh->getPositions();
  int vertex_count = pos.getFloatValues()->getCount() / 3;
  std::cerr << "Vertex count: " << vertex_count << std::endl;
  for (unsigned int i = 0; i < vertex_count; i++) {
    int current_vertex = i * 3;
    switch (pos.getType()) {
      case COLLADAFW::MeshVertexData::DATA_TYPE_FLOAT: {
        COLLADAFW::ArrayPrimitiveType<float>* values = pos.getFloatValues();
        if (!values->empty()) {
          vertex.add_data3f((*values)[i], (*values)[i + 1], (*values)[i + 2]);
        }
      }
    }
  }

  return true;
}

bool ColladaLoader::
writeVisualScene(const COLLADAFW::VisualScene* visualScene) {
  _visual_scenes.push_back(visualScene);
  return true;
}

void ColladaLoader::
finish() {
  pvector<const COLLADAFW::VisualScene*>::iterator iterator;
  for (iterator = _visual_scenes.begin(); iterator != _visual_scenes.end(); iterator++) {
    const COLLADAFW::NodePointerArray& roots = (*iterator)->getRootNodes();
    for (unsigned int i = 0; i < roots.getCount(); i++) {
      write_node(roots[i], DCAST(PandaNode, _root_node));
    }
  }
}

void ColladaLoader::
write_node(COLLADAFW::Node* collada_node, PT(PandaNode) parent_node) {
  if (collada_node->getType() == COLLADAFW::Node::JOINT) {
  } else {
    COLLADAFW::InstanceGeometryPointerArray &geom = collada_node->getInstanceGeometries();
    if (geom.getCount() != 0) {
      
      
    }
  }

  PT(PandaNode) panda_node = new PandaNode(collada_node->getName());
  parent_node->add_child(panda_node);
  COLLADAFW::NodePointerArray &child_nodes = collada_node->getChildNodes();
  for (unsigned int i = 0; i < child_nodes.getCount(); i++) {
    write_node(child_nodes[i], panda_node);
  }
}
