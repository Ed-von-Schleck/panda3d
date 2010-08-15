// Filename: effectFile.I
// Created by: Xidram (15Aug10)
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
//     Function: EffectFile::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE EffectFile::
EffectFile(const string &name) :
  Effect(name),
  _fullpath(name),
  _reference(new EffectFile::EffectReference)
{
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::get_effect_ref_count
//       Access: Published
//  Description: Returns the number of copies that exist of this
//               particular EffectFile node.  Each time
//               EffectFile::copy_subgraph() or make_copy() is called
//               (or some other copying mechanism, such as
//               NodePath.copy_to(), is used), this count will
//               increment by one in all copies; when one of the
//               copies is destructed, this count will decrement.
////////////////////////////////////////////////////////////////////
INLINE int EffectFile::
get_effect_ref_count() const {
  return _reference->get_ref_count();
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::get_fullpath
//       Access: Published
//  Description: Returns the full pathname of the effect represented by
//               this node, as found on disk.  This is mainly useful
//               for reference purposes, but is also used to index the
//               EffectFile into the EffectPool.
////////////////////////////////////////////////////////////////////
INLINE const Filename &EffectFile::
get_fullpath() const {
  return _fullpath;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::set_fullpath
//       Access: Published
//  Description: Sets the full pathname of the effect represented by
//               this node, as found on disk.  This is mainly useful
//               for reference purposes, but is also used to index the
//               EffectFile into the EffectPool.
//
//               This is normally set automatically when an effect is
//               loaded, and should not be set directly by the user.
//               If you change this on a loaded effect, then
//               EffectPool::release_effect() may fail.
////////////////////////////////////////////////////////////////////
INLINE void EffectFile::
set_fullpath(const Filename &fullpath) {
  _fullpath = fullpath;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::get_reference
//       Access: Published
//  Description: Returns the pointer that represents the object shared
//               between all copies of this EffectFile.  Since there's
//               not much associated with this object other than a
//               reference count, normally there's not much reason to
//               get the pointer (though it may be compared
//               pointerwise with other EffectFile objects).
////////////////////////////////////////////////////////////////////
INLINE EffectFile::EffectReference *EffectFile::
get_reference() const {
  return _reference;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::set_reference
//       Access: Published
//  Description: Changes the pointer that represents the object shared
//               between all copies of this EffectFile.  This will
//               disassociate this EffectFile from all of its copies.
//               Normally, there's no reason to do this.
////////////////////////////////////////////////////////////////////
INLINE void EffectFile::
set_reference(EffectFile::EffectReference *ref) {
  _reference = ref;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::Copy Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
INLINE EffectFile::
EffectFile(const EffectFile &copy) :
  Effect(copy),
  _fullpath(copy._fullpath),
  _reference(copy._reference)
{
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::EffectReference::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE EffectFile::EffectReference::
EffectReference() {
}
