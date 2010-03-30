// Filename: window.I
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
//     Function: Window::get_panda_framework
//       Access: Public
//  Description: Returns a pointer to the associated Panda
//               object.
////////////////////////////////////////////////////////////////////
INLINE Panda *Window::
get_panda_framework() const {
  return _panda_framework;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_graphics_window
//       Access: Public
//  Description: Returns a pointer to the underlying GraphicsWindow
//               object, if it is in fact a window; or NULL if it is
//               not.
////////////////////////////////////////////////////////////////////
INLINE GraphicsWindow *Window::
get_graphics_window() const {
  if (_window != (GraphicsOutput *)NULL && 
      _window->is_of_type(GraphicsWindow::get_class_type())) {
    return DCAST(GraphicsWindow, _window);
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_graphics_output
//       Access: Public
//  Description: Returns a pointer to the underlying GraphicsOutput
//               object
////////////////////////////////////////////////////////////////////
INLINE GraphicsOutput *Window::
get_graphics_output() const {
  return _window;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_num_cameras
//       Access: Public
//  Description: Returns the number of 3-d cameras associated with the
//               window.  A window usually has only one camera, but it
//               may have multiple cameras if there are multiple
//               display regions within the window.
////////////////////////////////////////////////////////////////////
INLINE int Window::
get_num_cameras() const {
  return _cameras.size();
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_camera
//       Access: Public
//  Description: Returns the nth camera associated with the window.
////////////////////////////////////////////////////////////////////
INLINE Camera *Window::
get_camera(int n) const {
  nassertr(n >= 0 && n < (int)_cameras.size(), NULL);
  return _cameras[n];
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_display_region_2d
//       Access: Public
//  Description: Returns the default DisplayRegion created for the 2-d
//               scene (render2d).
////////////////////////////////////////////////////////////////////
INLINE DisplayRegion *Window::
get_display_region_2d() const {
  return _display_region_2d;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_display_region_3d
//       Access: Public
//  Description: Returns the default DisplayRegion created for the 3-d
//               scene (render).
////////////////////////////////////////////////////////////////////
INLINE DisplayRegion *Window::
get_display_region_3d() const {
  return _display_region_3d;
}

////////////////////////////////////////////////////////////////////
//     Function: Window::get_background_type
//       Access: Public
//  Description: Returns the current background type setting.
////////////////////////////////////////////////////////////////////
INLINE Window::BackgroundType Window::
get_background_type() const {
  return _background_type;
}
