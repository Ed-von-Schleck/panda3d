// Filename: cgContextManager.h
// Created by:  rdb (20Aug10)
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

#ifndef CGCONTEXTMANAGER_H
#define CGCONTEXTMANAGER_H

#include "bamCacheRecord.h"
#include "config_cg.h"

typedef struct _CGcontext* CGcontext;
typedef struct _CGeffect* CGeffect;
typedef struct _CGstate* CGstate;

////////////////////////////////////////////////////////////////////
//       Class : CgContextManager
// Description : Manages a Cg context.
////////////////////////////////////////////////////////////////////
class CgContextManager {
public:
  virtual ~CgContextManager();
  static CGeffect create_effect(const char *source, const char **args, BamCacheRecord *record = NULL);

private:
  CgContextManager();
  void register_states();
  void register_sampler_states();
  void register_sampler_wrap_enumerants(CGstate cg_state);

  static CgContextManager *get_global_ptr();
  static void cg_include_callback(CGcontext context, const char* filename);

  static CgContextManager *_global_ptr;
  CGcontext _context;
  BamCacheRecord *_record;
  LightMutex _lock;
};

#endif

