// Filename: patchDrawer.h
// Created by:  drose (18Apr97)
// 
////////////////////////////////////////////////////////////////////
// Copyright (C) 1992,93,94,95,96,97  Walt Disney Imagineering, Inc.
// 
// These  coded  instructions,  statements,  data   structures   and
// computer  programs contain unpublished proprietary information of
// Walt Disney Imagineering and are protected by  Federal  copyright
// law.  They may  not be  disclosed to third  parties  or copied or
// duplicated in any form, in whole or in part,  without  the  prior
// written consent of Walt Disney Imagineering Inc.
////////////////////////////////////////////////////////////////////
//
#ifndef PATCHDRAWER_H
#define PATCHDRAWER_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "patch.h"
#include <Performer/pr/pfLinMath.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint bool int
#typehint VecType LVector3f

#exportfunc make_ParametricSurfaceDrawer rm_ParametricSurfaceDrawer

#exportclass ParametricSurfaceDrawer
#exportmember ParametricSurfaceDrawer get_surface
#wrappermember pf-downcast-null-convert ParametricSurfaceDrawer get_surface

#exportmember ParametricSurfaceDrawer set_num_s get_num_s
#exportmember ParametricSurfaceDrawer set_num_t get_num_t
#exportmember ParametricSurfaceDrawer get_geode
#exportmember ParametricSurfaceDrawer detach_geode
#exportmember ParametricSurfaceDrawer set_color
#exportmember ParametricSurfaceDrawer draw hide
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// 	 Class : ParametricSurfaceDrawer
// Description : Draws a 3-d parametric surface in the scene by creating
//               a series of polygons to approximate the surface with a
//               uniform subdivision.
////////////////////////////////////////////////////////////////////
class ParametricSurfaceDrawer {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  ParametricSurfaceDrawer(ParametricSurface *surface);
  virtual ~ParametricSurfaceDrawer();

  ParametricSurface *get_surface();

  pfGeode *get_geode();
  pfGeode *detach_geode();

  void set_num_s(int num_s);
  int get_num_s() const;

  void set_num_t(int num_t);
  int get_num_t() const;

  void set_color(float r, float g, float b);

  bool draw();
  void hide();

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////

public:

  pfGeode *_geode;
  int _num_s, _num_t;
  ParametricSurface *_surface;
  pfVec4 _color;
  bool _drawn;
  static pfGeoState *geostate;
};

ParametricSurfaceDrawer *make_ParametricSurfaceDrawer(ParametricSurface *surface);
void rm_ParametricSurfaceDrawer(ParametricSurfaceDrawer *psd);


#endif
