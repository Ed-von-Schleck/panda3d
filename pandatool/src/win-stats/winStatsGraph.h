// Filename: winStatsGraph.h
// Created by:  drose (03Dec03)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#ifndef WINSTATSGRAPH_H
#define WINSTATSGRAPH_H

#include "pandatoolbase.h"
#include "winStatsLabelStack.h"
#include "pmap.h"

#include <windows.h>

class WinStatsMonitor;

////////////////////////////////////////////////////////////////////
//       Class : WinStatsGraph
// Description : This is just an abstract base class to provide a
//               common pointer type for the various kinds of graphs
//               that may be created for a WinStatsMonitor.
////////////////////////////////////////////////////////////////////
class WinStatsGraph {
public:
  WinStatsGraph(WinStatsMonitor *monitor);
  virtual ~WinStatsGraph();

  virtual void new_collector(int collector_index);
  virtual void new_data(int thread_index, int frame_number);
  virtual void force_redraw();
  virtual void changed_graph_size(int graph_xsize, int graph_ysize);

protected:
  void close();

  void setup_label_stack();
  void move_label_stack();

  HBRUSH get_collector_brush(int collector_index);

  LONG WINAPI window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
  // Table of brushes for our various collectors.
  typedef pmap<int, HBRUSH> Brushes;
  Brushes _brushes;

  WinStatsMonitor *_monitor;
  HWND _window;
  HWND _graph_window;
  WinStatsLabelStack _label_stack;

  HBITMAP _bitmap;
  HDC _bitmap_dc;

  int _bitmap_xsize, _bitmap_ysize;
  int _left_margin, _right_margin;
  int _top_margin, _bottom_margin;

private:
  void setup_bitmap(int xsize, int ysize);
  void release_bitmap();
  void move_graph_window(int graph_left, int graph_top,
                         int graph_xsize, int graph_ysize);
  void create_graph_window();
  static void register_graph_window_class(HINSTANCE application);

  static LONG WINAPI static_graph_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  LONG WINAPI graph_window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

  static bool _graph_window_class_registered;
  static const char * const _graph_window_class_name;
};

#endif

