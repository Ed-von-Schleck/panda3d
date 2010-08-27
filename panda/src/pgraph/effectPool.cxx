// Filename: effectPool.cxx
// Created by: Xidram (14Aug10)
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

#include "effectPool.h"
#include "config_pgraph.h"
#include "lightMutexHolder.h"
#include "loader.h"
#include "virtualFileSystem.h"

EffectPool *EffectPool::_global_ptr = (EffectPool *)NULL;

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::write
//       Access: Published, Static
//  Description: Lists the contents of the effect pool to the
//               indicated output stream.
//               Helps with debugging.
////////////////////////////////////////////////////////////////////
void EffectPool::
write(ostream &out) {
  get_global_ptr()->ns_list_contents(out);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_has_effect
//       Access: Private
//  Description: The nonstatic implementation of has_effect().
////////////////////////////////////////////////////////////////////
bool EffectPool::
ns_has_effect(const Filename &filename) {
  LightMutexHolder holder(_lock);

  Effects::const_iterator ei;
  ei = _effects.find(filename);
  if (ei != _effects.end()) {
    // This effect was previously loaded.
    return true;
  }

  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_load_effect
//       Access: Private
//  Description: The nonstatic implementation of load_effect().
////////////////////////////////////////////////////////////////////
Effect *EffectPool::
ns_load_effect(const Filename &filename, const LoaderOptions &options) {
  {
    LightMutexHolder holder(_lock);
    Effects::const_iterator ei;
    ei = _effects.find(filename);
    if (ei != _effects.end()) {
      // This effect was previously loaded.
      Effect *effect = (*ei).second;
      nassertr(!effect->get_fullpath().empty(), effect);
      return effect;
    }
  }

  LoaderOptions new_options(options);
  new_options.set_flags((new_options.get_flags() | LoaderOptions::LF_no_ram_cache) &
                        ~(LoaderOptions::LF_search | LoaderOptions::LF_report_errors));

  Loader *model_loader = Loader::get_global_ptr();
  PT(PandaNode) panda_node = model_loader->load_sync(filename, new_options);
  PT(Effect) effect = NULL;

  if (panda_node.is_null()) {
    // This model was not found.

  } else {
    if (panda_node->is_of_type(Effect::get_class_type())) {
      effect = DCAST(Effect, panda_node);
      effect->set_fullpath(filename);
    } else {
      pgraph_cat.error() << filename << " is not an effect file!\n";
    }
  }

  {
    LightMutexHolder holder(_lock);

    // Now look again--someone may have just loaded this effect in
    // another thread.
    Effects::const_iterator ti;
    ti = _effects.find(filename);
    if (ti != _effects.end()) {
      // This effect was previously loaded.
      Effect *effect = (*ti).second;
      nassertr(!effect->get_fullpath().empty(), effect);
      return effect;
    }

    _effects[filename] = effect;
  }

  return effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_add_effect
//       Access: Private
//  Description: The nonstatic implementation of add_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_add_effect(const Filename &filename, Effect *effect) {
  LightMutexHolder holder(_lock);
  // We blow away whatever effect was there previously, if any.
  _effects[filename] = effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_effect
//       Access: Private
//  Description: The nonstatic implementation of release_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_effect(const Filename &filename) {
  LightMutexHolder holder(_lock);
  Effects::iterator ti;
  ti = _effects.find(filename);
  if (ti != _effects.end()) {
    _effects.erase(ti);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_add_effect
//       Access: Private
//  Description: The nonstatic implementation of add_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_add_effect(Effect *effect) {
  LightMutexHolder holder(_lock);
  // We blow away whatever effect was there previously, if any.
  _effects[effect->get_fullpath()] = effect;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_effect
//       Access: Private
//  Description: The nonstatic implementation of release_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_effect(Effect *effect) {
  LightMutexHolder holder(_lock);
  Effects::iterator ti;
  ti = _effects.find(effect->get_fullpath());
  if (ti != _effects.end()) {
    _effects.erase(ti);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_release_all_effects
//       Access: Private
//  Description: The nonstatic implementation of release_all_effects().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_release_all_effects() {
  LightMutexHolder holder(_lock);
  _effects.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_garbage_collect
//       Access: Private
//  Description: The nonstatic implementation of garbage_collect().
////////////////////////////////////////////////////////////////////
int EffectPool::
ns_garbage_collect() {
  LightMutexHolder holder(_lock);

  int num_released = 0;
  Effects new_set;

  Effects::iterator ti;
  for (ti = _effects.begin(); ti != _effects.end(); ++ti) {
    Effect *node = (*ti).second;
    if (node == (Effect *)NULL ||
        node->get_ref_count() == 1) {
      if (loader_cat.is_debug()) {
        loader_cat.debug()
          << "Releasing " << (*ti).first << "\n";
      }
      ++num_released;
    } else {
      new_set.insert(new_set.end(), *ti);
    }
  }

  _effects.swap(new_set);
  return num_released;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_list_contents
//       Access: Private
//  Description: The nonstatic implementation of list_contents().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_list_contents(ostream &out) const {
  LightMutexHolder holder(_lock);

  out << "effect pool contents:\n";

  Effects::const_iterator ti;
  int num_effects = 0;
  for (ti = _effects.begin(); ti != _effects.end(); ++ti) {
    if ((*ti).second != NULL) {
      ++num_effects;
      out << (*ti).first << "\n"
          << "  (count = " << (*ti).second->get_ref_count()
          << ")\n";
    }
  }

  out << "total number of effects: " << num_effects << " (plus "
      << _effects.size() - num_effects << " entries for nonexistent files)\n";
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::get_global_ptr
//       Access: Private, Static
//  Description: Initializes and/or returns the global pointer to the
//               one EffectPool object in the system.
////////////////////////////////////////////////////////////////////
EffectPool *EffectPool::
get_global_ptr() {
  if (_global_ptr == (EffectPool *)NULL) {
    _global_ptr = new EffectPool;
  }
  return _global_ptr;
}

