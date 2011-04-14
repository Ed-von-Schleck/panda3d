// Filename: ceguiSupport.cxx
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

#include "nodePathCollection.h"
#include "windowProperties.h"

#include "cegui_includes.h"

#include "ceguiSupport.h"
#include "ceguiPanda3dRenderer.h"
#include "ceguiInputHandler.h"
#include "config_cegui.h"

using namespace CEGUI;

void CeguiSupport::init(GraphicsWindow* gw,
                        NodePath mouse_and_keyboard,
                        NodePath render_2d,
                        const string& data_path) {
  NodePath cegui_np = render_2d.attach_new_node("CeguiNode");
  cegui_np.set_bin("unsorted", 50, 100);
  cegui_np.set_depth_test(false);
  cegui_np.set_depth_write(false);
  cegui_np.set_transparency(TransparencyAttrib::M_alpha);
  const ::WindowProperties& props = ::WindowProperties::get_default();
  Panda3DRenderer* myRenderer = new Panda3DRenderer(cegui_np,
                                                    props.get_x_size(),
                                                    props.get_y_size());
  init_cegui_resources(myRenderer, data_path);
  setup_input(gw, mouse_and_keyboard);

  // Add render task.
  RenderTask *render = new RenderTask();
  AsyncTaskManager::get_global_ptr()->add(render);
}

void CeguiSupport::init_cegui_resources(Panda3DRenderer* renderer,
                                        const string &data_path) {
  System::create(*renderer);
 
  DefaultResourceProvider* rp = 
    static_cast<DefaultResourceProvider*>(System::getSingleton().
                                          getResourceProvider());

  rp->setResourceGroupDirectory("schemes", data_path + "/schemes/");
  rp->setResourceGroupDirectory("imagesets", data_path + "/imagesets/");
  rp->setResourceGroupDirectory("fonts", data_path + "/fonts/");
  rp->setResourceGroupDirectory("layouts", data_path + "/layouts/");
  rp->setResourceGroupDirectory("looknfeels", data_path + "/looknfeel/");
    
  // set the default resource groups to be used
  Imageset::setDefaultResourceGroup("imagesets");
  Font::setDefaultResourceGroup("fonts");

  // load all the fonts (if they are not loaded yet)
  //FontManager::getSingleton().createAll("DejaVuSans-10.font", "fonts");
  FontManager::getSingleton().createFreeTypeFont("DefaultFont",
                                                 12, //pt
                                                 true,
                                                 "DejaVuSans.ttf");
                                                 // "",
                                                 // true,
                                                 // 800, 600);
  // System::getSingleton().setDefaultFont("DefaultFont");
  //System::getSingleton().setDefaultFont("DejaVuSans-10");


  Scheme::setDefaultResourceGroup("schemes");
  WidgetLookManager::setDefaultResourceGroup("looknfeels");
  CEGUI::WindowManager::setDefaultResourceGroup("layouts");

  // load scheme and set up defaults
  SchemeManager::getSingleton().create("TaharezLook.scheme");

  System::getSingleton().setDefaultMouseCursor("TaharezLook","MouseArrow");
  System::getSingleton().setDefaultTooltip("TaharezLook/Tooltip");
}

void CeguiSupport::setup_input(GraphicsWindow *gw, NodePath mouse_and_kbrd) {
  PT(CeguiInputHandler) cegui_injector =
    new CeguiInputHandler("cegui_input");
  ::WindowProperties props;
  props.set_cursor_hidden(true);
  gw->request_properties(props);
  NodePathCollection children = mouse_and_kbrd.get_children();
  NodePath cegui_np = mouse_and_kbrd.attach_new_node(cegui_injector);
  children.reparent_to(cegui_np);
}

AsyncTask::DoneStatus CeguiSupport::RenderTask::
do_task() {
  //nout << "render" << endl;
  CEGUI::System::getSingleton().renderGUI();
  return DS_cont;
}