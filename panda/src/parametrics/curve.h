// Filename: curve.h
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
#ifndef CURVE_H
#define CURVE_H

////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include <typedef.h>
#include <plist.h>
#include <pvector.h>
#include <Performer/pr/pfLinMath.h>
#include <Performer/pf/pfGroup.h>
#include <DNotify.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////

/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass ParametricCurve
#exportmember ParametricCurve is_valid get_max_t
#exportmember ParametricCurve set_curve_type get_curve_type
#exportmember ParametricCurve set_num_dimensions get_num_dimensions
#exportmember ParametricCurve calc_length
#exportmember ParametricCurve compute_t
#exportmember ParametricCurve convert_to_hermite
#exportmember ParametricCurve convert_to_nurbs
#exportmember ParametricCurve ascii_draw

#exportclass PiecewiseCurve
#exportmember PiecewiseCurve is_valid get_max_t
#exportmember PiecewiseCurve get_point get_tangent get_pt get_2ndtangent
#exportmember PiecewiseCurve adjust_point adjust_tangent adjust_pt

#exportclass CubicCurveseg
#exportmember CubicCurveseg get_point get_tangent get_pt get_2ndtangent

#exportsymbol PCT_NONE PCT_XYZ PCT_HPR PCT_T
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////

// Parametric curve semantic types.  A parametric curve may have one
// of these types specified.  These serve as hints to the egg reader
// and writer code about the intention of this curve, and have no
// other effect on the curve.

#define PCT_NONE        0
// Unspecified type.

#define PCT_XYZ         1
// A three-dimensional curve in space.

#define PCT_HPR         2
// The curve represents Euler rotation angles.

#define PCT_T           3
// A one-dimensional timewarp curve.


#define VecType pfVec3
//typedef pfVec3 VecType;


// These symbols are used to define the shape of the curve segment to
// CubicCurveseg::compute_seg().

#define RT_POINT       0x01
#define RT_TANGENT     0x02
#define RT_CV          0x03
#define RT_BASE_TYPE   0xff

#define RT_KEEP_ORIG  0x100


class ParametricCurveDrawer;
class HermiteCurveCV;
class HermiteCurve;
class NurbsCurve;


////////////////////////////////////////////////////////////////////
// 	 Class : ParametricCurve
// Description : A virtual base class for parametric curves.
//               This encapsulates all curves in 3-d space defined
//               for a single parameter t in the range [0,get_max_t()].
////////////////////////////////////////////////////////////////////
class ParametricCurve : public pfGroup {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  virtual boolean is_valid() const;

  virtual double get_max_t() const;

  void set_curve_type(int type);
  int get_curve_type() const;

  void set_num_dimensions(int num);
  int get_num_dimensions() const;

  virtual boolean get_point(double t, VecType &point) const=0;
  virtual boolean get_tangent(double t, VecType &tangent) const=0;
  virtual boolean get_pt(double t, VecType &point, VecType &tangent) const=0;
  virtual boolean get_2ndtangent(double t, VecType &tangent2) const=0;

  float calc_length() const;
  float calc_length(double from, double to) const;
  double compute_t(double start_t, double length_offset, double guess, 
		   double threshold) const;

  boolean convert_to_hermite(HermiteCurve &hc) const;
  boolean convert_to_nurbs(NurbsCurve &nc) const;

  void ascii_draw() const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:

  struct BezierSeg {
  public:
    pfVec3 _v[4];
    double _t;
  };
  typedef vector<BezierSeg> BezierSegs;

  ParametricCurve();

  virtual boolean GetBezierSegs(BezierSegs &) const {
    return false;
  }

