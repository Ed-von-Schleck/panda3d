// Filename: pandaFramework.h
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

#ifndef PANDA_H
#define PANDA_H

#include "pandabase.h"

#include "window.h"

#include "nodePath.h"
#include "eventHandler.h"
#include "graphicsPipe.h"
#include "graphicsEngine.h"
#include "graphicsWindow.h"
#include "pointerTo.h"
#include "asyncTaskManager.h"
#include "genericAsyncTask.h"

#include "pvector.h"

////////////////////////////////////////////////////////////////////
//       Class : Panda
// Description : This class serves to provide a high-level framework
//               for most Panda usage patterns.
//               It starts the system tasks, holds the graphic
//               outputs in use and registers events.
////////////////////////////////////////////////////////////////////
class EXPCL_FRAMEWORK Panda {
public:

  Panda();
  virtual ~Panda();

  void init();
  void close();

  GraphicsPipe *get_default_pipe();
  INLINE GraphicsEngine *get_graphics_engine();
  INLINE const NodePath &get_data_root() const;
  INLINE EventHandler &get_event_handler();
  INLINE AsyncTaskManager &get_task_manager();

  NodePath get_mouse(GraphicsOutput *window);
  void remove_mouse(const GraphicsOutput *window);

  void register_event(const string &event_name, 
                  EventHandler::EventCallbackFunction *function,
                  void *data = (void*)NULL);
  void unregister_event(const string &event_name);

  Window *open_window(const string &window_title = "Panda3D Application");
  Window *open_window(GraphicsPipe *pipe,
                      GraphicsStateGuardian *gsg = NULL,
                      const string &window_title = "Panda3D Application");
  Window *open_window(const WindowProperties &props,
                      int flags,
                      GraphicsPipe *pipe = NULL,
                      GraphicsStateGuardian *gsg = NULL);

  INLINE int get_num_windows() const;
  INLINE Window *get_window(int n) const;
  int find_window(const GraphicsOutput *win) const;
  int find_window(const Window *w) const;
  void close_window(int n);
  INLINE void close_window(Window *w);
  void close_all_windows();
  int get_num_open_windows() const;

  float get_frame_rate();
  void reset_frame_rate();

  virtual bool do_frame(Thread *current_thread);
  void run();

  INLINE void set_exit_flag(bool exit_flag);

public:
  static LoaderOptions _loader_options;

protected:
  PT(Window) make_window();
  void make_default_pipe();

public:

  static void event_window_event(const Event *, void *data);

  static AsyncTask::DoneStatus task_data_loop(GenericAsyncTask *task, void *data);
  static AsyncTask::DoneStatus task_event(GenericAsyncTask *task, void *data);
  static AsyncTask::DoneStatus task_igloop(GenericAsyncTask *task, void *data);
  static AsyncTask::DoneStatus task_play_frame(GenericAsyncTask *task, void *data);

private:


  bool _is_open;

  bool _made_default_pipe;

  PT(GraphicsPipe) _default_pipe;
  PT(GraphicsEngine) _engine;

  double _fps;

  NodePath _data_root;
  EventHandler &_event_handler;
  AsyncTaskManager &_task_mgr;

  typedef pvector< PT(Window) > Windows;
  Windows _windows;

  typedef pmap< const GraphicsOutput *, NodePath > Mouses;
  Mouses _mouses;

  // For counting frame rate.
  double _start_time;
  int _frame_count;

  bool _exit_flag;
  
  friend class Window;
};

#include "panda.I"

#endif
