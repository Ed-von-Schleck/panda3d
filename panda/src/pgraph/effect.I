// Filename: effect.I
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


////////////////////////////////////////////////////////////////////
//     Function: Effect::Constructor
//       Access: Published
//  Description: Constructs a new Effect that can be placed in the
//               scene graph.
////////////////////////////////////////////////////////////////////
INLINE Effect::
Effect(const string &name) : PandaNode(name) {
  set_cull_callback();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::has_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE bool Effect::
has_technique(CPT(InternalName) name) {
  return (bool) _techniques.count(name);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::get_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE CPT(Technique) Effect::
get_technique(CPT(InternalName) name) const {
  Techniques::const_iterator it = _techniques.find(name);
  nassertr(it != _techniques.end(), NULL);
  return it->second;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::set_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void Effect::
set_technique(CPT(InternalName) name, PT(Technique) technique) {
  _techniques[name] = technique;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::remove_technique
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void Effect::
remove_technique(CPT(InternalName) name) {
  Techniques::iterator it = _techniques.find(name);
  nassertv(it != _techniques.end());
  _techniques.erase(it);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::has_filename
//       Access: Published
//  Description: Returns true if the filename has been set and
//               is available.  See set_filename().
////////////////////////////////////////////////////////////////////
INLINE bool Effect::
has_filename() const {
  return !_filename.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::get_filename
//       Access: Published
//  Description: Returns the filename that has been set.  This is the
//               name of the file as it was requested.  Also see
//               get_fullpath().
////////////////////////////////////////////////////////////////////
INLINE const Filename &Effect::
get_filename() const {
  return _filename;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::set_filename
//       Access: Published
//  Description: Sets the name of the file that contains the effect's
//               contents.  Normally, this is set automatically when
//               the effect is loaded, for instance via
//               Effect::read().
////////////////////////////////////////////////////////////////////
INLINE void Effect::
set_filename(const Filename &filename) {
  _filename = filename;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::clear_filename
//       Access: Published
//  Description: Removes the filename, if it was previously set.
//               See set_filename().
////////////////////////////////////////////////////////////////////
INLINE void Effect::
clear_filename() {
  _filename = Filename();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::has_fullpath
//       Access: Published
//  Description: Returns true if the fullpath has been set and
//               is available.  See set_fullpath().
////////////////////////////////////////////////////////////////////
INLINE bool Effect::
has_fullpath() const {
  return !_fullpath.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::get_fullpath
//       Access: Published
//  Description: Returns the fullpath that has been set.  This is the
//               full path to the file as it was found along the
//               effect search path.
////////////////////////////////////////////////////////////////////
INLINE const Filename &Effect::
get_fullpath() const {
  return _fullpath;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::set_fullpath
//       Access: Published
//  Description: Sets the full pathname to the file that contains the
//               effect's contents, as found along the search path.
//               Normally, this is set automatically when the image is
//               loaded, for instance via Effect::read().
////////////////////////////////////////////////////////////////////
INLINE void Effect::
set_fullpath(const Filename &fullpath) {
  _fullpath = fullpath;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::clear_fullpath
//       Access: Published
//  Description: Removes the fullpath, if it was previously set.
//               See set_fullpath().
////////////////////////////////////////////////////////////////////
INLINE void Effect::
clear_fullpath() {
  _fullpath = Filename();
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::make_effect
//       Access: Public, Static
//  Description: A factory function to make a new Effect, used to
//               pass to the EffectPool.
////////////////////////////////////////////////////////////////////
INLINE PT(Effect) Effect::
make_effect() {
  return new Effect;
}

