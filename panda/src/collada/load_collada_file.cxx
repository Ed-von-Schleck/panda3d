// Filename: load_collada_file.cxx
// Created by:  rdb (23Aug09)
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

#include "load_collada_file.h"
#include "config_collada.h"
#include "sceneGraphReducer.h"
#include "virtualFileSystem.h"
#include "config_util.h"
#include "bamCacheRecord.h"

#include "colladaLoader.h"
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include "tinyxml.h"

static PT(PandaNode)
load_from_loader(ColladaLoader &loader) {
  //loader->_data->load_externals(DSearchPath(), loader->_record);

  loader.build_graph();

  if (loader._error && !collada_accept_errors) {
    collada_cat.error()
      << "Errors in collada file.\n";
    return NULL;
  }

  if (loader._root != (PandaNode *)NULL && collada_flatten) {
    SceneGraphReducer gr;

    int combine_siblings_bits = 0;
    if (collada_combine_geoms) {
      combine_siblings_bits |= SceneGraphReducer::CS_geom_node;
    }
    if (collada_flatten_radius > 0.0) {
      combine_siblings_bits |= SceneGraphReducer::CS_within_radius;
      gr.set_combine_radius(collada_flatten_radius);
    }

    int num_reduced = gr.flatten(loader._root, combine_siblings_bits);
    collada_cat.info() << "Flattened " << num_reduced << " nodes.\n";

    if (collada_unify) {
      // We want to premunge before unifying, since otherwise we risk
      // needlessly duplicating vertices.
      if (premunge_data) {
        gr.premunge(loader._root, RenderState::make_empty());
      }
      gr.collect_vertex_data(loader._root);
      gr.unify(loader._root, true);
      if (collada_cat.is_debug()) {
        collada_cat.debug() << "Unified.\n";
      }
    }
  }

  return loader._root;
}

////////////////////////////////////////////////////////////////////
//     Function: load_collada_file
//  Description: A convenience function.  Loads up the indicated collada
//               file, and returns the root of a scene graph.  Returns
//               NULL if the file cannot be read for some reason.
////////////////////////////////////////////////////////////////////
PT(PandaNode)
load_collada_file(const Filename &filename, CoordinateSystem cs,
                  BamCacheRecord *record) {
  Filename collada_filename = filename;
  collada_filename.set_text();
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  if (record != (BamCacheRecord *)NULL) {
    record->add_dependent_file(collada_filename);
  }

  ColladaLoader loader;
  loader._filename = collada_filename;
  loader._record = record;
  loader._error = false;

  bool okflag;
  istream *istr = vfs->open_read_file(collada_filename, true);
  if (istr == (istream *)NULL) {
    return NULL;
  }

  collada_cat.info()
    << "Reading " << collada_filename << "\n";

  //okflag = loader._data->read(*istr);
  vfs->close_read_file(istr);

  if (!okflag) {
    collada_cat.error()
      << "Error reading " << collada_filename << "\n";
    return NULL;
  }

  return load_from_loader(loader);
}

