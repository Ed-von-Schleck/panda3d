// Filename: queryContext.cxx
// Created by:  drose (27Mar06)
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

#include "queryContext.h"

TypeHandle QueryContext::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: QueryContext::Destructor
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
QueryContext::
~QueryContext() {
}

////////////////////////////////////////////////////////////////////
//     Function: QueryContext::is_answer_ready
//       Access: Public, Virtual
//  Description: Returns true if the query's answer is ready, false
//               otherwise.  If this returns false, the application
//               must continue to poll until it returns true.
//
//               It is only valid to call this from the draw thread.
////////////////////////////////////////////////////////////////////
bool QueryContext::
is_answer_ready() const {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: QueryContext::waiting_for_answer
//       Access: Public, Virtual
//  Description: Requests the graphics engine to expedite the pending
//               answer--the application is now waiting until the
//               answer is ready.
//
//               It is only valid to call this from the draw thread.
////////////////////////////////////////////////////////////////////
void QueryContext::
waiting_for_answer() {
}
