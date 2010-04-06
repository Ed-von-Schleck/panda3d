// Filename: config_collada.h
// Created by:  rdb (23Aug09)
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

#ifndef CONFIG_COLLADA_H
#define CONFIG_COLLADA_H

#include "pandabase.h"

#include "coordinateSystem.h"
#include "notifyCategoryProxy.h"
#include "dconfig.h"

ConfigureDecl(config_collada, EXPCL_PANDACOLLADA, EXPTP_PANDACOLLADA);
NotifyCategoryDecl(collada, EXPCL_PANDACOLLADA, EXPTP_PANDACOLLADA);

extern EXPCL_PANDACOLLADA ConfigVariableBool collada_flatten;
extern EXPCL_PANDACOLLADA ConfigVariableBool collada_unify;
extern EXPCL_PANDACOLLADA ConfigVariableDouble collada_flatten_radius;
extern EXPCL_PANDACOLLADA ConfigVariableBool collada_combine_geoms;
extern EXPCL_PANDACOLLADA ConfigVariableBool collada_accept_errors;

extern EXPCL_PANDACOLLADA void init_libcollada();

#endif
