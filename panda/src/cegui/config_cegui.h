// Filename: config_cegui.h
// Created by:  Nik (Oct10)
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

#ifndef CONFIG_CEGUI_H
#define CONFIG_CEGUI_H

#include "pandabase.h"
/* #include "notifyCategoryProxy.h" */

#include "dconfig.h"

/* This is needed to enable cegui_cat. */
/* NotifyCategoryDecl(cegui, EXPCL_PANDACEGUI, EXPTP_PANDACEGUI); */

extern EXPCL_PANDACEGUI void init_libcegui();

#endif /* CONFIG_CEGUI_H */
