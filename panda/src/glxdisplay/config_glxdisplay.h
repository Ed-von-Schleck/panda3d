// Filename: config_glxdisplay.h
// Created by:  cary (07Oct99)
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

#ifndef __CONFIG_GLXDISPLAY_H__
#define __CONFIG_GLXDISPLAY_H__

#include "pandabase.h"
#include "notifyCategoryProxy.h"
#include "configVariableString.h"
#include "configVariableBool.h"
#include "configVariableInt.h"

NotifyCategoryDecl(glxdisplay, EXPCL_PANDAGL, EXPTP_PANDAGL);

extern EXPCL_PANDAGL void init_libglxdisplay();

extern ConfigVariableString display_cfg;
extern ConfigVariableBool x_error_abort;
extern ConfigVariableBool glx_get_proc_address;
extern ConfigVariableBool glx_get_os_address;

extern ConfigVariableInt x_wheel_up_button;
extern ConfigVariableInt x_wheel_down_button;
extern ConfigVariableInt x_wheel_left_button;
extern ConfigVariableInt x_wheel_right_button;

#endif /* __CONFIG_GLXDISPLAY_H__ */
