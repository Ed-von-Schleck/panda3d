// Filename: window.cxx
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

#include "window.h"
#include "panda.h"
#include "displayRegion.h"
#include "buttonThrower.h"
#include "transform2sg.h"
#include "dSearchPath.h"
#include "filename.h"
#include "loader.h"
#include "keyboardButton.h"
#include "geom.h"
#include "geomTriangles.h"
#include "geomTristrips.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geomVertexWriter.h"
#include "texturePool.h"
#include "textureAttrib.h"
#include "colorAttrib.h"
#include "perspectiveLens.h"
#include "orthographicLens.h"
#include "auto_bind.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "lightAttrib.h"
#include "boundingSphere.h"
#include "deg_2_rad.h"
#include "config_framework.h"
#include "cullFaceAttrib.h"
#include "rescaleNormalAttrib.h"
#include "shadeModelAttrib.h"
#include "pgTop.h"
#include "geomNode.h"
#include "texture.h"
#include "videoTexture.h"
#include "movieTexture.h"
#include "texturePool.h"
#include "loaderFileTypeRegistry.h"
#include "pnmImage.h"
#include "virtualFileSystem.h"
#include "string_utils.h"
#include "bamFile.h"
#include "staticTextFont.h"
#include "mouseButton.h"

TypeHandle Window::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: Window::Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
Window::
Window(Panda *panda_framework) :
  _panda_framework(panda_framework)
{
  _alight = (AmbientLight *)NULL;
  _dlight = (DirectionalLight *)NULL;
  _got_keyboard = false;
  _got_trackball = false;
  _got_lights = false;
  _anim_controls_enabled = false;
  _anim_index = 0;
  _wireframe_enabled = false;
  _texture_enabled = true;
  _two_sided_enabled = false;
  _one_sided_reverse_enabled = false;
  _lighting_enabled = false;
  _perpixel_enabled = false;
  _background_type = BT_default;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::Copy Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
Window::
Window(const Window &copy, DisplayRegion *display_region) :
  _panda_framework(copy._panda_framework),
  _window(copy._window),
  _display_region_3d(display_region)
{
  _alight = (AmbientLight *)NULL;
  _dlight = (DirectionalLight *)NULL;
  _got_keyboard = false;
  _got_trackball = false;
  _got_lights = false;
  _anim_controls_enabled = false;
  _anim_index = 0;
  _wireframe_enabled = false;
  _texture_enabled = true;
  _two_sided_enabled = false;
  _one_sided_reverse_enabled = false;
  _lighting_enabled = false;
  _perpixel_enabled = false;
  _background_type = BT_default;

  set_background_type(copy._background_type);
  // Set up a 3-d camera for the window by default.
  NodePath camera_np = make_camera();
  _display_region_3d->set_camera(camera_np);
}

////////////////////////////////////////////////////////////////////
//     Function: Window::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
Window::
~Window() {
  close_window();
}

////////////////////////////////////////////////////////////////////
//     Function: Window::open_window
//       Access: Protected
//  Description: Opens the actual window or buffer.  This is normally
//               called only from Panda::open_window().
////////////////////////////////////////////////////////////////////
GraphicsOutput *Window::
open_window(const WindowProperties &props, int flags, GraphicsEngine *engine,
            GraphicsPipe *pipe, GraphicsStateGuardian *gsg) {
  nassertr(_window == (GraphicsOutput *)NULL, _window);

  static int next_window_index = 1;
  ostringstream stream;
  stream << "window" << next_window_index;
  next_window_index++;
  string name = stream.str();

  _window = 0;
  GraphicsOutput *winout = 
    engine->make_output(pipe, name, 0,
                        FrameBufferProperties::get_default(),
                        props, flags, gsg, NULL);
  if (winout != (GraphicsOutput *)NULL) {
    _window = winout;
    //    _window->request_properties(props);

    // Create a display region that covers the entire window.
    _display_region_3d = _window->make_display_region();

    // Make sure the DisplayRegion does the clearing, not the window,
    // so we can have multiple DisplayRegions of different colors.
    _window->set_clear_color_active(false);
    _window->set_clear_depth_active(false);
    _window->set_clear_stencil_active(false);

    // Set up a 3-d camera for the window by default.
    NodePath camera_np = make_camera();
    _display_region_3d->set_camera(camera_np);

    set_background_type(_background_type);

    if (show_frame_rate_meter) {
      _frame_rate_meter = new FrameRateMeter("frame_rate_meter");
      _frame_rate_meter->setup_window(_window);
    }
    if (show_scene_graph_analyzer_meter) {
      _scene_graph_analyzer_meter = new SceneGraphAnalyzerMeter("scene_graph_analyzer_meter", get_render().node());
      _scene_graph_analyzer_meter->setup_window(_window);
    }
  }

  enable_input();

  return _window;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::close_window
//       Access: Protected
//  Description: Closes the window or buffer.  This is normally called
//               from Panda::close_window().
////////////////////////////////////////////////////////////////////
void Window::
close_window() {
  _window.clear();
  _camera_group.remove_node();
  _render.remove_node();
  _render_2d.remove_node();
  _mouse.remove_node();

  _alight = (AmbientLight *)NULL;
  _dlight = (DirectionalLight *)NULL;
  _got_keyboard = false;
  _got_trackball = false;
  _got_lights = false;

  _wireframe_enabled = false;
  _texture_enabled = true;
  _two_sided_enabled = false;
  _one_sided_reverse_enabled = false;
  _lighting_enabled = false;
  _perpixel_enabled = false;

  if (_frame_rate_meter != (FrameRateMeter *)NULL) {
    _frame_rate_meter->clear_window();
    _frame_rate_meter = (FrameRateMeter *)NULL;
  }
  if (_scene_graph_analyzer_meter != (SceneGraphAnalyzerMeter *)NULL) {
    _scene_graph_analyzer_meter->clear_window();
    _scene_graph_analyzer_meter = (SceneGraphAnalyzerMeter *)NULL;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_camera_group
//       Access: Public
//  Description: Returns the node above the collection of 3-d cameras
//               in the scene graph.  This node may be moved around to
//               represent the viewpoint.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_camera_group() {
  if (_camera_group.is_empty()) {
    _camera_group = get_render().attach_new_node("camera_group");
  }
  return _camera_group;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_render
//       Access: Public
//  Description: Returns the root of the 3-d scene graph.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_render() {
  if (_render.is_empty()) {
    _render = NodePath("render");

    _render.node()->set_attrib(RescaleNormalAttrib::make_default());
    _render.node()->set_attrib(ShadeModelAttrib::make(ShadeModelAttrib::M_smooth));

    // This is maybe here temporarily, and maybe not.
    _render.set_two_sided(0);
  }
  return _render;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_render_2d
//       Access: Public
//  Description: Returns the root of the 2-d scene graph.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_render_2d() {
  if (_render_2d.is_empty()) {
    _render_2d = NodePath("render_2d");

    // Some standard properties for the 2-d display.

    _render_2d.set_depth_write(0);
    _render_2d.set_depth_test(0);
    _render_2d.set_material_off(1);
    _render_2d.set_two_sided(1);

    // Now set up a 2-d camera to view render_2d.

    // Create a display region that matches the size of the 3-d
    // display region.
    float l, r, b, t;
    _display_region_3d->get_dimensions(l, r, b, t);
    _display_region_2d = _window->make_mono_display_region(l, r, b, t);
    _display_region_2d->set_sort(10);

    // Finally, we need a camera to associate with the display region.
    PT(Camera) camera = new Camera("camera2d");
    NodePath camera_np = _render_2d.attach_new_node(camera);

    PT(Lens) lens = new OrthographicLens;

    static const float left = -1.0f;
    static const float right = 1.0f;
    static const float bottom = -1.0f;
    static const float top = 1.0f;
    lens->set_film_size(right - left, top - bottom);
    lens->set_film_offset((right + left) * 0.5, (top + bottom) * 0.5);
    lens->set_near_far(-1000, 1000);

    camera->set_lens(lens);
    _display_region_2d->set_camera(camera_np);
  }

  return _render_2d;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_aspect_2d
//       Access: Public
//  Description: Returns the node under the 2-d scene graph that is
//               scaled to suit the window's aspect ratio.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_aspect_2d() {
  if (_aspect_2d.is_empty()) {
    PGTop *top = new PGTop("aspect_2d");
    _aspect_2d = get_render_2d().attach_new_node(top);

    // Tell the PGTop about our MouseWatcher object, so the PGui
    // system can operate.
    PandaNode *mouse_node = get_mouse().node();
    if (mouse_node->is_of_type(MouseWatcher::get_class_type())) {
      top->set_mouse_watcher(DCAST(MouseWatcher, mouse_node));
    }

    float this_aspect_ratio = aspect_ratio;
    if (this_aspect_ratio == 0.0f) {
      // An aspect ratio of 0.0 means to try to infer it.
      this_aspect_ratio = 1.0f;

      if (_window->has_size()) {
        int x_size = _window->get_x_size();
        int y_size = _window->get_y_size();
        if (y_size != 0) {
          this_aspect_ratio = (float)x_size / (float)y_size;
        }
      }
    }

    _aspect_2d.set_scale(1.0f / this_aspect_ratio, 1.0f, 1.0f);
  }

  return _aspect_2d;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_mouse
//       Access: Public
//  Description: Returns the node in the data graph corresponding to
//               the mouse associated with this window.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_mouse() {
  if (_mouse.is_empty()) {
    NodePath mouse = _panda_framework->get_mouse(_window);

    // Create a MouseWatcher to filter the mouse input.  We do this
    // mainly so we can constrain the mouse input to our particular
    // display region, if we have one.  This means the node we return
    // from get_mouse() is actually a MouseWatcher, but since it
    // presents the same interface as a Mouse, no one should mind.

    // Another advantage to using a MouseWatcher is that the PGTop of
    // aspect2d likes it better.
    PT(MouseWatcher) mw = new MouseWatcher("watcher");
    mw->set_display_region(_display_region_3d);
    _mouse = mouse.attach_new_node(mw);
  }
  return _mouse;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_button_thrower
//       Access: Public
//  Description: Returns the node in the data graph corresponding to
//               the ButtonThrower object associated with this window.
////////////////////////////////////////////////////////////////////
NodePath Window::
get_button_thrower() {
  return _button_thrower;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::enable_input
//       Access: Public
//  Description: Creates a ButtonThrower to listen to button presses
//               and throw them as events.
////////////////////////////////////////////////////////////////////
void Window::
enable_input() {
  if (_got_keyboard) {
    return;
  }

  if (_window->is_of_type(GraphicsWindow::get_class_type()) &&
      DCAST(GraphicsWindow, _window)->get_num_input_devices() > 0) {
    NodePath mouse = get_mouse();

    // Create a button thrower to listen for our keyboard events and
    // associate this Window pointer with each one.
    PT(ButtonThrower) bt = new ButtonThrower("kb-events");
    bt->add_parameter(EventParameter(this));
    ModifierButtons mods;
    mods.add_button(KeyboardButton::shift());
    mods.add_button(KeyboardButton::control());
    mods.add_button(KeyboardButton::alt());
    mods.add_button(KeyboardButton::meta());
    bt->set_modifier_buttons(mods);
    _button_thrower = mouse.attach_new_node(bt);
  }

  _got_keyboard = true;
}


////////////////////////////////////////////////////////////////////
//     Function: Window::disable_input
//       Access: Public
//  Description: Removes the ButtonThrower that listens to button
//               presses them as events, if present.
////////////////////////////////////////////////////////////////////
void Window::
disable_input() {
  if (!_got_keyboard) {
    return;
  }

  if (!_button_thrower.is_empty()){
    _button_thrower.remove_node();
  }

  _got_keyboard = false;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::setup_trackball
//       Access: Public
//  Description: Sets up the mouse to trackball around the camera.
////////////////////////////////////////////////////////////////////
void Window::
setup_trackball() {
  if (_got_trackball) {
    return;
  }

  if (_window->is_of_type(GraphicsWindow::get_class_type()) &&
      DCAST(GraphicsWindow, _window)->get_num_input_devices() > 0) {
    NodePath mouse = get_mouse();
    NodePath camera = get_camera_group();

    _trackball = new Trackball("trackball");
    _trackball->set_pos(LVector3f::forward() * 50.0);
    mouse.attach_new_node(_trackball);

    PT(Transform2SG) tball2cam = new Transform2SG("tball2cam");
    tball2cam->set_node(camera.node());
    _trackball->add_child(tball2cam);
  }

  _got_trackball = true;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::center_trackball
//       Access: Public
//  Description: Centers the trackball on the indicated object, and
//               scales the trackball motion suitably.
////////////////////////////////////////////////////////////////////
void Window::
center_trackball(const NodePath &object) {
  if (_trackball == (Trackball *)NULL) {
    return;
  }

  PT(BoundingVolume) volume = object.get_bounds();
  // We expect at least a geometric bounding volume around the world.
  nassertv(volume != (BoundingVolume *)NULL);
  nassertv(volume->is_of_type(GeometricBoundingVolume::get_class_type()));
  CPT(GeometricBoundingVolume) gbv = DCAST(GeometricBoundingVolume, volume);

  if (object.has_parent()) {
    CPT(TransformState) net_transform = object.get_parent().get_net_transform();
    PT(GeometricBoundingVolume) new_gbv = DCAST(GeometricBoundingVolume, gbv->make_copy());
    new_gbv->xform(net_transform->get_mat());
    gbv = new_gbv;
  }

  // Determine the bounding sphere around the object.
  if (gbv->is_infinite()) {
    framework_cat.warning()
      << "Infinite bounding volume for " << object << "\n";
    return;
  }

  if (gbv->is_empty()) {
    framework_cat.warning()
      << "Empty bounding volume for " << object << "\n";
    return;
  }

  // The BoundingVolume might be a sphere (it's likely), but since it
  // might not, we'll take no chances and make our own sphere.
  PT(BoundingSphere) sphere = new BoundingSphere(gbv->get_approx_center(), 0.0f);
  if (!sphere->extend_by(gbv)) {
    framework_cat.warning()
      << "Cannot determine bounding volume of " << object << "\n";
    return;
  }

  LPoint3f center = sphere->get_center();
  float radius = sphere->get_radius();

  float distance = 50.0f;

  // Choose a suitable distance to view the whole volume in our frame.
  // This is based on the camera lens in use.  Determine the lens
  // based on the first camera; this will be the default camera.
  Lens *lens = (Lens *)NULL;
  if (!_cameras.empty()) {
    Cameras::const_iterator ci;
    for (ci = _cameras.begin();
         ci != _cameras.end() && lens == (Lens *)NULL;
         ++ci) {
      lens = (*ci)->get_lens();
    }
  }

  if (lens != (Lens *)NULL) {
    LVecBase2f fov = lens->get_fov();
    distance = radius / ctan(deg_2_rad(min(fov[0], fov[1]) / 2.0f));

    // Ensure the far plane is far enough back to see the entire object.
    float ideal_far_plane = distance + radius * 1.5;
    lens->set_far(max(lens->get_default_far(), ideal_far_plane));

    // And that the near plane is far enough forward.
    float ideal_near_plane = distance - radius;
    lens->set_near(min(lens->get_default_near(), ideal_near_plane));
  }

  _trackball->set_origin(center);
  _trackball->set_pos(LVector3f::forward() * distance);

  // Also set the movement scale on the trackball to be consistent
  // with the size of the model and the lens field-of-view.
  _trackball->set_forward_scale(distance * 0.006);
}

////////////////////////////////////////////////////////////////////
//     Function: Window::load_model
//       Access: Public
//  Description: Loads up the indicated model and returns the new
//               NodePath, parented to the specified parent, or the
//               empty NodePath if the model could not be loaded.
////////////////////////////////////////////////////////////////////
NodePath Window::
load_model(const NodePath &parent, Filename filename) {
  return parent.attach_new_node(load_model(filename).node());
}

////////////////////////////////////////////////////////////////////
//     Function: Window::load_model
//       Access: Public
//  Description: Loads up the indicated model and returns the new
//               NodePath, or the empty NodePath if the model could
//               not be loaded.
////////////////////////////////////////////////////////////////////
NodePath Window::
load_model(Filename filename) {
  nout << "Loading " << filename << "\n";

  // If the filename already exists where it is, or if it is fully
  // qualified, don't search along the model path for it.
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  bool search = !(filename.is_fully_qualified() || vfs->exists(filename));

  // We allow loading image files here.  Check to see if it might be
  // an image file, based on the filename extension.
  bool is_image = false;
  string extension = filename.get_extension();
#ifdef HAVE_ZLIB
  if (extension == "pz") {
    extension = Filename(filename.get_basename_wo_extension()).get_extension();
  }
#endif  // HAVE_ZLIB
  if (!extension.empty()) {
    LoaderFileTypeRegistry *reg = LoaderFileTypeRegistry::get_global_ptr();
    LoaderFileType *model_type =
      reg->get_type_from_extension(extension);
    if (model_type == (LoaderFileType *)NULL) {
      // The extension isn't a known model file type; is it a known
      // image file extension?
      if (extension == "txo" || downcase(extension) == "dds") {
        // A texture object.  Not exactly an image, but certainly a
        // texture.
        is_image = true;
        
      } else {
        TexturePool *texture_pool = TexturePool::get_global_ptr();
        if (texture_pool->get_texture_type(extension) != NULL) {
          // It is a known image file extension.
          is_image = true;
        }
      }
    }
  }

  LoaderOptions options = Panda::_loader_options;
  if (search) {
    options.set_flags(options.get_flags() | LoaderOptions::LF_search);
  } else {
    options.set_flags(options.get_flags() & ~LoaderOptions::LF_search);
  }

  Loader loader;
  PT(PandaNode) node;
  if (is_image) {
    node = load_image_as_model(filename);
  } else {
    node = loader.load_sync(filename, options);
  }

  if (node == (PandaNode *)NULL) {
    nout << "Unable to load " << filename << "\n";
    return NodePath::not_found();
  }

  return NodePath(node);
}

////////////////////////////////////////////////////////////////////
//     Function: Window::adjust_aspect_ratio
//       Access: Public
//  Description: Reevaluates the aspect ratio of the window,
//               presumably after the window has been resized by the
//               user or some other force.  Adjusts the render film
//               size and aspect2d scale as necessary according to the
//               new window shape, or new config setting.
////////////////////////////////////////////////////////////////////
void Window::
adjust_aspect_ratio() {
  float this_aspect_ratio = aspect_ratio;
  int x_size = 0, y_size = 0;
  if (this_aspect_ratio == 0.0f) {
    // An aspect ratio of 0.0 means to try to infer it.
    this_aspect_ratio = 1.0f;
    
    if (_window->has_size()) {
      x_size = _window->get_x_size();
      y_size = _window->get_y_size();
      if (y_size != 0) {
        this_aspect_ratio = (float)x_size / (float)y_size;
      }
    }
  }

  if (!_aspect_2d.is_empty()) {
    _aspect_2d.set_scale(1.0f / this_aspect_ratio, 1.0f, 1.0f);
  }

  Cameras::iterator ci;
  for (ci = _cameras.begin(); ci != _cameras.end(); ++ci) {
    Lens *lens = (*ci)->get_lens();
    if (lens != (Lens *)NULL) {
      if (y_size != 0) {
        lens->set_film_size(x_size, y_size);
      } else {
        lens->set_aspect_ratio(this_aspect_ratio);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Window::split_window
//       Access: Public
//  Description: Divides the window into two display regions, each of
//               which gets its own trackball and keyboard events.
//               The new window pointer is returned.
//
//               There is not an interface for recombining divided
//               windows.
////////////////////////////////////////////////////////////////////
Window *Window::
split_window(SplitType split_type) {
  DisplayRegion *new_region = NULL;

  if (split_type == ST_default) {
    // Choose either horizontal or vertical according to the largest
    // dimension.

    if (_display_region_3d->get_pixel_width() >
        _display_region_3d->get_pixel_height()) {
      split_type = ST_horizontal;
    } else {
      split_type = ST_vertical;
    }
  }

  float left, right, bottom, top;
  _display_region_3d->get_dimensions(left, right, bottom, top);
  new_region = _display_region_3d->get_window()->make_display_region();

  if (split_type == ST_vertical) {
    _display_region_3d->set_dimensions(left, right, bottom, (top + bottom) / 2.0f);
    if (_display_region_2d != (DisplayRegion *)NULL) {
      _display_region_2d->set_dimensions(left, right, bottom, (top + bottom) / 2.0f);
    }

    new_region->set_dimensions(left, right, (top + bottom) / 2.0f, top);

  } else {
    _display_region_3d->set_dimensions(left, (left + right) / 2.0f, bottom, top);
    if (_display_region_2d != (DisplayRegion *)NULL) {
      _display_region_2d->set_dimensions(left, (left + right) / 2.0f, bottom, top);
    }

    new_region->set_dimensions((left + right) / 2.0f, right, bottom, top);
  }

  PT(Window) wf = new Window(*this, new_region);
  _panda_framework->_windows.push_back(wf);

  return wf;
}


////////////////////////////////////////////////////////////////////
//     Function: Window::set_background_type
//       Access: Public
//  Description: Sets the background of the window to one of the
//               pre-canned background types (or to BT_other, which
//               indicates the user intends to set up his own special
//               background mode).
////////////////////////////////////////////////////////////////////
void Window::
set_background_type(Window::BackgroundType type) {
  _background_type = type;

  if (_display_region_3d == (DisplayRegion *)NULL) {
    return;
  }

  switch (_background_type) {
  case BT_other:
    break;

  case BT_default:
    _display_region_3d->set_clear_color_active(true);
    _display_region_3d->set_clear_depth_active(true);
    _display_region_3d->set_clear_stencil_active(true);
    _display_region_3d->set_clear_color(_window->get_clear_color());
    _display_region_3d->set_clear_depth(_window->get_clear_depth());
    _display_region_3d->set_clear_stencil(_window->get_clear_stencil());
    break;

  case BT_black:
    _display_region_3d->set_clear_color_active(true);
    _display_region_3d->set_clear_depth_active(true);
    _display_region_3d->set_clear_stencil_active(true);
    _display_region_3d->set_clear_color(Colorf(0.0f, 0.0f, 0.0f, 0.0f));
    _display_region_3d->set_clear_depth(1.0f);
    _display_region_3d->set_clear_stencil(0);
    break;

  case BT_gray:
    _display_region_3d->set_clear_color_active(true);
    _display_region_3d->set_clear_depth_active(true);
    _display_region_3d->set_clear_stencil_active(true);
    _display_region_3d->set_clear_color(Colorf(0.3f, 0.3f, 0.3f, 0.0f));
    _display_region_3d->set_clear_depth(1.0f);
    _display_region_3d->set_clear_stencil(0);
    break;

  case BT_white:
    _display_region_3d->set_clear_color_active(true);
    _display_region_3d->set_clear_depth_active(true);
    _display_region_3d->set_clear_stencil_active(true);
    _display_region_3d->set_clear_color(Colorf(1.0f, 1.0f, 1.0f, 0.0f));
    _display_region_3d->set_clear_depth(1.0f);
    _display_region_3d->set_clear_stencil(0);
    break;

  case BT_none:
    _display_region_3d->set_clear_color_active(false);
    _display_region_3d->set_clear_depth_active(false);
    _display_region_3d->set_clear_stencil_active(false);
    break;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Window::make_camera
//       Access: Public
//  Description: Makes a new 3-d camera for the window.
////////////////////////////////////////////////////////////////////
NodePath Window::
make_camera() {
  // Finally, we need a camera to associate with the display region.
  PT(Camera) camera = new Camera("camera");
  NodePath camera_np = get_camera_group().attach_new_node(camera);
  _cameras.push_back(camera);

  PT(Lens) lens = new PerspectiveLens;

  if (aspect_ratio != 0.0) {
    // If we're given an explict aspect ratio, use it
    lens->set_aspect_ratio(aspect_ratio);

  } else {
    // Otherwise, infer the aspect ratio from the window size.  This
    // does assume we have square pixels on our output device.
    if (_window->has_size()) {
      int x_size = _window->get_x_size();
      int y_size = _window->get_y_size();
      if (y_size != 0) {
        lens->set_film_size(x_size, y_size);
      }
    }
  }

  camera->set_lens(lens);

  return camera_np;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::load_image_as_model
//       Access: Private
//  Description: Loads the indicated image file as a texture, and
//               creates a polygon to render it.  Returns the new
//               model.
////////////////////////////////////////////////////////////////////
PT(PandaNode) Window::
load_image_as_model(const Filename &filename) {
  PT(Texture) tex = TexturePool::load_texture(filename);
  if (tex == NULL) {
    return NULL;
  }

  int x_size = tex->get_x_size() - tex->get_pad_x_size();
  int y_size = tex->get_y_size() - tex->get_pad_y_size();
  int full_x = tex->get_x_size();
  int full_y = tex->get_y_size();
  bool has_alpha = true;
  LVecBase2f tex_scale((x_size)*1.0f/full_x, (y_size*1.0f)/full_y);

  if (tex->is_of_type(VideoTexture::get_class_type())) {
    // Get the size from the video stream.
    VideoTexture *vtex = DCAST(VideoTexture, tex);
    x_size = vtex->get_video_width();
    y_size = vtex->get_video_height();
    tex_scale = vtex->get_tex_scale();
  } else {
    // Get the size from the original image (the texture may have
    // scaled it to make a power of 2).
    x_size = tex->get_orig_file_x_size();
    y_size = tex->get_orig_file_y_size();
  }

  // Yes, it is an image file; make a texture out of it.
  tex->set_minfilter(Texture::FT_linear_mipmap_linear);
  tex->set_magfilter(Texture::FT_linear);

  // Ok, now make a polygon to show the texture.

  // Choose the dimensions of the polygon appropriately.
  float left,right,top,bottom;
  if (x_size > y_size) {
    float scale = 10.0;
    left   = -scale;
    right  =  scale;
    top    =  (scale * y_size) / x_size;
    bottom = -(scale * y_size) / x_size;
  } else {
    float scale = 10.0;
    left   = -(scale * x_size) / y_size;
    right  =  (scale * x_size) / y_size;
    top    =  scale;
    bottom = -scale;
  }

  PT(GeomNode) card_node = new GeomNode("card");
  card_node->set_attrib(TextureAttrib::make(tex));
  if (has_alpha) {
    card_node->set_attrib(TransparencyAttrib::make(TransparencyAttrib::M_alpha));
  }

  PT(GeomVertexData) vdata = new GeomVertexData
    ("card", GeomVertexFormat::get_v3t2(),
     Geom::UH_static);
  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  GeomVertexWriter texcoord(vdata, InternalName::get_texcoord());

  vertex.add_data3f(Vertexf::rfu(left, 0.02f, top));
  vertex.add_data3f(Vertexf::rfu(left, 0.02f, bottom));
  vertex.add_data3f(Vertexf::rfu(right, 0.02f, top));
  vertex.add_data3f(Vertexf::rfu(right, 0.02f, bottom));

  texcoord.add_data2f(0.0f, tex_scale[1]);
  texcoord.add_data2f(0.0f, 0.0f);
  texcoord.add_data2f(tex_scale[0], tex_scale[1]);
  texcoord.add_data2f(tex_scale[0], 0.0f);

  PT(GeomTristrips) strip = new GeomTristrips(Geom::UH_static);
  strip->add_consecutive_vertices(0, 4);
  strip->close_primitive();

  PT(Geom) geom = new Geom(vdata);
  geom->add_primitive(strip);

  card_node->add_geom(geom);

  return card_node.p();
}

