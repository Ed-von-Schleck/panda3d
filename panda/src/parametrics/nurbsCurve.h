// Filename: nurbsCurve.h
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
#ifndef NURBSCURVE_H
#define NURBSCURVE_H

////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curve.h"

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////

/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass NurbsCurve
#exportfunc make_NurbsCurve rm_NurbsCurve

#exportmember NurbsCurve set_order get_order
#exportmember NurbsCurve get_num_cvs
#exportmember NurbsCurve insert_cv
#exportmember NurbsCurve append_cv
#exportmember NurbsCurve remove_cv
#exportmember NurbsCurve remove_all_cvs
#exportmember NurbsCurve set_cv_point get_cv_point
#exportmember NurbsCurve set_cv_weight get_cv_weight
#exportmember NurbsCurve set_knot get_knot
#exportmember NurbsCurve Print print_cv
#exportmember NurbsCurve normalize_tlength
#exportmember NurbsCurve recompute
#exportmember NurbsCurve write_egg
#exportmember NurbsCurve splice

$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////

#define VecType pfVec3
//typedef pfVec3 VecType;


class HermiteCurve;

////////////////////////////////////////////////////////////////////
// 	 Class : NurbsCurve
// Description : A Nonuniform Rational B-Spline.
//
//               This class is actually implemented as a
//               PiecewiseCurve made up of several CubicCurvesegs,
//               each of which is created using the nurbs_basis()
//               method.  The list of CV's and knots is kept here,
//               within the NurbsCurve class.
////////////////////////////////////////////////////////////////////
class NurbsCurve : public PiecewiseCurve {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  NurbsCurve();
  NurbsCurve(const ParametricCurve &hc);
  NurbsCurve(int order, int num_cvs,
	     const double knots[], const pfVec4 cvs[]);

  void set_order(int order);
  int get_order() const;

  int get_num_cvs() const;
  int get_num_knots() const {
    return _cvs.size() + _order;
  }

  int insert_cv(double t);
  int append_cv(float x, float y, float z);
  inline int append_cv(const VecType &v) {
    return append_cv(pfVec4(v[0], v[1], v[2], 1.0));
  }
  inline int append_cv(const pfVec4 &v) {
    _cvs.push_back(CV(v, GetKnot(_cvs.size())+1.0));
    return _cvs.size()-1;
  }

  boolean remove_cv(int n);
  void remove_all_cvs();

  boolean set_cv_point(int n, float x, float y, float z);
  inline boolean set_cv_point(int n, const VecType &v) {
    return set_cv_point(n, v[0], v[1], v[2]);
  }
  void get_cv_point(int n, VecType &v) const;
  const VecType &get_cv_point(int n) const;

  boolean set_cv_weight(int n, float w);
  float get_cv_weight(int n) const;

  boolean set_knot(int n, double t);
  double get_knot(int n) const;

  void Print() const;
  void print_cv(int n) const;

  boolean recompute();

  void normalize_tlength();

  virtual boolean
  rebuild_curveseg(int rtype0, double t0, const pfVec4 &v0,
		   int rtype1, double t1, const pfVec4 &v1,
		   int rtype2, double t2, const pfVec4 &v2,
		   int rtype3, double t3, const pfVec4 &v3);

  boolean write_egg(const char *filename);
  boolean write_egg(ostream &out, const char *basename);

  void splice(double t, const NurbsCurve &other);
  
////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:

  CubicCurveseg *get_curveseg(int ti) {
    return (CubicCurveseg *)PiecewiseCurve::get_curveseg(ti);
  }

  double
  GetKnot(int n) const {
    if (n < _order || _cvs.empty()) {
      return 0.0;
    } else if (n-1 >= _cvs.size()) {
      return _cvs.back()._t;
    } else {
      return _cvs[n-1]._t;
    }
  }

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new NurbsCurve; }
  static int st_descend_pfb(pfNode *node, void *handle) {
    return ((NurbsCurve *)node)->descend_pfb(handle);
  }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((NurbsCurve *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((NurbsCurve *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

  void Output(ostream &out, int indent=0) const;

protected:
  virtual ~NurbsCurve();

  int FindCV(double t);

  int _order;

  class CV {
  public:
    CV() {}
    CV(const pfVec4 &p, double t) : _p(p), _t(t) {}
    pfVec4 _p;
    double _t;
  };

  perf_vector<CV> _cvs;

private:
  static pfType *classType;
};

NurbsCurve* make_NurbsCurve();
NurbsCurve* make_NurbsCurve(const ParametricCurve &hc);
void rm_NurbsCurve(NurbsCurve *curve);

#endif
