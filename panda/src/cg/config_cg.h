// Filename: config_cg.h
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

#ifndef CONFIG_CG_H
#define CONFIG_CG_H

#include "pandabase.h"

#include "notifyCategoryProxy.h"
#include "dconfig.h"

ConfigureDecl(config_cg, EXPCL_CG, EXPTP_CG);
NotifyCategoryDecl(cg, EXPCL_CG, EXPTP_CG);

extern EXPCL_CG void init_libcg();

#endif

