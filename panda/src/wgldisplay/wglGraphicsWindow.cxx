// Filename: wglGraphicsWindow.cxx
// Created by:  drose (20Dec02)
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

#include "wglGraphicsWindow.h"
#include "config_wgldisplay.h"
#include "config_windisplay.h"
#include "wglGraphicsPipe.h"
#include "glGraphicsStateGuardian.h"

#include <wingdi.h>

TypeHandle wglGraphicsWindow::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
wglGraphicsWindow::
wglGraphicsWindow(GraphicsPipe *pipe, GraphicsStateGuardian *gsg) :
  WinGraphicsWindow(pipe, gsg) 
{
  _hdc = (HDC)0;
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
wglGraphicsWindow::
~wglGraphicsWindow() {
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::make_current
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               during begin_frame() to ensure the graphics context
//               is ready for drawing.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
make_current() {
  wglGraphicsStateGuardian *wglgsg;
  DCAST_INTO_V(wglgsg, _gsg);
  wglMakeCurrent(_hdc, wglgsg->_context);

  // Now that we have made the context current to a window, we can
  // reset the GSG state if this is the first time it has been used.
  // (We can't just call reset() when we construct the GSG, because
  // reset() requires having a current context.)
  wglgsg->reset_if_new();
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::release_gsg
//       Access: Public, Virtual
//  Description: Releases the current GSG pointer, if it is currently
//               held, and resets the GSG to NULL.  The window will be
//               permanently unable to render; this is normally called
//               only just before destroying the window.  This should
//               only be called from within the draw thread.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
release_gsg() {
  wglMakeCurrent(_hdc, NULL);
  GraphicsWindow::release_gsg();
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::begin_flip
//       Access: Public, Virtual
//  Description: This function will be called within the draw thread
//               after end_frame() has been called on all windows, to
//               initiate the exchange of the front and back buffers.
//
//               This should instruct the window to prepare for the
//               flip at the next video sync, but it should not wait.
//
//               We have the two separate functions, begin_flip() and
//               end_flip(), to make it easier to flip all of the
//               windows at the same time.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
begin_flip() {
  if (_gsg != (GraphicsStateGuardian *)NULL) {
    make_current();
    glFinish();
    SwapBuffers(_hdc);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::open_window
//       Access: Protected, Virtual
//  Description: Opens the window right now.  Called from the window
//               thread.  Returns true if the window is successfully
//               opened, or false if there was a problem.
////////////////////////////////////////////////////////////////////
bool wglGraphicsWindow::
open_window() {
  if (!WinGraphicsWindow::open_window()) {
    return false;
  }

  wglGraphicsStateGuardian *wglgsg;
  DCAST_INTO_R(wglgsg, _gsg, false);

  // Set up the pixel format of the window appropriately for GL.
  _hdc = GetDC(_mwindow);

  if (!SetPixelFormat(_hdc, wglgsg->_pfnum, &wglgsg->_pixelformat)) {
    wgldisplay_cat.error()
      << "SetPixelFormat(" << wglgsg->_pfnum
      << ") failed after window create\n";
    close_window();
    return false;
  }

  // Initializes _colormap
  setup_colormap();
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::close_window
//       Access: Protected, Virtual
//  Description: Closes the window right now.  Called from the window
//               thread.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
close_window() {
  ReleaseDC(_mwindow, _hdc);
  _hdc = (HDC)0;
  WinGraphicsWindow::close_window();
}

////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::setup_colormap
//       Access: Private
//  Description: Sets up a colormap for the window matching the
//               selected pixel format.  This is necessary before
//               creating a GL context.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
setup_colormap() {
  PIXELFORMATDESCRIPTOR pfd;
  LOGPALETTE *logical;
  int n;

  /* grab the pixel format */
  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  DescribePixelFormat(_hdc, GetPixelFormat(_hdc),
                      sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  if (!(pfd.dwFlags & PFD_NEED_PALETTE ||
      pfd.iPixelType == PFD_TYPE_COLORINDEX))
    return;

  n = 1 << pfd.cColorBits;

  /* allocate a bunch of memory for the logical palette (assume 256
     colors in a Win32 palette */
  logical = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
                                sizeof(PALETTEENTRY) * n);
  memset(logical, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);

  /* set the entries in the logical palette */
  logical->palVersion = 0x300;
  logical->palNumEntries = n;

  /* start with a copy of the current system palette */
  GetSystemPaletteEntries(_hdc, 0, 256, &logical->palPalEntry[0]);

  if (pfd.iPixelType == PFD_TYPE_RGBA) {
    int redMask = (1 << pfd.cRedBits) - 1;
    int greenMask = (1 << pfd.cGreenBits) - 1;
    int blueMask = (1 << pfd.cBlueBits) - 1;
    int i;

    /* fill in an RGBA color palette */
    for (i = 0; i < n; ++i) {
      logical->palPalEntry[i].peRed =
        (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
      logical->palPalEntry[i].peGreen =
        (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
        logical->palPalEntry[i].peBlue =
        (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
      logical->palPalEntry[i].peFlags = 0;
    }
  }

  _colormap = CreatePalette(logical);
  free(logical);

  SelectPalette(_hdc, _colormap, FALSE);
  RealizePalette(_hdc);
}
