// Filename: hermiteCurve.h
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
#ifndef HERMITECURVE_H
#define HERMITECURVE_H

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

#exportclass HermiteCurve
#exportfunc make_HermiteCurve rm_HermiteCurve
#exportmember HermiteCurve get_num_cvs insert_cv append_cv 
#exportmember HermiteCurve remove_cv remove_all_cvs
#exportmember HermiteCurve set_cv_type set_cv_point set_cv_in set_cv_out
#exportmember HermiteCurve set_cv_tstart set_cv_name
#exportmember HermiteCurve get_cv_type get_cv_point get_cv_in get_cv_out
#exportmember HermiteCurve get_cv_tstart get_cv_name
#exportmember HermiteCurve Print print_cv
#exportmember HermiteCurve write_egg

#exportsymbol HC_CUT HC_FREE HC_G1 HC_SMOOTH
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////


// Hermite curve continuity types.
#define HC_CUT         1 
// The curve is disconnected at this point.  All points between
// this and the following CV are not part of the curve.

#define HC_FREE        2  
// Tangents are unconstrained.  The curve is continuous, but its first
// derivative is not.  This is G0 geometric continuity.

#define HC_G1          3  
// Tangents are constrained to be collinear.  The curve's derivative
// is not continuous in parametric space, but its geometric slope is.
// The distinction is mainly relevant in the context of animation
// along the curve--when crossing the join point, direction of motion
// will change continuously, but the speed of motion may change
// suddenly.  This is G1 geometric continuity.

#define HC_SMOOTH     4
// Tangents are constrained to be identical.  The curve and its first
// derivative are continuous in parametric space.  When animating
// motion across the join point, speed and direction of motion will
// change continuously.  This is C1 parametric continuity.


#define VecType pfVec3
//typedef pfVec3 VecType;

class NurbsCurve;

////////////////////////////////////////////////////////////////////
// 	 Class : HermiteCurveCV
// Description : A single CV of a Hermite curve.  Hermite curve CV's
//               include an in and out tangent, as well as a position.
////////////////////////////////////////////////////////////////////
class HermiteCurveCV {
public:
  HermiteCurveCV();
  HermiteCurveCV(const HermiteCurveCV &c);
  ~HermiteCurveCV();
  
  void set_point(const VecType &point) { _p = point; }
  void set_in(const VecType &in);
  void set_out(const VecType &out);
  void set_type(int type);
  void set_name(const char *name);

  int store_pfb(void *handle);
  int load_pfb(void *handle);

  void Output(ostream &out, int indent, int num_dimensions,
	      boolean show_in, boolean show_out,
	      double scale_in, double scale_out) const;
  
  VecType _p, _in, _out;
  int _type;
  char *_name;
};

////////////////////////////////////////////////////////////////////
// 	 Class : HermiteCurve
// Description : A parametric curve defined by a sequence of control
//               vertices, each with an in and out tangent.
//
//               This class is actually implemented as a
//               PiecewiseCurve made up of several CubicCurvesegs,
//               each of which is created using the hermite_basis()
//               method.  The HermiteCurve class itself keeps its own
//               list of the CV's that are used to define the curve
//               (since the CubicCurveseg class doesn't retain these).
////////////////////////////////////////////////////////////////////
class HermiteCurve : public PiecewiseCurve {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  HermiteCurve();
  HermiteCurve(const ParametricCurve &pc);

  int get_num_cvs() const;

  int insert_cv(double t);
  int append_cv(int type, float x, float y, float z);
  inline int append_cv(int type, const VecType &v) {
    return append_cv(type, v[0], v[1], v[2]);
  }

  boolean remove_cv(int n);
  void remove_all_cvs();

  boolean set_cv_type(int n, int type);
  boolean set_cv_point(int n, float x, float y, float z);
  inline boolean set_cv_point(int n, const VecType &v) {
    return set_cv_point(n, v[0], v[1], v[2]);
  }
  boolean set_cv_in(int n, float x, float y, float z);
  inline boolean set_cv_in(int n, const VecType &v) {
    return set_cv_in(n, v[0], v[1], v[2]);
  }
  boolean set_cv_out(int n, float x, float y, float z);
  inline boolean set_cv_out(int n, const VecType &v) {
    return set_cv_out(n, v[0], v[1], v[2]);
  }
  boolean set_cv_tstart(int n, double tstart);
  boolean set_cv_name(int n, const char *name);


  int get_cv_type(int n) const;
  const VecType &get_cv_point(int n) const;
  void get_cv_point(int n, VecType &v) const;
  const VecType &get_cv_in(int n) const;
  void get_cv_in(int n, VecType &v) const;
  const VecType &get_cv_out(int n) const;
  void get_cv_out(int n, VecType &v) const;
  double get_cv_tstart(int n) const;
  const char *get_cv_name(int n) const;

  void Print() const;
  void print_cv(int n) const;

  boolean write_egg(const char *filename);
  boolean write_egg(ostream &out, const char *basename);
  
////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:

  CubicCurveseg *get_curveseg(int ti) {
    return (CubicCurveseg *)PiecewiseCurve::get_curveseg(ti);
  }

  virtual boolean
  rebuild_curveseg(int rtype0, double t0, const pfVec4 &v0,
		   int rtype1, double t1, const pfVec4 &v1,
		   int rtype2, double t2, const pfVec4 &v2,
		   int rtype3, double t3, const pfVec4 &v3);

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new HermiteCurve; }
  static int st_descend_pfb(pfNode *node, void *handle) {
    return ((HermiteCurve *)node)->descend_pfb(handle);
  }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((HermiteCurve *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((HermiteCurve *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

  void Output(ostream &out, int indent=0) const;

protected:
  virtual ~HermiteCurve();

  void invalidate_cv(int n, boolean redo_all);
  int find_cv(double t);
  void recompute_basis();

  perf_vector<HermiteCurveCV> _points;

private:
  static pfType *classType;
};

// Wrappers for salivate
HermiteCurve* make_HermiteCurve();
HermiteCurve* make_HermiteCurve(const ParametricCurve &pc);
void rm_HermiteCurve(HermiteCurve *curve);

#endif
