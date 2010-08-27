// Filename: loaderFileTypeCgFX.h
// Created by:  rdb (27Aug10)
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

#ifndef LOADERFILETYPECGFX_H
#define LOADERFILETYPECGFX_H

#include "pandabase.h"
#include "material.h"
#include "loaderFileType.h"

typedef struct _CGpass *CGpass;

////////////////////////////////////////////////////////////////////
//       Class : LoaderFileTypeCgFX
// Description : This defines the Loader interface to read CgFX files.
////////////////////////////////////////////////////////////////////
class EXPCL_CG LoaderFileTypeCgFX : public LoaderFileType {
public:
  LoaderFileTypeCgFX();

  virtual string get_name() const;
  virtual string get_extension() const;
  virtual bool supports_compressed() const;

  virtual PT(PandaNode) load_file(const Filename &path, const LoaderOptions &options,
                                  BamCacheRecord *record) const;

private:
  PT(Material) load_material(const CGpass pass) const;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    LoaderFileType::init_type();
    register_type(_type_handle, "LoaderFileTypeCgFX",
                  LoaderFileType::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif

