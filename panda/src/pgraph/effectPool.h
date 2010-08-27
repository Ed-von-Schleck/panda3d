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

#include "effect.h"
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
  INLINE static Effect *load_effect(const Filename &filename,
                                    const LoaderOptions &options = LoaderOptions());

  INLINE static void add_effect(const Filename &filename, Effect *effect);
  INLINE static void release_effect(const Filename &filename);

  INLINE static void add_effect(Effect *effect);
  INLINE static void release_effect(Effect *effect);


  INLINE static void release_all_effects();

  INLINE static int garbage_collect();

  INLINE static void list_contents(ostream &out);
  INLINE static void list_contents();
  static void write(ostream &out);

  static EffectPool *get_global_ptr();

private:
  INLINE EffectPool();

  bool ns_has_effect(const Filename &filename);
  Effect *ns_load_effect(const Filename &filename,
                           const LoaderOptions &options);
  void ns_add_effect(const Filename &filename, Effect *effect);
  void ns_release_effect(const Filename &filename);

  void ns_add_effect(Effect *effect);
  void ns_release_effect(Effect *effect);

  void ns_release_all_effects();
  int ns_garbage_collect();
  void ns_list_contents(ostream &out) const;

  static EffectPool *_global_ptr;

  LightMutex _lock;
  typedef pmap<Filename, PT(Effect)> Effects;
  Effects _effects;
};

#include "effectPool.I"

#endif

