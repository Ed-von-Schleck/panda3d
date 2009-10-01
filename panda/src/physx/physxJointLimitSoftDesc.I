// Filename: physxJointLimitSoftDesc.I
// Created by:  enn0x (01Oct09)
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
//     Function: PhysxJointLimitSoftDesc::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxJointLimitSoftDesc::
PhysxJointLimitSoftDesc() {

}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJointLimitSoftDesc::Constructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxJointLimitSoftDesc::
PhysxJointLimitSoftDesc(float value, float restitution, float spring, float damping) {

  _desc.value = value;
  _desc.restitution = restitution;
  _desc.spring = spring;
  _desc.damping = damping;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJointLimitSoftDesc::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxJointLimitSoftDesc::
PhysxJointLimitSoftDesc(const NxJointLimitSoftDesc &desc) {

  _desc = desc;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJointLimitSoftDesc::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
INLINE PhysxJointLimitSoftDesc::
~PhysxJointLimitSoftDesc() {

}

////////////////////////////////////////////////////////////////////
//     Function: PhysxJointLimitSoftDesc::get_desc
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
INLINE NxJointLimitSoftDesc PhysxJointLimitSoftDesc::
desc() const {

  return _desc;
}

