// Filename: curveDrawer.h
// Created by:  drose (14Mar97)
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
#ifndef CURVEDRAWER_H
#define CURVEDRAWER_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curve.h"
#include "lineSegs.h"
#include <Performer/pr/pfLinMath.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass ParametricCurveDrawer
#exportfunc make_ParametricCurveDrawer
#exportfunc rm_ParametricCurveDrawer
#exportmember ParametricCurveDrawer set_curve get_curve
#wrappermember pf-downcast-null-convert ParametricCurveDrawer get_curve
#exportmember ParametricCurveDrawer set_time_curve get_time_curve
#wrappermember pf-downcast-null-convert ParametricCurveDrawer get_time_curve
#exportmember ParametricCurveDrawer set_surface get_surface
#wrappermember pf-downcast-null-convert ParametricCurveDrawer get_surface
#exportmember ParametricCurveDrawer get_geode detach_geode
#exportmember ParametricCurveDrawer set_num_segs get_num_segs
#exportmember ParametricCurveDrawer set_num_ticks get_num_ticks
#exportmember ParametricCurveDrawer set_color set_tick_color
#exportmember ParametricCurveDrawer set_frame_accurate
#exportmember ParametricCurveDrawer draw recompute hide
#exportmember ParametricCurveDrawer set_tick_scale get_tick_scale
#exportmember ParametricCurveDrawer get_frame_accurate
#exportmember ParametricCurveDrawer set_graph_type

#exportsymbol PCD_DEFAULT
#exportsymbol PCD_XVST
#exportsymbol PCD_YVST
#exportsymbol PCD_ZVST
#exportsymbol PCD_DXVST
#exportsymbol PCD_DYVST
#exportsymbol PCD_DZVST
#exportsymbol PCD_IXVST
#exportsymbol PCD_IYVST
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////

typedef pfVec3 VecTypeMapper(const VecType &point, 
			     const VecType &tangent, 
			     double t);

// The different kinds of ParametricCurveDrawer graph types
#define PCD_DEFAULT 1
#define PCD_XVST    2
#define PCD_YVST    3
#define PCD_ZVST    4
#define PCD_DXVST   6
#define PCD_DYVST   7
#define PCD_DZVST   8
#define PCD_IXVST   9
#define PCD_IYVST   10

class ParametricSurface;

////////////////////////////////////////////////////////////////////
// 	 Class : ParametricCurveDrawer
// Description : Draws a 3-d parametric curve in the scene by creating
//               a series of line segments to approximate the curve.
////////////////////////////////////////////////////////////////////
class ParametricCurveDrawer {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  ParametricCurveDrawer(ParametricCurve *curve);
  virtual ~ParametricCurveDrawer();

  void set_curve(ParametricCurve *curve);
  ParametricCurve *get_curve();

  void set_time_curve(ParametricCurve *curve);
  ParametricCurve *get_time_curve();

  void set_surface(ParametricSurface *surface);
  ParametricSurface *get_surface();

  pfGeode *get_geode();
  pfGeode *detach_geode();

  void set_num_segs(int num_segs);
  int get_num_segs() const;

  void set_num_ticks(int num_ticks);
  int get_num_ticks() const;

  void set_color(float r, float g, float b);
  void set_tick_color(float r, float g, float b);

  void set_frame_accurate(boolean frame_accurate);
  boolean get_frame_accurate() const;

  virtual boolean draw();
  virtual boolean recompute(double t1, double t2, ParametricCurve *curve=NULL);
  void hide();

  void set_tick_scale(double scale);
  double get_tick_scale() const;

  void set_graph_type(int graph_type);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  double get_max_t() const {
    return _time_curve==NULL ? _curve->get_max_t() : _time_curve->get_max_t();
  }

  void disable(ParametricCurve *curve);

  void set_mapper(VecTypeMapper *mapper);

  static pfVec3 DefaultMap(const VecType &point, const VecType &, double);
  static pfVec3 XvsT(const VecType &point, const VecType &, double t);
  static pfVec3 iXvsT(const VecType &point, const VecType &, double t);
  static pfVec3 YvsT(const VecType &point, const VecType &, double t);
  static pfVec3 iYvsT(const VecType &point, const VecType &, double t);
  static pfVec3 ZvsT(const VecType &point, const VecType &, double t);
  static pfVec3 dXvsT(const VecType &, const VecType &tangent, double t);
  static pfVec3 dYvsT(const VecType &, const VecType &tangent, double t);
  static pfVec3 dZvsT(const VecType &, const VecType &tangent, double t);

protected:
  static void get_tick_marks(const pfVec3 &tangent, pfVec3 &t1, pfVec3 &t2);

  pfGeode *_geode;
  int _num_segs;
  ParametricCurve *_curve, *_time_curve;
  ParametricSurface *_surface;
  LineSegs _lines, _ticks;
  boolean _drawn;
  int _num_ticks;
  double _tick_scale;
  boolean _frame_accurate;
  VecTypeMapper *_mapper;
};

ParametricCurveDrawer* make_ParametricCurveDrawer( ParametricCurve *curve );
void rm_ParametricCurveDrawer( ParametricCurveDrawer* pcd );

  
#endif
