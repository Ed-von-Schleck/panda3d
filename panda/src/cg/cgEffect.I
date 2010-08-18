// Filename: cgEffect.I
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
//     Function: CgEffect::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE CgEffect::
CgEffect(const string &name) :
  Effect(name)
{
}

////////////////////////////////////////////////////////////////////
//     Function: CgEffect::make_effect
//       Access: Public, Static
//  Description: A factory function to make a new CgEffect, used to
//               pass to the EffectPool.
////////////////////////////////////////////////////////////////////
INLINE PT(CgEffect) CgEffect::
make_effect() {
  return new CgEffect;
}
