// Filename: config_navigation.h
// Created by:  rdb (20Jul10)
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

#ifndef CONFIG_NAVIGATION_H
#define CONFIG_NAVIGATION_H

#include "pandabase.h"

//FIXME: do this properly in pandabase.h
#define EXPCL_NAVIGATION
#define EXPTP_NAVIGATION

#include "notifyCategoryProxy.h"
#include "dconfig.h"

ConfigureDecl(config_navigation, EXPCL_NAVIGATION, EXPTP_NAVIGATION);
NotifyCategoryDecl(navigation, EXPCL_NAVIGATION, EXPTP_NAVIGATION);

extern EXPCL_NAVIGATION void init_libnavigation();

#endif
