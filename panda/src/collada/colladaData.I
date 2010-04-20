// Filename: colladaData.I
// Created by:  rdb (13Apr10)
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

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaData::
ColladaData() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::get_coordinate_system
//       Access: Public
//  Description: Returns the coordinate system in which the file
//               is defined. Note that this returns the global
//               coordinate system for the entire file - it is
//               possible that elements in the document override
//               this value.
////////////////////////////////////////////////////////////////////
INLINE CoordinateSystem ColladaData::
get_coordinate_system() const {
  return _asset ? _asset->get_coordinate_system() : CS_yup_right;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::set_filename
//       Access: Public
//  Description: Sets the filename--especially the directory part--in
//               which the file is considered to reside.  This is
//               also implicitly set by read().
////////////////////////////////////////////////////////////////////
INLINE void ColladaData::
set_filename(const Filename &filename) {
  _filename = filename;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::get_filename
//       Access: Public
//  Description: Returns the directory in which the collada file is
//               considered to reside.
////////////////////////////////////////////////////////////////////
INLINE const Filename &ColladaData::
get_filename() const {
  return _filename;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::resolve_instance
//       Access: Public
//  Description: Resolves the URL in the given instance, returns
//               the right element, or NULL if it wasn't found.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE PT(T) ColladaData::
resolve_instance(const ColladaInstance<T> &inst) const {
  if (inst._url.empty()) {
    return NULL;
  }

  if (inst._url[0] != '#') {
    collada_cat.warning() << "URL '" << inst._url << "' is not local, cannot resolve\n";
    return NULL;
  }

  string id = inst._url.substr(1);
  const TypeHandle &handle = T::get_class_type();
  if (handle == ColladaEffect::get_class_type()) {
    return DCAST(T, _library_effects.get_element_by_id(id));
  } else if (handle == ColladaGeometry::get_class_type()) {
    return DCAST(T, _library_geometries.get_element_by_id(id));
  } else if (handle == ColladaMaterial::get_class_type()) {
    return DCAST(T, _library_materials.get_element_by_id(id));
  } else if (handle == ColladaNode::get_class_type()) {
    return DCAST(T, _library_nodes.get_element_by_id(id));
  } else if (handle == ColladaVisualScene::get_class_type()) {
    return DCAST(T, _library_visual_scenes.get_element_by_id(id));
  } else {
    collada_cat.error()
      << "ColladaData::resolve_instance invoked with unknown type "
      << T::get_class_type().get_name() << "\n";
  }
  return NULL;
}

