// Filename: nurbsCurveDrawer.h
// Created by:  drose (27Feb98)
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
#ifndef NURBSCURVEDRAWER_H
#define NURBSCURVEDRAWER_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curveDrawer.h"
#include "nurbsCurve.h"
#include "lineSegs.h"
#include <Performer/pr/pfLinMath.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass NurbsCurveDrawer
#exportfunc  make_NurbsCurveDrawer
#exportfunc  rm_NurbsCurveDrawer
#exportmember NurbsCurveDrawer set_cv_color
#exportmember NurbsCurveDrawer set_hull_color
#exportmember NurbsCurveDrawer set_knot_color
#exportmember NurbsCurveDrawer draw recompute
#exportmember NurbsCurveDrawer set_show_cvs get_show_cvs
#exportmember NurbsCurveDrawer set_show_hull get_show_hull
#exportmember NurbsCurveDrawer set_show_knots get_show_knots
#exportmember NurbsCurveDrawer hilight unhilight
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 	 Class : NurbsCurveDrawer
// Description : Draws a Nurbs curve, also drawing in the control
//               vertices and tangent vectors.
////////////////////////////////////////////////////////////////////
class NurbsCurveDrawer : public ParametricCurveDrawer {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  NurbsCurveDrawer(NurbsCurve *curve);
  virtual ~NurbsCurveDrawer();

  void set_cv_color(float r, float g, float b);
  void set_hull_color(float r, float g, float b);
  void set_knot_color(float r, float g, float b);

  virtual boolean draw();
  virtual boolean recompute(double t1, double t2, ParametricCurve *curve=NULL);

  void set_show_cvs(boolean flag);
  boolean get_show_cvs() const;
  void set_show_hull(boolean flag);
  boolean get_show_hull() const;
  void set_show_knots(boolean flag);
  boolean get_show_knots() const;

  boolean hilight(int n, float hr=1.0, float hg=1.0, float hb=0.0);
  boolean unhilight(int n);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
protected:
  pfVec3 _cv_color, _hull_color, _knot_color;
  int _num_cvs, _num_hull, _num_knots;
  LineSegs _hull, _knots, _cvs;
  vector<int> _knotnums;

  boolean _show_cvs, _show_hull, _show_knots;
};

NurbsCurveDrawer* make_NurbsCurveDrawer( NurbsCurve* Curve );
void rm_NurbsCurveDrawer( NurbsCurveDrawer* hcd );
  
#endif


