// Filename: wavAudioCursor.I
// Created by: gogg (20Mar2010)
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
//     Function: MovieAudioCursor::is_valid
//       Access: Public
//  Description: Returns true if the file has been successfully
//               loaded.
////////////////////////////////////////////////////////////////////
INLINE bool WavAudioCursor::
is_valid() const {
  return _opened;
}
