// Filename: hermiteCurveDrawer.h
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
#ifndef HERMITECURVEDRAWER_H
#define HERMITECURVEDRAWER_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curveDrawer.h"
#include "hermiteCurve.h"
#include "lineSegs.h"
#include <Performer/pr/pfLinMath.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass HermiteCurveDrawer
#exportfunc  make_HermiteCurveDrawer
#exportfunc  rm_HermiteCurveDrawer
#exportmember HermiteCurveDrawer set_cv_color
#exportmember HermiteCurveDrawer draw recompute
#exportmember HermiteCurveDrawer set_show_cvs get_show_cvs
#exportmember HermiteCurveDrawer hilight unhilight
#exportmember HermiteCurveDrawer set_tangent_scale get_tangent_scale
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 	 Class : HermiteCurveDrawer
// Description : Draws a Hermite curve, also drawing in the control
//               vertices and tangent vectors.
////////////////////////////////////////////////////////////////////
class HermiteCurveDrawer : public ParametricCurveDrawer {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  HermiteCurveDrawer(HermiteCurve *curve);
  virtual ~HermiteCurveDrawer();

  void set_cv_color(float r, float g, float b);

  virtual boolean draw();
  virtual boolean recompute(double t1, double t2, ParametricCurve *curve=NULL);

  void set_show_cvs(boolean flag);
  boolean get_show_cvs() const;

  boolean hilight(int n, float hr=1.0, float hg=1.0, float hb=0.0);
  boolean unhilight(int n);

  void set_tangent_scale(double scale);
  double get_tangent_scale() const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
protected:
  float _pr, _pg, _pb;
  LineSegs _points, _tangents;
  int _num_points;
  boolean _show_cvs;
  double _tangent_scale;
};

HermiteCurveDrawer* make_HermiteCurveDrawer( HermiteCurve* Curve );
void rm_HermiteCurveDrawer( HermiteCurveDrawer* hcd );
  
#endif

