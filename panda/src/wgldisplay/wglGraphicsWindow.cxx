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
  wglMakeCurrent(_hdc, wglgsg->get_context(_hdc));

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

  _hdc = GetDC(_hWnd);

  // Set up the pixel format of the window appropriately for GL.
  int pfnum = wglgsg->get_pfnum();

  PIXELFORMATDESCRIPTOR pixelformat;
  DescribePixelFormat(_hdc, pfnum, sizeof(PIXELFORMATDESCRIPTOR), 
                      &pixelformat);

#ifdef _DEBUG
  char msg[200];
  sprintf(msg, "Selected GL PixelFormat is #%d", pfnum);
  print_pfd(&pixelformat, msg);
#endif

  if (!SetPixelFormat(_hdc, pfnum, &pixelformat)) {
    wgldisplay_cat.error()
      << "SetPixelFormat(" << pfnum << ") failed after window create\n";
    close_window();
    return false;
  }

  // Initializes _colormap
  setup_colormap(pixelformat);
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
  ReleaseDC(_hWnd, _hdc);
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
setup_colormap(const PIXELFORMATDESCRIPTOR &pixelformat) {
  LOGPALETTE *logical;
  int n;

  if (!(pixelformat.dwFlags & PFD_NEED_PALETTE ||
      pixelformat.iPixelType == PFD_TYPE_COLORINDEX))
    return;

  n = 1 << pixelformat.cColorBits;

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

  if (pixelformat.iPixelType == PFD_TYPE_RGBA) {
    int redMask = (1 << pixelformat.cRedBits) - 1;
    int greenMask = (1 << pixelformat.cGreenBits) - 1;
    int blueMask = (1 << pixelformat.cBlueBits) - 1;
    int i;

    /* fill in an RGBA color palette */
    for (i = 0; i < n; ++i) {
      logical->palPalEntry[i].peRed =
        (((i >> pixelformat.cRedShift)   & redMask)   * 255) / redMask;
      logical->palPalEntry[i].peGreen =
        (((i >> pixelformat.cGreenShift) & greenMask) * 255) / greenMask;
        logical->palPalEntry[i].peBlue =
        (((i >> pixelformat.cBlueShift)  & blueMask)  * 255) / blueMask;
      logical->palPalEntry[i].peFlags = 0;
    }
  }

  _colormap = CreatePalette(logical);
  free(logical);

  SelectPalette(_hdc, _colormap, FALSE);
  RealizePalette(_hdc);
}

#ifdef _DEBUG
////////////////////////////////////////////////////////////////////
//     Function: wglGraphicsWindow::print_pfd
//       Access: Private, Static
//  Description: Reports information about the selected pixel format
//               descriptor, along with the indicated message.
////////////////////////////////////////////////////////////////////
void wglGraphicsWindow::
print_pfd(PIXELFORMATDESCRIPTOR *pfd, char *msg) {
  OGLDriverType drvtype;
  if ((pfd->dwFlags & PFD_GENERIC_ACCELERATED) && 
      (pfd->dwFlags & PFD_GENERIC_FORMAT)) {
    drvtype=MCD;
  } else if (!(pfd->dwFlags & PFD_GENERIC_ACCELERATED) && !(pfd->dwFlags & PFD_GENERIC_FORMAT)) {
    drvtype=ICD;
  } else {
    drvtype=Software;
  }

#define PRINT_FLAG(FLG) ((pfd->dwFlags &  PFD_##FLG) ? (" PFD_" #FLG "|") : "")
  wgldisplay_cat.spam()
    << "================================\n";

  wgldisplay_cat.spam()
    << msg << ", " << OGLDrvStrings[drvtype] << " driver\n"
    << "PFD flags: 0x" << (void*)pfd->dwFlags << " (" 
    << PRINT_FLAG(GENERIC_ACCELERATED) 
    << PRINT_FLAG(GENERIC_FORMAT)
    << PRINT_FLAG(DOUBLEBUFFER)
    << PRINT_FLAG(SUPPORT_OPENGL)
    << PRINT_FLAG(SUPPORT_GDI)
    << PRINT_FLAG(STEREO)
    << PRINT_FLAG(DRAW_TO_WINDOW)
    << PRINT_FLAG(DRAW_TO_BITMAP)
    << PRINT_FLAG(SWAP_EXCHANGE)
    << PRINT_FLAG(SWAP_COPY)
    << PRINT_FLAG(SWAP_LAYER_BUFFERS)
    << PRINT_FLAG(NEED_PALETTE)
    << PRINT_FLAG(NEED_SYSTEM_PALETTE)
    << PRINT_FLAG(SUPPORT_DIRECTDRAW) << ")\n"
    << "PFD iPixelType: "
    << ((pfd->iPixelType==PFD_TYPE_RGBA) ? "PFD_TYPE_RGBA":"PFD_TYPE_COLORINDEX")
    << endl
    << "PFD cColorBits: " << (DWORD)pfd->cColorBits
    << "  R: " << (DWORD)pfd->cRedBits
    <<" G: " << (DWORD)pfd->cGreenBits
    <<" B: " << (DWORD)pfd->cBlueBits << endl
    << "PFD cAlphaBits: " << (DWORD)pfd->cAlphaBits
    << "  DepthBits: " << (DWORD)pfd->cDepthBits
    <<" StencilBits: " << (DWORD)pfd->cStencilBits
    <<" AccumBits: " << (DWORD)pfd->cAccumBits
    << endl;
}
#endif
