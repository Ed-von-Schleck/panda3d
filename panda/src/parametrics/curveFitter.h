// Filename: curveFitter.h
// Created by:  drose (17Sep98)
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

#ifndef CURVEFITTER_H
#define CURVEFITTER_H

#include <typedef.h>
#include <linMathOutput.h>
#include <vector.h>
#include <Performer/pr/pfLinMath.h>
#include <iostream.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////

/*$ 
#typehint boolean int

#exportclass CurveFitter
#exportfunc make_CurveFitter rm_CurveFitter

#exportmember CurveFitter reset
#exportmember CurveFitter add_point
#exportmember CurveFitter sample
#exportmember CurveFitter generate_even
#exportmember CurveFitter wrap_hpr
#exportmember CurveFitter compute_timewarp
#exportmember CurveFitter sort_points
#exportmember CurveFitter desample
#exportmember CurveFitter compute_tangents
#exportmember CurveFitter make_hermite
#exportmember CurveFitter make_nurbs
#exportmember CurveFitter print

$*/

class HermiteCurve;
class NurbsCurve;
class ParametricCurve;

////////////////////////////////////////////////////////////////////
//       Class : CurveFitter
// Description : 
////////////////////////////////////////////////////////////////////
class CurveFitter {
public:
  void reset();
  void add_point(double t, const pfVec3 &point);

  void sample(ParametricCurve *curve, int count, boolean even);
  void generate_even(int count, double net_distance, double net_time);

  void wrap_hpr();
  void compute_timewarp(const ParametricCurve *xyz);
  void sort_points();
  void desample(double factor);

  void compute_tangents(double scale);
  HermiteCurve *make_hermite() const;
  NurbsCurve *make_nurbs() const;
  
  void print() const;

  void output(ostream &out) const;

  class DataPoint {
  public:
    DataPoint() : _t(0.0), _point(0.0, 0.0, 0.0), _tangent(0.0, 0.0, 0.0) { }
    void output(ostream &out) const {
      out << "Time " << _t << " point " << _point << " tan " << _tangent;
    }

    int operator < (const DataPoint &other) const {
      return _t < other._t;
    }
    
    double _t;
    pfVec3 _point;
    pfVec3 _tangent;
  };
  
  typedef vector<DataPoint> Data;
  Data _data;
};

inline ostream &operator << (ostream &out, const CurveFitter::DataPoint &dp) {
  dp.output(out);
  return out;
}

inline ostream &operator << (ostream &out, const CurveFitter &cf) {
  cf.output(out);
  return out;
}

CurveFitter *make_CurveFitter();
void rm_CurveFitter(CurveFitter *cf);

#endif
