// Filename: osMesaGraphicsStateGuardian.cxx
// Created by:  drose (09Feb04)
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

#include "osMesaGraphicsStateGuardian.h"

#ifndef WIN32_VC
// Define this symbol if we should use dlopen() to look for osmesa
// extensions.
#define USE_DLOPEN
#endif

#ifdef USE_DLOPEN
#include <dlfcn.h>
#endif

TypeHandle OSMesaGraphicsStateGuardian::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: OSMesaGraphicsStateGuardian::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
OSMesaGraphicsStateGuardian::
OSMesaGraphicsStateGuardian(const FrameBufferProperties &properties,
                            OSMesaGraphicsStateGuardian *share_with) : 
  MesaGraphicsStateGuardian(properties)
{
  OSMesaContext share_context = NULL;
  if (share_with != (OSMesaGraphicsStateGuardian *)NULL) {
    share_context = share_with->_context;
    _prepared_objects = share_with->get_prepared_objects();
  }

  _context = OSMesaCreateContext(OSMESA_RGBA, share_context);

  // I think OSMesa always creates single-buffered contexts.  I don't
  // see any documentation to this effect, but it seems to be the
  // case.
  FrameBufferProperties props = get_properties();
  int mode = props.get_frame_buffer_mode();
  mode = (mode & ~FrameBufferProperties::FM_buffer) | FrameBufferProperties::FM_single_buffer;
  props.set_frame_buffer_mode(mode);
  set_properties(props);

  _dl_handle = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: OSMesaGraphicsStateGuardian::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
OSMesaGraphicsStateGuardian::
~OSMesaGraphicsStateGuardian() {
  if (_context != (OSMesaContext)NULL) {
    OSMesaDestroyContext(_context);
    _context = (OSMesaContext)NULL;
  }

#ifdef USE_DLOPEN
  if (_dl_handle != (void *)NULL) {
    dlclose(_dl_handle);
  }
#endif  // USE_DLOPEN
}

////////////////////////////////////////////////////////////////////
//     Function: OSMesaGraphicsStateGuardian::get_extension_func
//       Access: Public, Virtual
//  Description: Returns the pointer to the GL extension function with
//               the indicated name.  It is the responsibility of the
//               caller to ensure that the required extension is
//               defined in the OpenGL runtime prior to calling this;
//               it is an error to call this for a function that is
//               not defined.
////////////////////////////////////////////////////////////////////
void *OSMesaGraphicsStateGuardian::
get_extension_func(const char *name) {
#ifdef USE_DLOPEN
  if (_dl_handle == (void *)NULL) {
    _dl_handle = dlopen(NULL, RTLD_LAZY);
    nassertr(_dl_handle != (void *)NULL, NULL);
  }

  return dlsym(_dl_handle, name);
#else  // USE_DLOPEN
  return NULL;
#endif  // USE_DLOPEN
}