  virtual boolean GetBezierSeg(BezierSeg &) const {
    return false;
  }

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { dnassert(false); return NULL; }
  static int st_descend_pfb(pfNode *, void *) { return 0; }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((ParametricCurve *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((ParametricCurve *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

  void register_drawer(ParametricCurveDrawer *drawer);
  void unregister_drawer(ParametricCurveDrawer *drawer);

protected:
  virtual ~ParametricCurve();

  void invalidate(double t1, double t2);
  void invalidate_all();

  float r_calc_length(double t1, double t2,
		      const pfVec3 &p1, const pfVec3 &p2,
		      float seglength) const;

  typedef perf_list< ParametricCurveDrawer * > DrawerList;
  DrawerList _drawers;
  int _curve_type;
  int _num_dimensions;
private:
  static pfType *classType;
};


////////////////////////////////////////////////////////////////////
// 	 Class : PiecewiseCurve
// Description : A PiecewiseCurve is a curve made up of several curve
//               segments, connected in a head-to-tail fashion.  The
//               length of each curve segment in parametric space is
//               definable.
////////////////////////////////////////////////////////////////////
class PiecewiseCurve : public ParametricCurve {
public:

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  virtual boolean is_valid() const;
  virtual double get_max_t() const;

  virtual boolean get_point(double t, VecType &point) const;
  virtual boolean get_tangent(double t, VecType &tangent) const;
  virtual boolean get_pt(double t, VecType &point, VecType &tangent) const;
  virtual boolean get_2ndtangent(double t, VecType &tangent2) const;

  boolean adjust_point(double t, 
		       float px, float py, float pz);
  boolean adjust_tangent(double t, 
			 float tx, float ty, float tz);
  boolean adjust_pt(double t, 
		    float px, float py, float pz,
		    float tx, float ty, float tz);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  PiecewiseCurve();

  int get_num_segs() const;

  ParametricCurve *get_curveseg(int ti);
  boolean insert_curveseg(int ti, ParametricCurve *seg, double tlength);

  boolean remove_curveseg(int ti);
  void remove_all_curvesegs();
  
  double get_tlength(int ti) const;
  double get_tstart(int ti) const;
  double get_tend(int ti) const;
  boolean set_tlength(int ti, double tlength);

  void make_nurbs(int order, int num_cvs,
		  const double knots[], const pfVec4 cvs[]);

  virtual boolean GetBezierSegs(BezierSegs &bz_segs) const;

  virtual boolean
  rebuild_curveseg(int rtype0, double t0, const pfVec4 &v0,
		   int rtype1, double t1, const pfVec4 &v1,
		   int rtype2, double t2, const pfVec4 &v2,
		   int rtype3, double t3, const pfVec4 &v3);

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new PiecewiseCurve; }
  static int st_descend_pfb(pfNode *node, void *handle) {
    return ((PiecewiseCurve *)node)->descend_pfb(handle);
  }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((PiecewiseCurve *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((PiecewiseCurve *)node)->load_pfb(handle);
  }

  int descend_pfb(void *handle);
  int store_pfb(void *handle);
  int load_pfb(void *handle);

protected:
  ~PiecewiseCurve();

  boolean find_curve(const ParametricCurve *&curve, double &t) const;
  double current_seg_range(double t) const;

  class Curveseg {
  public:
    Curveseg() {}
    Curveseg(ParametricCurve *c, double t) : _curve(c), _tend(t) {}

    int descend_pfb(void *handle);
    int store_pfb(void *handle);
    int load_pfb(void *handle);

    ParametricCurve *_curve;
    double _tend;
  };

  perf_vector<Curveseg> _segs;
  int _last_ti;

private:
  static pfType *classType;
};


  

////////////////////////////////////////////////////////////////////
// 	 Class : CubicCurveseg
// Description : A CubicCurveseg is any curve that can be completely
//               described by four 4-valued basis vectors, one for
//               each dimension in three-space, and one for the
//               homogeneous coordinate.  This includes Beziers,
//               Hermites, and NURBS.
//
//               This class encapsulates a single curve segment of the
//               cubic curve.  Normally, when we think of Bezier and
//               Hermite curves, we think of a piecewise collection of
//               such segments.
//
//               Although this class includes methods such as
//               hermite_basis() and nurbs_basis(), to generate a
//               Hermite and NURBS curve segment, respectively, only
//               the final basis vectors are stored: the product of
//               the basis matrix of the corresponding curve type, and
//               its geometry vectors.  This is the minimum
//               information needed to evaluate the curve.  However,
//               the individual CV's that were used to compute these
//               basis vectors are not retained; this might be handled
//               in a subclass (for instance, HermiteCurve).
////////////////////////////////////////////////////////////////////
class CubicCurveseg : public ParametricCurve {
public:

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  virtual boolean get_point(double t, VecType &point) const;
  virtual boolean get_tangent(double t, VecType &tangent) const;
  virtual boolean get_pt(double t, VecType &point, VecType &tangent) const;
  virtual boolean get_2ndtangent(double t, VecType &tangent2) const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  CubicCurveseg();
  CubicCurveseg(const pfMatrix &basis);
  CubicCurveseg(const HermiteCurveCV &cv0,
		const HermiteCurveCV &cv1);
  CubicCurveseg(const BezierSeg &seg);
  CubicCurveseg(int order, const double knots[], const pfVec4 cvs[]);

  void hermite_basis(const HermiteCurveCV &cv0,
		     const HermiteCurveCV &cv1,
		     double tlength = 1.0);
  void bezier_basis(const BezierSeg &seg);
  void nurbs_basis(int order, const double knots[], const pfVec4 cvs[]);

  // evaluate_point() and evaluate_vector() both evaluate the curve at
  // a given point by applying the basis vector against the vector 
  // [t3 t2 t 1] (or some derivative).  The difference between the
  // two is that evaluate_point() is called only with the vector
  // [t3 t2 t 1] and computes a point in three-space and will scale by
  // the homogeneous coordinate when the curve demands it (e.g. a
  // NURBS), while evaluate_vector() is called with some derivative
  // vector like [3t2 2t 1 0] and computes a vector difference between
  // points, and will never scale by the homogeneous coordinate (which
  // would be zero anyway).

  void evaluate_point(const pfVec4 &tv, VecType &result) const {
    double h = (rational) ? tv.dot(Bw) : 1.0;
    result.set(tv.dot(Bx) / h,
	       tv.dot(By) / h,
	       tv.dot(Bz) / h);
  }

  void evaluate_vector(const pfVec4 &tv, VecType &result) const {
    result.set(tv.dot(Bx),
	       tv.dot(By),
	       tv.dot(Bz));
  }

  virtual boolean GetBezierSeg(BezierSeg &seg) const;

  static boolean compute_seg(int rtype0, double t0, const pfVec4 &v0,
			     int rtype1, double t1, const pfVec4 &v1,
			     int rtype2, double t2, const pfVec4 &v2,
			     int rtype3, double t3, const pfVec4 &v3,
			     const pfMatrix &B, 
			     const pfMatrix &Bi,
			     pfMatrix &G);

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new CubicCurveseg; }
  static int st_descend_pfb(pfNode *, void *) { return 0; }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((CubicCurveseg *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((CubicCurveseg *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

  pfVec4 Bx, By, Bz, Bw;
  boolean rational;

protected:
  virtual ~CubicCurveseg();

private:
  static pfType *classType;
};

// This function is used internally to build the NURBS basis matrix
// based on a given knot sequence.
void compute_nurbs_basis(int order, 
			 const double knots_in[],
			 pfMatrix &basis);


#endif
