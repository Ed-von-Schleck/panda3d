// Filename: textureAttrib.I
// Created by:  drose (21Feb02)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::Constructor
//       Access: Private
//  Description: Use TextureAttrib::make() to construct a new
//               TextureAttrib object.
////////////////////////////////////////////////////////////////////
INLINE TextureAttrib::
TextureAttrib() {
  _operation = O_set;
  _sort_seq = UpdateSeq::old();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::is_off
//       Access: Published
//  Description: Returns true if the TextureAttrib is an 'off'
//               TextureAttrib, indicating that it should disable
//               texturing.
//
//               This method is deprecated, and is provided for
//               backward compatibility; you should use the
//               multitexture form of this instead.
////////////////////////////////////////////////////////////////////
INLINE bool TextureAttrib::
is_off() const {
  return is_all_off();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_texture
//       Access: Published
//  Description: If the TextureAttrib is not an 'off' TextureAttrib,
//               returns the texture that is associated.  Otherwise,
//               return NULL.
//
//               This method is deprecated, and is provided for
//               backward compatibility; you should use the
//               multitexture form of this instead.
////////////////////////////////////////////////////////////////////
INLINE Texture *TextureAttrib::
get_texture() const {
  TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
  return get_texture(tex_mgr->get_default_stage());
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_operation
//       Access: Published
//  Description: Returns the basic operation type of the TextureAttrib.
//               If this is O_set, the stages listed here completely
//               replace any stages that were already on.  If this is
//               O_add, the stages here are added to the set of of
//               stages that were already on, and if O_remove, the
//               stages here are removed from the set of stages that
//               were on.
////////////////////////////////////////////////////////////////////
INLINE TextureAttrib::Operation TextureAttrib::
get_operation() const {
  return _operation;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_num_stages
//       Access: Published
//  Description: Returns the number of stages listed in the attribute.
////////////////////////////////////////////////////////////////////
INLINE int TextureAttrib::
get_num_stages() const {
  check_sorted();
  return _stages.size();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_stage
//       Access: Published
//  Description: Returns the nth stage listed in the attribute, sorted
//               in render order.
////////////////////////////////////////////////////////////////////
INLINE TextureStage *TextureAttrib::
get_stage(int n) const {
  nassertr(n >= 0 && n < (int)_stages.size(), (TextureStage *)NULL);
  return _stages[n];
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::has_stage
//       Access: Published
//  Description: Returns true if the indicated stage is listed in the
//               attrib, false otherwise.
////////////////////////////////////////////////////////////////////
INLINE bool TextureAttrib::
has_stage(TextureStage *stage) const {
  return _textures.find(stage) != _textures.end();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_texture
//       Access: Published
//  Description: Returns the texture associated with the indicated
//               stage, or NULL if no texture is associated.
////////////////////////////////////////////////////////////////////
INLINE Texture *TextureAttrib::
get_texture(TextureStage *stage) const {
  Textures::const_iterator ti;
  ti = _textures.find(stage);
  if (ti != _textures.end()) {
    return (*ti).second;
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::add_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage added to the list of stages.
////////////////////////////////////////////////////////////////////
INLINE CPT(RenderAttrib) TextureAttrib::
add_stage(TextureStage *stage, Texture *tex) const {
  if (_operation == O_remove) {  
    return compose(make(O_remove, stage, tex));
  } else {
    return compose(make(O_add, stage, tex));
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::remove_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage removed from the list of
//               stages.
////////////////////////////////////////////////////////////////////
INLINE CPT(RenderAttrib) TextureAttrib::
remove_stage(TextureStage *stage) const {
  if (_operation == O_remove) {  
    return compose(make(O_add, stage, NULL));
  } else {
    return compose(make(O_remove, stage, NULL));
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::is_identity
//       Access: Published
//  Description: Returns true if this is an identity attrib: it does
//               not change the set of stages in use.
////////////////////////////////////////////////////////////////////
INLINE bool TextureAttrib::
is_identity() const {
  return _operation != O_set && _stages.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::is_all_off
//       Access: Published
//  Description: Returns true if this attrib turns off all stages and
//               turns none on.
////////////////////////////////////////////////////////////////////
INLINE bool TextureAttrib::
is_all_off() const {
  return _operation == O_set && _stages.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::get_stages
//       Access: Published
//  Description: Returns the complete list of stages defined within
//               this TextureAttrib, suitable for passing to
//               Geom::setup_multitexcoord_iterator().
////////////////////////////////////////////////////////////////////
INLINE const Geom::ActiveTextureStages &TextureAttrib::
get_stages() const {
  return _stages;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::check_sorted
//       Access: Private
//  Description: Confirms whether the _stages list is still sorted.
//               It will become unsorted if someone calls
//               TextureStage::set_sort().
//
//               If the list requires sorting, transparently sorts it
//               before returning.
////////////////////////////////////////////////////////////////////
INLINE void TextureAttrib::
check_sorted() const {
  TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
  if (_sort_seq != tex_mgr->get_sort_seq()) {
    ((TextureAttrib *)this)->sort_stages();
  }
}
