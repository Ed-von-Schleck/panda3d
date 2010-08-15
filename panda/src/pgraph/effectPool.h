// Filename: effectPool.h
// Created by: Xidram (13Aug10)
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

#ifndef EFFECTPOOL_H
#define EFFECTPOOL_H

#include "pandabase.h"

#include "effectFile.h"
#include "filename.h"
#include "loaderOptions.h"
#include "pmap.h"
#include "pointerTo.h"
#include "renderPass.h"

////////////////////////////////////////////////////////////////////
//       Class : EffectPool
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PGRAPH EffectPool {
PUBLISHED:
  INLINE static bool has_effect(const Filename &filename);
  INLINE static bool verify_effect(const Filename &filename);
  INLINE static EffectFile *load_effect(const Filename &filename,
                                    const LoaderOptions &options = LoaderOptions());

  INLINE static void add_effect(const Filename &filename, EffectFile *effect);
  INLINE static void release_effect(const Filename &filename);

  INLINE static void add_effect(EffectFile *effect);
  INLINE static void release_effect(EffectFile *effect);


  INLINE static void release_all_effects();

  INLINE static int garbage_collect();

  INLINE static void list_contents(ostream &out);
  INLINE static void list_contents();
  static void write(ostream &out);

private:
  INLINE EffectPool();

  bool ns_has_effect(const Filename &filename);
  EffectFile *ns_load_effect(const Filename &filename,
                           const LoaderOptions &options);
  void ns_add_effect(const Filename &filename, EffectFile *effect);
  void ns_release_effect(const Filename &filename);

  void ns_add_effect(EffectFile *effect);
  void ns_release_effect(EffectFile *effect);

  void ns_release_all_effects();
  int ns_garbage_collect();
  void ns_list_contents(ostream &out) const;

  static EffectPool *get_ptr();

  static EffectPool *_global_ptr;

  LightMutex _lock;
  typedef pmap<Filename,  PT(EffectFile)> Effects;
  Effects _effects;

};

#include "effectPool.I"

#endif

