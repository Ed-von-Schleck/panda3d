// Filename: renderPass.I
// Created by:  rdb (13Aug10)
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
//     Function: RenderPass::Constructor
//       Access: Public
//  Description: Constructs a new RenderPass using the indicated
//               name and draw type.
////////////////////////////////////////////////////////////////////
INLINE RenderPass::
RenderPass(const string &name, DrawType draw_type) :
  Namable(name), _draw_type(draw_type) {}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::set_draw_type
//       Access: Public
//  Description: Sets the draw type, which defines what will be
//               drawn when rendering this pass.
////////////////////////////////////////////////////////////////////
INLINE void RenderPass::
set_draw_type(RenderPass::DrawType draw_type) {
  _draw_type = draw_type;
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::get_draw_type
//       Access: Public
//  Description: Returns the draw type, which defines what will be
//               drawn when rendering this pass.
////////////////////////////////////////////////////////////////////
INLINE RenderPass::DrawType RenderPass::
get_draw_type() const {
  return _draw_type;
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::get_state
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE CPT(RenderState) RenderPass::
get_state() const {
  return _state;
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::set_state
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void RenderPass::
set_state(CPT(RenderState) state) {
  _state = state;
}

