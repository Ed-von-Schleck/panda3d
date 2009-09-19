// Filename: physxTriggerReport.cxx
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

#include "physxTriggerReport.h"

#include "eventQueue.h"
#include "eventParameter.h"

////////////////////////////////////////////////////////////////////
//     Function : PhysxTriggerReport::clear_events
//       Access : Public
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxTriggerReport::
clear_events() {

  events.clear();
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxTriggerReport::throw_events
//       Access : Public
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxTriggerReport::
throw_events() {

  pvector<Event *>::const_iterator it;
  for (it = events.begin(); it != events.end(); ++it) {
    EventQueue::get_global_event_queue( )->queue_event(*it);
  }
}

////////////////////////////////////////////////////////////////////
//     Function : PhysxTriggerReport::onTrigger
//       Access : Public
//  Description :
////////////////////////////////////////////////////////////////////
void PhysxTriggerReport::
onTrigger(NxShape &triggerShape, NxShape &otherShape, NxTriggerFlag status) {

  Event *event;

  if (status & NX_TRIGGER_ON_ENTER) {
    event = new Event("physx-trigger-enter");
  }
  else if (status & NX_TRIGGER_ON_LEAVE) {
    event = new Event("physx-trigger-leave");
  }
  else if (status & NX_TRIGGER_ON_STAY) {
    event = new Event("physx-trigger-stay");
  }
  else {
    return;
  }

  PT(PhysxShape) pTriggerShape = (PhysxShape *)triggerShape.userData;
  PT(PhysxShape) pOtherShape = (PhysxShape *)otherShape.userData;

  event->add_parameter(EventParameter(pTriggerShape));
  event->add_parameter(EventParameter(pOtherShape));
  events.push_back(event);
}

