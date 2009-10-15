// Filename: physxHeightFieldDesc.I
// Created by:  enn0x (15Oct09)
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
//     Function: PhysxHeightFieldDesc::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxHeightFieldDesc::
PhysxHeightFieldDesc() {

  _samples = NULL;
  _desc.setToDefault();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxHeightFieldDesc::
PhysxHeightFieldDesc(const NxHeightFieldDesc *desc) : _desc(*desc) {

  dealloc_samples();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxHeightFieldDesc::
~PhysxHeightFieldDesc() {

}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::set_to_default
//       Access: Published
//  Description: (re)sets the structure to the default.
////////////////////////////////////////////////////////////////////
INLINE void PhysxHeightFieldDesc::
set_to_default() {

  _desc.setToDefault();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::is_valid
//       Access: Published
//  Description: Returns true if the descriptor is valid.
////////////////////////////////////////////////////////////////////
INLINE bool PhysxHeightFieldDesc::
is_valid() const {

  return _desc.isValid();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::alloc_samples
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void PhysxHeightFieldDesc::
alloc_samples(unsigned int n) {

  if (_samples) {
    dealloc_samples();
  }

  _samples = new NxU32[n];
  _desc.samples = _samples;

}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::dealloc_samples
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void PhysxHeightFieldDesc::
dealloc_samples() {

  if (_samples) {
    _desc.samples = NULL;
    delete[] _samples;
  }
}

