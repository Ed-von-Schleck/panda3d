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

EffectPool *EffectPool::_global_ptr = (EffectPool *)NULL;

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_has_effect
//       Access: Private
//  Description: The nonstatic implementation of has_effect().
////////////////////////////////////////////////////////////////////
bool EffectPool::
ns_has_effect(const Filename &filename) {
  LightMutexHolder holder(_lock);
  Effects::const_iterator ti;
  ti = _effects.find(filename);
  if (ti != _effects.end() && (*ti).second != (EffectFile *)NULL) {
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
EffectFile *EffectPool::
ns_load_effect(const Filename &filename, const LoaderOptions &options) {
  {
    LightMutexHolder holder(_lock);
    Effects::const_iterator ti;
    ti = _effects.find(filename);
    if (ti != _effects.end()) {
      // This effect was previously loaded.
      return (*ti).second;
    }
  }

  LoaderOptions new_options(options);
  new_options.set_flags((new_options.get_flags() | LoaderOptions::LF_no_ram_cache) &
                        ~(LoaderOptions::LF_search | LoaderOptions::LF_report_errors));

  Loader *effect_loader = Loader::get_global_ptr();
  PT(PandaNode) panda_node = effect_loader->load_sync(filename, new_options);
  PT(EffectFile) node;

  if (panda_node.is_null()) {
    // This effect was not found.

  } else {
    if (panda_node->is_of_type(EffectFile::get_class_type())) {
      node = DCAST(EffectFile, panda_node);
      
    } else {
      // We have to construct a EffectFile node to put it under.
      node = new EffectFile(filename);
      node->add_child(panda_node);
    }
    node->set_fullpath(filename);
  }

  {
    LightMutexHolder holder(_lock);

    // Look again, in case someone has just loaded the effect in
    // another thread.
    Effects::const_iterator ti;
    ti = _effects.find(filename);
    if (ti != _effects.end()) {
      // This effect was previously loaded.
      return (*ti).second;
    }

    _effects[filename] = node;
  }

  return node;
}


////////////////////////////////////////////////////////////////////
//     Function: EffectPool::ns_add_effect
//       Access: Private
//  Description: The nonstatic implementation of add_effect().
////////////////////////////////////////////////////////////////////
void EffectPool::
ns_add_effect(const Filename &filename, EffectFile *effect) {
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
ns_add_effect(EffectFile *effect) {
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
ns_release_effect(EffectFile *effect) {
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
    EffectFile *node = (*ti).second;
    if (node == (EffectFile *)NULL ||
        node->get_effect_ref_count() == 1) {
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
          << "  (count = " << (*ti).second->get_effect_ref_count() 
          << ")\n";
    }
  }
  
  out << "total number of effects: " << num_effects << " (plus " 
      << _effects.size() - num_effects << " entries for nonexistent files)\n";
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::get_ptr
//       Access: Private, Static
//  Description: Initializes and/or returns the global pointer to the
//               one EffectPool object in the system.
////////////////////////////////////////////////////////////////////
EffectPool *EffectPool::
get_ptr() {
  if (_global_ptr == (EffectPool *)NULL) {
    _global_ptr = new EffectPool;
  }
  return _global_ptr;
}

