// Filename: glxGraphicsStateGuardian.cxx
// Created by:  drose (27Jan03)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "glxGraphicsStateGuardian.h"

#include <dlfcn.h>


TypeHandle glxGraphicsStateGuardian::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: glxGraphicsStateGuardian::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
glxGraphicsStateGuardian::
glxGraphicsStateGuardian(const FrameBufferProperties &properties,
                         glxGraphicsStateGuardian *share_with,
                         GLXContext context, GLXFBConfig fbconfig,
                         XVisualInfo *visual, Display *display, int screen) :
  GLGraphicsStateGuardian(properties),
  _context(context),
  _fbconfig(fbconfig),
  _visual(visual),
  _display(display),
  _screen(screen)
{
  if (share_with != (glxGraphicsStateGuardian *)NULL) {
    _prepared_objects = share_with->get_prepared_objects();
  }
  _libgl_handle = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: glxGraphicsStateGuardian::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
glxGraphicsStateGuardian::
~glxGraphicsStateGuardian() {
  if (_visual != (XVisualInfo *)NULL) {
    XFree(_visual);
  }
  if (_context != (GLXContext)NULL) {
    glXDestroyContext(_display, _context);
    _context = (GLXContext)NULL;
  }
  if (_libgl_handle != (void *)NULL) {
    dlclose(_libgl_handle);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: glxGraphicsStateGuardian::get_extra_extensions
//       Access: Protected, Virtual
//  Description: This may be redefined by a derived class (e.g. glx or
//               wgl) to get whatever further extensions strings may
//               be appropriate to that interface, in addition to the
//               GL extension strings return by glGetString().
////////////////////////////////////////////////////////////////////
void glxGraphicsStateGuardian::
get_extra_extensions() {
  save_extensions(glXQueryExtensionsString(_display, _screen));
}

////////////////////////////////////////////////////////////////////
//     Function: glxGraphicsStateGuardian::get_extension_func
//       Access: Public, Virtual
//  Description: Returns the pointer to the GL extension function with
//               the indicated name.  It is the responsibility of the
//               caller to ensure that the required extension is
//               defined in the OpenGL runtime prior to calling this;
//               it is an error to call this for a function that is
//               not defined.
////////////////////////////////////////////////////////////////////
void *glxGraphicsStateGuardian::
get_extension_func(const char *name) {
  if (_libgl_handle == (void *)NULL) {
    _libgl_handle = dlopen("libGL.so", RTLD_LAZY);
    nassertr(_libgl_handle != (void *)NULL, NULL);
  }
  
  return dlsym(_libgl_handle, name);
}
