// Filename: effectPool.I
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


////////////////////////////////////////////////////////////////////
//     Function: EffectPool::Constructor
//       Access: Private
//  Description: The constructor is not intended to be called
//               directly; there's only supposed to be one EffectPool
//               in the universe and it constructs itself.
////////////////////////////////////////////////////////////////////
INLINE EffectPool::
EffectPool() {
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::has_effect
//       Access: Public, Static
//  Description: Returns true if the effect has ever been loaded;
//               false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool EffectPool::
has_effect(const Filename &filename) {
  return get_global_ptr()->ns_has_effect(filename);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::verify_effect
//       Access: Public, Static
//  Description: Loads the given filename up as an effect, if it has
//               not already been loaded, and returns true to indicate
//               success, or false to indicate failure. If this
//               returns true, it is guaranteed that a subsequent call
//               to load_effect() with the same effect name will
//               return a valid Effect pointer.
////////////////////////////////////////////////////////////////////
INLINE bool EffectPool::
verify_effect(const Filename &filename) {
  return load_effect(filename) != (Effect *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::load_effect
//       Access: Public, Static
//  Description: Loads the given filename up as an effect, if it has
//               not already been loaded, and returns the new effect.
//               If an effect with the same filename was previously
//               loaded, returns that one instead. If the effect
//               file cannot be found, returns NULL.
////////////////////////////////////////////////////////////////////
INLINE Effect *EffectPool::
load_effect(const Filename &filename, const LoaderOptions &options) {
  return get_global_ptr()->ns_load_effect(filename, options);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::add_effect
//       Access: Public, Static
//  Description: Adds the indicated already-loaded effect to the
//               pool.  The effect will always replace any
//               previously-loaded effect in the pool that had the
//               same filename.
//
//               This two-parameter version of this method is
//               deprecated; use the one-parameter add_effect(effect)
//               instead.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
add_effect(const Filename &filename, Effect *effect) {
  get_global_ptr()->ns_add_effect(filename, effect);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::release_effect
//       Access: Public, Static
//  Description: Removes the indicated effect from the pool,
//               indicating it will never be loaded again; the effect
//               may then be freed.  If this function is never called,
//               a reference count will be maintained on every effect
//               every loaded, and effects will never be freed.
//
//               This version of this method is deprecated; use
//               release_effect(effect) instead.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
release_effect(const Filename &filename) {
  get_global_ptr()->ns_release_effect(filename);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::add_effect
//       Access: Public, Static
//  Description: Adds the indicated already-loaded effect to the
//               pool.  The effect will always replace any
//               previously-loaded effect in the pool that had the
//               same filename.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
add_effect(Effect *effect) {
  get_global_ptr()->ns_add_effect(effect);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::release_effect
//       Access: Public, Static
//  Description: Removes the indicated effect from the pool,
//               indicating it will never be loaded again; the effect
//               may then be freed.  If this function is never called,
//               a reference count will be maintained on every effect
//               every loaded, and effects will never be freed.
//
//               The effect's get_fullpath() value should not have been
//               changed during its lifetime, or this function may
//               fail to locate it in the pool.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
release_effect(Effect *effect) {
  get_global_ptr()->ns_release_effect(effect);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::release_all_effects
//       Access: Public, Static
//  Description: Releases all effects in the pool and restores the
//               pool to the empty state.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
release_all_effects() {
  get_global_ptr()->ns_release_all_effects();
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::garbage_collect
//       Access: Public, Static
//  Description: Releases only those effects in the pool that have a
//               reference count of exactly 1; i.e. only those
//               effects that are not being used outside of the pool.
//               Returns the number of effects released.
////////////////////////////////////////////////////////////////////
INLINE int EffectPool::
garbage_collect() {
  return get_global_ptr()->ns_garbage_collect();
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::list_contents
//       Access: Public, Static
//  Description: Lists the contents of the effect pool to the
//               indicated output stream.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
list_contents(ostream &out) {
  get_global_ptr()->ns_list_contents(out);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectPool::list_contents
//       Access: Public, Static
//  Description: Lists the contents of the effect pool to cout.
////////////////////////////////////////////////////////////////////
INLINE void EffectPool::
list_contents() {
  get_global_ptr()->ns_list_contents(cout);
}

