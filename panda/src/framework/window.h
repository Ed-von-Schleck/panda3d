// Filename: window.h
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

#ifndef WINDOW_H
#define WINDOW_H

#include "pandabase.h"
#include "nodePath.h"
#include "camera.h"
#include "graphicsOutput.h"
#include "graphicsWindow.h"
#include "animControlCollection.h"
#include "trackball.h"
#include "filename.h"
#include "frameRateMeter.h"
#include "sceneGraphAnalyzerMeter.h"
#include "pointerTo.h"
#include "partGroup.h"
#include "pvector.h"
#include "typedWritableReferenceCount.h"
#include "loaderOptions.h"
#include "pgSliderBar.h"
#include "textNode.h"
#include "eventHandler.h"
#include "genericAsyncTask.h"

class Panda;
class AmbientLight;
class DirectionalLight;
class GraphicsEngine;
class GraphicsPipe;
class DisplayRegion;

////////////////////////////////////////////////////////////////////
//       Class : Window
// Description : This encapsulates the data that is normally
//               associated with a single window, or with a single
//               display region within a window.  (In the case where a
//               window has been subdivided with split_window(), there
//               may be multiple Windows objects that share
//               the same GraphicsOutput pointer, but reference
//               different display regions within that window).
////////////////////////////////////////////////////////////////////
class EXPCL_FRAMEWORK Window : public TypedWritableReferenceCount {
protected:
  Window(Panda *panda_framework);
  Window(const Window &copy, DisplayRegion *display_region);

public:
  virtual ~Window();

protected:
  GraphicsOutput *open_window(const WindowProperties &props, int flags,
                              GraphicsEngine *engine, GraphicsPipe *pipe,
                              GraphicsStateGuardian *gsg = NULL);
  void close_window();

public:
  INLINE Panda *get_panda_framework() const;
  INLINE GraphicsWindow *get_graphics_window() const;
  INLINE GraphicsOutput *get_graphics_output() const;
  NodePath get_camera_group();

  INLINE int get_num_cameras() const;
  INLINE Camera *get_camera(int n) const;

  INLINE DisplayRegion *get_display_region_2d() const;
  INLINE DisplayRegion *get_display_region_3d() const;

  NodePath get_render();
  NodePath get_render_2d();
  NodePath get_aspect_2d();
  NodePath get_mouse();
  NodePath get_button_thrower();

  void enable_keyboard();
  void setup_trackball();
  void center_trackball(const NodePath &object);

  NodePath load_model(Filename filename);
  NodePath load_model(const NodePath &parent, Filename filename);
  PT(PandaNode) load_image_as_model(const Filename &filename);

  void adjust_aspect_ratio();

  enum BackgroundType {
    BT_other = 0,
    BT_default,
    BT_black,
    BT_gray,
    BT_white,
    BT_none
  };

  enum SplitType {
    ST_default,
    ST_horizontal,
    ST_vertical,
  };
  Window *split_window(SplitType split_type = ST_default);

  void set_background_type(BackgroundType type);

  INLINE BackgroundType get_background_type() const;

  static TextFont *get_shuttle_controls_font();
  NodePath make_camera();

protected:
  void setup_lights();

private:
  Panda *_panda_framework;
  PT(GraphicsOutput) _window;
  PT(DisplayRegion) _display_region_2d;
  PT(DisplayRegion) _display_region_3d;

  NodePath _camera_group;
  typedef pvector< PT(Camera) > Cameras;
  Cameras _cameras;

  NodePath _render;
  NodePath _render_2d;
  NodePath _aspect_2d;

  AnimControlCollection _anim_controls;
  bool _anim_controls_enabled;
  int _anim_index;
  NodePath _anim_controls_group;
  PT(PGSliderBar) _anim_slider;
  PT(PGSliderBar) _play_rate_slider;
  PT(TextNode) _frame_number;
  PT(GenericAsyncTask) _update_anim_controls_task;

  NodePath _mouse;
  NodePath _button_thrower;
  PT(Trackball) _trackball;

  NodePath _alight;
  NodePath _dlight;
  
  bool _got_keyboard;
  bool _got_trackball;
  bool _got_lights;

  bool _wireframe_enabled;
  bool _texture_enabled;
  bool _two_sided_enabled;
  bool _one_sided_reverse_enabled;
  bool _lighting_enabled;
  bool _perpixel_enabled;

  PT(FrameRateMeter) _frame_rate_meter;
  PT(SceneGraphAnalyzerMeter) _scene_graph_analyzer_meter;

  BackgroundType _background_type;
  
  static PT(TextFont) _shuttle_controls_font;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedWritableReferenceCount::init_type();
    register_type(_type_handle, "Window",
                  TypedWritableReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

  friend class Panda;
};

#include "window.I"

#endif
