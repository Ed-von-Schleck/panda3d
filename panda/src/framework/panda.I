// Filename: pandaFramework.I
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


////////////////////////////////////////////////////////////////////
//     Function: Panda::get_graphics_engine
//       Access: Public
//  Description: Returns the GraphicsEngine that is used to render all
//               the windows in the framework.  Normally there's no
//               reason for user code to mess with this.
////////////////////////////////////////////////////////////////////
INLINE GraphicsEngine *Panda::
get_graphics_engine() {
  if (_engine == (GraphicsEngine *)NULL) {
    _engine = GraphicsEngine::get_global_ptr();
    PT(GenericAsyncTask) task = new GenericAsyncTask("igloop", task_igloop, this);
    task->set_sort(50);
    _task_mgr.add(task);
  }
  return _engine;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_data_root
//       Access: Public
//  Description: Returns the root of the data graph.  This is the
//               graph of nodes that is traversed to control the
//               inputs from user devices like the mouse and keyboard.
////////////////////////////////////////////////////////////////////
INLINE const NodePath &Panda::
get_data_root() const {
  return _data_root;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_event_handler
//       Access: Public
//  Description: Returns the EventHandler object that serves events in
//               the framework.  This is primarily used to dispatch on
//               keypresses and such.
////////////////////////////////////////////////////////////////////
INLINE EventHandler &Panda::
get_event_handler() {
  return _event_handler;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_task_manager
//       Access: Public
//  Description: Returns the Task Manager object that manages tasks in
//               the framework.
////////////////////////////////////////////////////////////////////
INLINE AsyncTaskManager &Panda::
get_task_manager() {
  return _task_mgr;
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_num_windows
//       Access: Public
//  Description: Returns the number of windows that are currently
//               open.
////////////////////////////////////////////////////////////////////
INLINE int Panda::
get_num_windows() const {
  return _windows.size();
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::get_window
//       Access: Public
//  Description: Returns the nth window currently open.
////////////////////////////////////////////////////////////////////
INLINE Window *Panda::
get_window(int n) const {
  nassertr(n >= 0 && n < (int)_windows.size(), NULL);
  return _windows[n];
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::close_window
//       Access: Public
//  Description: Closes the indicated Window window and
//               removes it from the list.
////////////////////////////////////////////////////////////////////
INLINE void Panda::
close_window(Window *wf) {
  int n = find_window(wf);
  if (n >= 0) {
    close_window(n);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Panda::set_exit_flag
//       Access: Public
//  Description: Sets the flag that indicates it is time for the
//               application to exit.  The application will actually
//               exit at the end of the current frame.
////////////////////////////////////////////////////////////////////
INLINE void Panda::
set_exit_flag(bool exit_flag) {
  _exit_flag = exit_flag;
}

