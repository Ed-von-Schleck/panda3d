// Filename: ceguiSupport.h
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

#ifndef CEGUISUPPORT_H
#define CEGUISUPPORT_H

#include "pandabase.h"
#include "nodePath.h"
#include "graphicsWindow.h"
#include "asyncTask.h"

namespace CEGUI {
  class Panda3DRenderer;
}

class EXPCL_PANDACEGUI CeguiSupport {
PUBLISHED:
  static void init(GraphicsWindow *gw,
                   NodePath mouse_and_keyboard,
                   NodePath render_2d,
                   const string &data_path = "./datafiles");

private:
  static void init_cegui_resources(CEGUI::Panda3DRenderer* renderer,
                                   const string &data_path);
  static void setup_input(GraphicsWindow *gw, NodePath mouse_and_kbrd);

  class RenderTask : public AsyncTask {
    AsyncTask::DoneStatus do_task();
  };
};

#endif
