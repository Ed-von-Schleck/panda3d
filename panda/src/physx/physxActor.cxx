// Filename: physxActor.cxx
// Created by:  enn0x (14Sep09)
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

#include "physxActor.h"
#include "physxManager.h"

TypeHandle PhysxActor::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::link
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
link(NxActor *actorPtr) {

  // TODO: link shapes

  ref();
  _ptr = actorPtr;
  _error_type = ET_ok;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::unlink
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
unlink() {

  // TODO: unlink shapes etc.

  _error_type = ET_released;
  unref();
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::release
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxActor::
release() {

  nassertv(_error_type == ET_ok);

  unlink();
  _ptr->getScene().releaseActor(*_ptr);
  _ptr = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::set_name
//       Access: Published
//  Description: Sets a name string for the object that can be
//               retrieved with get_name(). 
//               This is for debugging and is not used by the
//               engine.
////////////////////////////////////////////////////////////////////
void PhysxActor::
set_name( const char *name ) {

  nassertv( _error_type == ET_ok );
  _ptr->setName( name );
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxActor::get_name
//       Access: Published
//  Description: Retrieves the name string.
////////////////////////////////////////////////////////////////////
const char *PhysxActor::
get_name() const {

  nassertr( _error_type == ET_ok, "" );
  return _ptr->getName();
}

