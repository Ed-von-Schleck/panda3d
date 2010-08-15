// Filename: technique.I
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
//     Function: Technique::get_num_render_passes
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE int Technique::
get_num_render_passes() const {
  return _render_passes.size();
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::get_render_pass
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE CPT(RenderPass) Technique::
get_render_pass(int i) const {
  nassertr(_render_passes.size() >= i, NULL);
  return _render_passes[i];
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::modify_render_pass
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE PT(RenderPass) Technique::
modify_render_pass(int i) {
  
}

