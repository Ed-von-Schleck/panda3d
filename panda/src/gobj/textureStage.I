// Filename: textureStage.I
// Created by:  masad (15Jul04)
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
//     Function: TextureStage::get_name
//       Access: Published
//  Description: Returns the name of this texture stage
////////////////////////////////////////////////////////////////////
INLINE const string &TextureStage::
get_name() const {
  return _name;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_sort
//       Access: Published
//  Description: Returns the sort order of this texture stage
////////////////////////////////////////////////////////////////////
INLINE int TextureStage::
get_sort() const {
  return _sort;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_priority
//       Access: Published
//  Description: Returns the priority associated with this stage.
//               This is specially helpful for cards that do not
//               support more than n stages of multi-texturing
////////////////////////////////////////////////////////////////////
INLINE int TextureStage::
get_priority() const {
  return _priority;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::opertor <
//       Access: Published
//  Description: Compare if the sort order is lower
////////////////////////////////////////////////////////////////////
INLINE bool TextureStage::
operator < (const TextureStage &other) const {
  return (_sort < other._sort);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_texcoord_name
//       Access: Published
//  Description: Set the TexCoordName
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_texcoord_name(const TexCoordName *name) {
  _texcoord_name = name;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_texcoord_name
//       Access: Published
//  Description: Returns the TexCoordName
////////////////////////////////////////////////////////////////////
INLINE const TexCoordName *TextureStage::
get_texcoord_name() const {
  return _texcoord_name;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_mode
//       Access: Published
//  Description: Set the mode of this texture stage
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_mode(TextureStage::Mode mode) {
  _mode = mode;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_mode
//       Access: Published
//  Description: Return the mode of this stage
////////////////////////////////////////////////////////////////////
INLINE TextureStage::Mode TextureStage::
get_mode() const {
  return _mode;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_color
//       Access: Published
//  Description: Set the color for this stage
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_color(const Colorf &color) {
  _color = color;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_color
//       Access: Published
//  Description: return the color for this stage
////////////////////////////////////////////////////////////////////
INLINE Colorf TextureStage::
get_color() const {
  return _color;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_rgb
//       Access: Published
//  Description: Set combine_rgb_mode with source and operand
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_rgb(CombineMode mode, 
                CombineSource source0, CombineOperand operand0) {
  _combine_rgb_mode = mode;
  _combine_rgb_source0 = source0;
  _combine_rgb_operand0 = operand0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_rgb
//       Access: Published
//  Description: Set combine_rgb_mode with two sources and operands
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_rgb(CombineMode mode, 
                CombineSource source0, CombineOperand operand0,
                CombineSource source1, CombineOperand operand1) {
  _combine_rgb_mode = mode;
  _combine_rgb_source0 = source0;
  _combine_rgb_operand0 = operand0;
  _combine_rgb_source1 = source1;
  _combine_rgb_operand1 = operand1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_rgb
//       Access: Published
//  Description: Set combine_rgb_mode with three sources and operands
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_rgb(CombineMode mode, 
                CombineSource source0, CombineOperand operand0,
                CombineSource source1, CombineOperand operand1,
                CombineSource source2, CombineOperand operand2) {
  _combine_rgb_mode = mode;
  _combine_rgb_source0 = source0;
  _combine_rgb_operand0 = operand0;
  _combine_rgb_source1 = source1;
  _combine_rgb_operand1 = operand1;
  _combine_rgb_source2 = source2;
  _combine_rgb_operand2 = operand2;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_mode
//       Access: Published
//  Description: Get the combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineMode TextureStage::
get_combine_rgb_mode() const {
  return _combine_rgb_mode;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_source0
//       Access: Published
//  Description: Get source0 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_rgb_source0() const {
  return _combine_rgb_source0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_operand0
//       Access: Published
//  Description: Get operand0 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_rgb_operand0() const {
  return _combine_rgb_operand0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_source1
//       Access: Published
//  Description: Get source1 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_rgb_source1() const {
  return _combine_rgb_source1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_operand1
//       Access: Published
//  Description: Get operand1 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_rgb_operand1() const {
  return _combine_rgb_operand1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_source2
//       Access: Published
//  Description: Get source2 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_rgb_source2() const {
  return _combine_rgb_source2;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_rgb_operand2
//       Access: Published
//  Description: Get operand2 of combine_rgb_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_rgb_operand2() const {
  return _combine_rgb_operand2;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_alpha
//       Access: Published
//  Description: Set combine_alpha_mode with single source and operand
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_alpha(CombineMode mode, 
                  CombineSource source0, CombineOperand operand0) {
  _combine_alpha_mode = mode;
  _combine_alpha_source0 = source0;
  _combine_alpha_operand0 = operand0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_alpha
//       Access: Published
//  Description: Set combin_alpha_mode with two sources and operands
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_alpha(CombineMode mode, 
                  CombineSource source0, CombineOperand operand0,
                  CombineSource source1, CombineOperand operand1) {
  _combine_alpha_mode = mode;
  _combine_alpha_source0 = source0;
  _combine_alpha_operand0 = operand0;
  _combine_alpha_source1 = source1;
  _combine_alpha_operand1 = operand1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::set_combine_alpha
//       Access: Published
//  Description: Set combine_alpha_mode with three sources and operands
////////////////////////////////////////////////////////////////////
INLINE void TextureStage::
set_combine_alpha(CombineMode mode, 
                  CombineSource source0, CombineOperand operand0,
                  CombineSource source1, CombineOperand operand1,
                  CombineSource source2, CombineOperand operand2) {
  _combine_alpha_mode = mode;
  _combine_alpha_source0 = source0;
  _combine_alpha_operand0 = operand0;
  _combine_alpha_source1 = source1;
  _combine_alpha_operand1 = operand1;
  _combine_alpha_source2 = source2;
  _combine_alpha_operand2 = operand2;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha
//       Access: Published
//  Description: Get combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineMode TextureStage::
get_combine_alpha_mode() const {
  return _combine_alpha_mode;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_source0
//       Access: Published
//  Description: Get source0 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_alpha_source0() const {
  return _combine_alpha_source0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_operand0
//       Access: Published
//  Description: Get operand0 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_alpha_operand0() const {
  return _combine_alpha_operand0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_source1
//       Access: Published
//  Description: Get source1 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_alpha_source1() const {
  return _combine_alpha_source1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_operand1
//       Access: Published
//  Description: Get operand1 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_alpha_operand1() const {
  return _combine_alpha_operand1;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_source2
//       Access: Published
//  Description: Get source2 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineSource TextureStage::
get_combine_alpha_source2() const {
  return _combine_alpha_source2;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::get_combine_alpha_operand2
//       Access: Published
//  Description: Get operand2 of combine_alpha_mode
////////////////////////////////////////////////////////////////////
INLINE TextureStage::CombineOperand TextureStage::
get_combine_alpha_operand2() const {
  return _combine_alpha_operand2;
}

