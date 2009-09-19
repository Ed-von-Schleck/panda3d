// Filename: physxContactReport.h
// Created by:  enn0x (19Sep09)
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

#ifndef PHYSXCONTACTREPORT_H
#define PHYSXCONTACTREPORT_H

#include "pandabase.h"
#include "pvector.h"
#include "event.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxContactHandler
// Description :
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxContactReport : public NxUserContactReport {

public:
  INLINE PhysxContactReport();
  INLINE ~PhysxContactReport();

  void clear_events();
  void throw_events();

  void onContactNotify(NxContactPair& pair, NxU32 flags);

private:
  pvector<Event *> events;
};

#include "physxContactReport.I"

#endif // PHYSXCONTACTREPORT_H
