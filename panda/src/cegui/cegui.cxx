// Filename: cegui.cxx
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

// This is an example/test file that uses CEGUI from C++.


#include "pandaFramework.h"
#include "pandaSystem.h"

#include "ceguiInputHandler.h"
#include "ceguiSupport.h"

#include <CEGUI/CEGUI.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>

void create_gui() {
  using namespace CEGUI;
  WindowManager& winMgr = WindowManager::getSingleton();
  Window* root = winMgr.createWindow("DefaultWindow", "Root");
  root->setMousePassThroughEnabled(true);
  System::getSingleton().setGUISheet(root);
  Window* wnd = WindowManager::getSingleton().
      loadWindowLayout("TabControlDemo.layout");
  root->addChildWindow(wnd);
}


int main(int argc, char *argv[]) {
  PandaFramework framework;

  //open a new window framework
  framework.open_framework(argc, argv);
  //set the window title to My Panda3D Window
  framework.set_window_title("My Panda3D Window");
  
  //open the window
  WindowFramework *window = framework.open_window();

  //check whether the window is loaded correctly
  if (window != (WindowFramework *)NULL) {
    nout << "Opened the window successfully!\n";

    window->enable_keyboard(); //enable keyboard detection
    window->setup_trackball(); //enable default camera movement

    // load the box model
    NodePath box = window->load_model(framework.get_models(),
                       "/home/nik/Code/panda3d/project/models/cube.egg");
    box.reparent_to(window->get_render());
    // engine.set_bin("fixed", 0);

    GraphicsWindow *gw = window->get_graphics_window();
    // We can't pass a WindowFramework here because it's C++ only.
    CeguiSupport::init(gw,
                       window->get_panda_framework()->get_mouse(gw),
                       window->get_render_2d(),
                       "/usr/local/share/CEGUI/");
    create_gui();

    //do the main loop
    framework.main_loop();
  } else {
    nout << "Could not load the window!\n";
  }

  //close the window framework
  framework.close_framework();
  return (0);
}
