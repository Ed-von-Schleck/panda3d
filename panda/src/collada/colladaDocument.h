// Filename: colladaDocument.h
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

#ifndef COLLADADOCUMENT_H
#define COLLADADOCUMENT_H

#include "pandabase.h"

#include "config_collada.h"
#include "filename.h"
#include "coordinateSystem.h"
#include "pnotify.h"
#include "dSearchPath.h"
#include "typedReferenceCount.h"
#include "pointerTo.h"
#include "pandaNode.h"

class ColladaElement;
class ColladaInstanceBase;
class ColladaRoot;

////////////////////////////////////////////////////////////////////
//       Class : ColladaDocument
// Description : This is the primary interface into all the data,
//               and the root of the collada file structure.  A
//               ColladaDocument structure corresponds exactly with
//               a collada file on the disk.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaDocument : public TypedReferenceCount {
PUBLISHED:
  ColladaDocument();
  virtual ~ColladaDocument();

  static bool resolve_dae_filename(Filename &dae_filename,
                                   const DSearchPath &searchpath = DSearchPath());

  virtual PT(PandaNode) make_node() const;

  bool read(Filename filename, string display_name = string());
  bool read(istream &in);

  bool write_dae(Filename filename) const;
  bool write_dae(ostream &out) const;

  INLINE void set_filename(const Filename &filename);
  INLINE const Filename &get_filename() const;

  ColladaElement *resolve_url(const string &url) const;
  ColladaElement *resolve_instance(const ColladaInstanceBase *inst) const;

private:
  ColladaRoot *_root;
  Filename _filename;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaDocument",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaDocument.I"

#endif

