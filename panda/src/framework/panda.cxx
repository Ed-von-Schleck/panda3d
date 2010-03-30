// Filename: panda.cxx
// Created by:  gogg (26Mar10)
// (Heavily based on the Framework classes by drose (2002))
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

#include "panda.h"
#include "clockObject.h"
#include "pStatClient.h"
#include "eventQueue.h"
#include "dataGraphTraverser.h"
#include "depthOffsetAttrib.h"
#include "collisionNode.h"
#include "config_framework.h"
#include "graphicsPipeSelection.h"
#include "nodePathCollection.h"
#include "textNode.h"
#include "mouseAndKeyboard.h"
#include "throw_event.h"
#include "executionEnvironment.h"
#include "sceneGraphAnalyzer.h"

LoaderOptions Panda::_loader_options;

////////////////////////////////////////////////////////////////////
//     Function: Panda::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
Panda::
Panda() :
  _event_handler(*EventHandler::get_global_event_handler()),
  _task_mgr(*AsyncTaskManager::get_global_ptr())
{
  _made_default_pipe = false;
  _engine = (GraphicsEngine *)NULL;

  _start_time = 0.0;
  _frame_count = 0;
  _fps = 0;

  _exit_flag = false;

  do_initialize();
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::Destructor
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
Panda::
~Panda() {
  do_finalize();
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::do_initialize
//       Access: Private
//  Description: Called by the constructor to perform
//               initialization.
////////////////////////////////////////////////////////////////////
void Panda::
do_initialize() {

#ifdef LINK_ALL_STATIC
  // If we're statically linking, we need to explicitly link with
  // at least one of the available renderers.
  #ifdef HAVE_GL
  extern EXPCL_PANDAGL void init_libpandagl();
  init_libpandagl();
  #elif HAVE_DX9
  extern EXPCL_PANDADX9 void init_libpandadx9();
  init_libpandadx9();
  #elif HAVE_DX8
  extern EXPCL_PANDADX8 void init_libpandadx8();
  init_libpandadx8();
  #elif HAVE_TINYDISPLAY
  extern EXPCL_TINYDISPLAY void init_libtinydisplay();
  init_libtinydisplay();
  #endif

  // Get the available image types too.
  extern EXPCL_PANDA_PNMIMAGETYPES void init_libpnmimagetypes();
  init_libpnmimagetypes();

  // Ensure the animation subsystem is available.
  extern EXPCL_PANDA_CHAR void init_libchar();
  init_libchar();

  // We also want the egg loader.
  #ifdef HAVE_EGG
  extern EXPCL_PANDAEGG void init_libpandaegg();
  init_libpandaegg();
  #endif

#endif

  reset_frame_rate();

  PT(GenericAsyncTask) event_task = new GenericAsyncTask("event", task_event, this);
  _task_mgr.add(event_task);

  _data_root = NodePath("data");
  
  PT(GenericAsyncTask) data_task = new GenericAsyncTask("data_loop", task_data_loop, this);
  data_task->set_sort(-50);
  _task_mgr.add(data_task);

  _event_handler.add_hook("window-event", event_window_event, this);
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::do_finalize
//       Access: Private
//  Description: Called by the destructor to close Panda.
////////////////////////////////////////////////////////////////////
void Panda::
do_finalize() {

  framework_cat.info()
    << "Closing Panda.\n";

  close_all_windows();
  // Also close down any other windows that might have been opened.
  if (_engine != (GraphicsEngine *)NULL) {
    _engine->remove_all_windows();
    _engine = NULL;
  }

  _event_handler.remove_all_hooks();
  _default_pipe.clear();
  Thread::prepare_for_exit();

}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_default_pipe
//       Access: Public
//  Description: Returns the default pipe.  This is the GraphicsPipe
//               that all windows in the framework will be created on,
//               unless otherwise specified in open_window().  It is
//               usually the primary graphics interface on the local
//               machine.
//
//               If the default pipe has not yet been created, this
//               creates it.
//
//               The return value is the default pipe, or NULL if no
//               default pipe could be created.
////////////////////////////////////////////////////////////////////
GraphicsPipe *Panda::
get_default_pipe() {
  if (!_made_default_pipe) {
    make_default_pipe();
    _made_default_pipe = true;
  }
  return _default_pipe;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_mouse
//       Access: Public
//  Description: Returns a NodePath to the MouseAndKeyboard associated
//               with the indicated GraphicsWindow object.  If there's
//               not yet a mouse associated with the window, creates
//               one.
//
//               This allows multiple Window objects that
//               represent different display regions of the same
//               GraphicsWindow to share the same mouse.
////////////////////////////////////////////////////////////////////
NodePath Panda::
get_mouse(GraphicsOutput *window) {
  Mouses::iterator mi = _mouses.find(window);
  if (mi != _mouses.end()) {
    return (*mi).second;
  }

  NodePath mouse;

  if (window->is_of_type(GraphicsWindow::get_class_type())) {
    NodePath data_root = get_data_root();
    GraphicsWindow *win = DCAST(GraphicsWindow, window);
    MouseAndKeyboard *mouse_node = new MouseAndKeyboard(win, 0, "mouse");
    mouse = data_root.attach_new_node(mouse_node);    
  }
    
  _mouses[window] = mouse;

  return mouse;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::remove_mouse
//       Access: Public
//  Description: Removes the mouse that may have been created by an
//               earlier call to get_mouse().
////////////////////////////////////////////////////////////////////
void Panda::
remove_mouse(const GraphicsOutput *window) {
  Mouses::iterator mi = _mouses.find(window);
  if (mi != _mouses.end()) {
    (*mi).second.remove_node();
    _mouses.erase(mi);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::register_event
//       Access: Public
//  Description: Sets up a handler for the indicated key.  When the
//               key is pressed in a window, the given callback will
//               be called.  The description is a one-line description
//               of the function of the key, for display to the user.
////////////////////////////////////////////////////////////////////
void Panda::
register_event(const string &event_name,
               EventHandler::EventCallbackFunction *function,
               void *data) {

  _event_handler.add_hook(event_name, function, data);
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::register_event
//       Access: Public
//  Description: Sets up a handler for the indicated key.  When the
//               key is pressed in a window, the given callback will
//               be called.  The description is a one-line description
//               of the function of the key, for display to the user.
////////////////////////////////////////////////////////////////////
void Panda::
unregister_event(const string &event_name) {
  _event_handler.remove_hooks(event_name);
}

//////////////////////////////////////////////////////////////////////
////     Function: Panda::get_default_window_props
////       Access: Public, Virtual
////  Description: Fills in the indicated window properties structure
////               according to the normal window properties for this
////               application.
//////////////////////////////////////////////////////////////////////
//void Panda::
//get_default_window_props(WindowProperties &props) {
//  // This function is largely vestigial and will be removed soon.  We
//  // have moved the default window properties into
//  // WindowProperties::get_default().
//
//  props.add_properties(WindowProperties::get_default());
//  if (!_window_title.empty()) {
//    props.set_title(_window_title);
//  }
//}

////////////////////////////////////////////////////////////////////
//     Function: Panda::open_window
//       Access: Public
//  Description: Opens a window on the default graphics pipe.  If the
//               default graphics pipe can't open a window for some
//               reason, automatically fails over to the next
//               available graphics pipe, and updates _default_pipe
//               accordingly.  Returns NULL only if all graphics pipes
//               fail.
////////////////////////////////////////////////////////////////////
Window *Panda::
open_window(const string &window_title) {
  GraphicsPipe *pipe = get_default_pipe();
  if (pipe == (GraphicsPipe *)NULL) {
    // Can't get a pipe.
    return NULL;
  }

  Window *wf = open_window(pipe, NULL);
  if (wf == (Window *)NULL) {
    // Ok, the default graphics pipe failed; try a little harder.
    GraphicsPipeSelection *selection = GraphicsPipeSelection::get_global_ptr();
    selection->load_aux_modules();

    int num_pipe_types = selection->get_num_pipe_types();
    for (int i = 0; i < num_pipe_types; i++) {
      TypeHandle pipe_type = selection->get_pipe_type(i);
      if (pipe_type != _default_pipe->get_type()) {
        PT(GraphicsPipe) new_pipe = selection->make_pipe(pipe_type);
        if (new_pipe != (GraphicsPipe *)NULL) {
          wf = open_window(new_pipe, NULL, window_title);
          if (wf != (Window *)NULL) {
            // Here's the winner!
            _default_pipe = new_pipe;
            return wf;
          }
        }
      }
    }

    // Too bad; none of the pipes could open a window.  Fall through
    // and return NULL.
  }

  return wf;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::open_window
//       Access: Public
//  Description: Opens a new window on the indicated pipe, using the
//               default parameters.  Returns the new Window
//               if successful, or NULL if not.
////////////////////////////////////////////////////////////////////
Window *Panda::
open_window(GraphicsPipe *pipe, GraphicsStateGuardian *gsg, const string &window_title) {

  WindowProperties props = WindowProperties::get_default();
  props.set_title(window_title);

  int flags = GraphicsPipe::BF_require_window;
  if (window_type == "offscreen") {
    flags = GraphicsPipe::BF_refuse_window;
  }
  
  return open_window(props, flags, pipe, gsg);
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::open_window
//       Access: Public
//  Description: Opens a new window using the indicated properties.
//               (You may initialize the properties to their default
//               values by calling get_default_window_props() first.)
//
//               Returns the new Window if successful, or
//               NULL if not.
////////////////////////////////////////////////////////////////////
Window *Panda::
open_window(const WindowProperties &props, int flags,
            GraphicsPipe *pipe, GraphicsStateGuardian *gsg) {
  if (pipe == (GraphicsPipe *)NULL) {
    pipe = get_default_pipe();
    if (pipe == (GraphicsPipe *)NULL) {
      // Can't get a pipe.
      return NULL;
    }
  }

  PT(Window) wf = make_window();

  GraphicsOutput *win = wf->open_window(props, flags, get_graphics_engine(), 
                                        pipe, gsg);
  _engine->open_windows();
  if (win != (GraphicsOutput *)NULL && !win->is_valid()) {
    // The window won't open.
    _engine->remove_window(win);
    wf->close_window();
    win = NULL;
  }

  if (win == (GraphicsOutput *)NULL) {
    // Oops, couldn't make a window or buffer.
    framework_cat.error()
      << "Unable to create window.\n";
    return NULL;
  }

  _windows.push_back(wf);
  return wf;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::find_window
//       Access: Public
//  Description: Returns the index of the first Window object
//               found that references the indicated GraphicsOutput
//               pointer, or -1 if none do.
////////////////////////////////////////////////////////////////////
int Panda::
find_window(const GraphicsOutput *win) const {
  int n;
  for (n = 0; n < (int)_windows.size(); n++) {
    if (_windows[n]->get_graphics_output() == win) {
      return n;
    }
  }

  return -1;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::find_window
//       Access: Public
//  Description: Returns the index of the given Window
//               object, or -1 if the object does not represent a
//               window opened with this Panda.
////////////////////////////////////////////////////////////////////
int Panda::
find_window(const Window *wf) const {
  int n;
  for (n = 0; n < (int)_windows.size(); n++) {
    if (_windows[n] == wf) {
      return n;
    }
  }

  return -1;
}


////////////////////////////////////////////////////////////////////
//     Function: Panda::close_window
//       Access: Public
//  Description: Closes the nth window and removes it from the list.
////////////////////////////////////////////////////////////////////
void Panda::
close_window(int n) {
  nassertv(n >= 0 && n < (int)_windows.size());
  Window *wf = _windows[n];

  GraphicsOutput *win = wf->get_graphics_output();
  if (win != (GraphicsOutput *)NULL) {
    _engine->remove_window(win);
  }
  
  wf->close_window();
  _windows.erase(_windows.begin() + n);
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::close_all_windows
//       Access: Public
//  Description: Closes all currently open windows and empties the
//               list of windows.
////////////////////////////////////////////////////////////////////
void Panda::
close_all_windows() {
  Windows::iterator wi;
  for (wi = _windows.begin(); wi != _windows.end(); ++wi) {
    Window *wf = (*wi);

    GraphicsOutput *win = wf->get_graphics_output();
    if (win != (GraphicsOutput *)NULL) {
      _engine->remove_window(win);
    }
    
    wf->close_window();
  }

  Mouses::iterator mi;
  for (mi = _mouses.begin(); mi != _mouses.end(); ++mi) {
    (*mi).second.remove_node();
  }

  _windows.clear();
  _mouses.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_num_open_windows
//       Access: Public
//  Description: Returns the number of windows that haven't been
//               closed by the user.
////////////////////////////////////////////////////////////////////
int Panda::
get_num_open_windows() const {
  Windows::const_iterator wi;
  int count = 0;
  for (wi = _windows.begin(); wi != _windows.end(); ++wi) {
    Window *wf = (*wi);
    if (wf->get_graphics_output()->is_valid()) {
      count++;
    }
  }

  return count;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_frame_rate
//       Access: Public
//  Description: Returns the currently measured average frame rate.
//               If not enough frames have passed we return the last
//               successful measurement for convenience.
////////////////////////////////////////////////////////////////////
float Panda::
get_frame_rate() {
  double now = ClockObject::get_global_clock()->get_frame_time();
  double delta = now - _start_time;
  
  int frame_count = ClockObject::get_global_clock()->get_frame_count();
  int num_frames = frame_count - _frame_count;
  if (num_frames > 0) {
    _fps = ((double)num_frames) / delta;
  }
  return _fps;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::reset_frame_rate
//       Access: Public
//  Description: Resets the frame rate computation.
////////////////////////////////////////////////////////////////////
void Panda::
reset_frame_rate() {
  _start_time = ClockObject::get_global_clock()->get_frame_time();
  _frame_count = ClockObject::get_global_clock()->get_frame_count();
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::do_frame
//       Access: Public, Virtual
//  Description: Renders one frame and performs all associated
//               processing.  Returns true if we should continue
//               rendering, false if we should exit.  This is normally
//               called only from main_loop().
////////////////////////////////////////////////////////////////////
bool Panda::
do_frame(Thread *current_thread) {

  _task_mgr.poll();

  return !_exit_flag;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::main_loop
//       Access: Public
//  Description: Called to yield control to the panda framework.  This
//               function does not return until set_exit_flag() has
//               been called.
////////////////////////////////////////////////////////////////////
void Panda::
run() {
  Thread *current_thread = Thread::get_current_thread();
  while (do_frame(current_thread)) {
    //Thread::sleep(0.001);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::make_window
//       Access: Protected
//  Description: Creates a new Window object.
////////////////////////////////////////////////////////////////////
PT(Window) Panda::
make_window() {
  return new Window(this);
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::make_default_pipe
//       Access: Protected, Virtual
//  Description: Creates the default GraphicsPipe that will contain
//               all windows that are not opened on a specific pipe.
////////////////////////////////////////////////////////////////////
void Panda::
make_default_pipe() {
  // This depends on the shared library or libraries (DLL's to you
  // Windows folks) that have been loaded in at runtime from the
  // load-display and/or aux-display Configrc variables.
  GraphicsPipeSelection *selection = GraphicsPipeSelection::get_global_ptr();
  selection->print_pipe_types();
  _default_pipe = selection->make_default_pipe();

  if (_default_pipe == (GraphicsPipe*)NULL) {
    nout << "No graphics pipe is available!\n"
         << "Your Config.prc file must name at least one valid panda display\n"
         << "library via load-display or aux-display.\n";
  }
}



////////////////////////////////////////////////////////////////////
//     Function: Panda::event_window_event
//       Access: Public, Static
//  Description: Default handler for window events: window resized or
//               closed, etc.
////////////////////////////////////////////////////////////////////
void Panda::
event_window_event(const Event *event, void *data) {
  Panda *self = (Panda *)data;
  if (event->get_num_parameters() == 1) {
    // The parameter of the window event is the window itself, rather
    // than the window framework object (which is the parameter of all
    // of the keyboard events).
    EventParameter param = event->get_parameter(0);
    const GraphicsOutput *win;
    DCAST_INTO_V(win, param.get_ptr());

    // Is this a window we've heard about?
    int window_index = self->find_window(win);
    if (window_index == -1) {
      framework_cat.debug()
        << "Ignoring message from unknown window.\n";

    } else {
      if (!win->is_valid()) {
        // The window has been closed.
        int window_index = self->find_window(win);
        while (window_index != -1) {
          self->close_window(window_index);
          window_index = self->find_window(win);
        }
        
        // Free up the mouse for that window.
        self->remove_mouse(win);

        // If the last window was closed, exit the application.
        if (self->get_num_open_windows() == 0 && !self->_exit_flag) {
          framework_cat.info()
            << "Last window was closed by user.\n";
          self->_exit_flag = true;
        }
      } else {
        // Adjust aspect ratio.
        for (int n = 0; n < (int)self->_windows.size(); n++) {
          if (self->_windows[n]->get_graphics_output() == win) {
            return self->_windows[n]->adjust_aspect_ratio();
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::task_data_loop
//       Access: Public, Static
//  Description: Called once per frame to process the data graph (which
//               handles user input via the mouse and keyboard, etc.)
////////////////////////////////////////////////////////////////////
AsyncTask::DoneStatus Panda::
task_data_loop(GenericAsyncTask *task, void *data) {
  Panda *self = (Panda *)data;

  DataGraphTraverser dg_trav;
  dg_trav.traverse(self->_data_root.node());

  return AsyncTask::DS_cont;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::task_event
//       Access: Public, Static
//  Description: Called once per frame to process the pending events.
////////////////////////////////////////////////////////////////////
AsyncTask::DoneStatus Panda::
task_event(GenericAsyncTask *task, void *data) {
  Panda *self = (Panda *)data;

  throw_event("NewFrame");
  self->_event_handler.process_events();

  return AsyncTask::DS_cont;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::task_igloop
//       Access: Public, Static
//  Description: Called once per frame to render the scene.
////////////////////////////////////////////////////////////////////
AsyncTask::DoneStatus Panda::
task_igloop(GenericAsyncTask *task, void *data) {
  Panda *self = (Panda *)data;

  if (self->_engine != (GraphicsEngine *)NULL) {
    self->_engine->render_frame();
  }

  return AsyncTask::DS_cont;
}

