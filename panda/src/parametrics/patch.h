// Filename: patch.h
// Created by:  drose (17Apr97)
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
#ifndef PATCH_H
#define PATCH_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curve.h"

#include <typedef.h>
#include <vector.h>
#include <Performer/pr/pfLinMath.h>
#include <Performer/pf/pfGroup.h>
#include <DNotify.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass ParametricSurface
#exportmember ParametricSurface is_valid get_max_s get_max_t
#exportmember ParametricSurface calc_s_length calc_t_length
#exportmember ParametricSurface Print

#exportclass SIsoCurve
#exportfunc make_SIsoCurve rm_SIsoCurve
#exportmember SIsoCurve is_valid get_max_t
#exportmember SIsoCurve get_point get_tangent get_pt get_2ndtangent

#exportclass TIsoCurve
#exportfunc make_TIsoCurve rm_TIsoCurve
#exportmember TIsoCurve is_valid get_max_t
#exportmember TIsoCurve get_point get_tangent get_pt get_2ndtangent

#exportclass Quilt
#exportmember Quilt is_valid
#exportmember Quilt get_num_s get_num_t
#exportmember Quilt get_max_s get_max_t
#exportmember Quilt set_max_s set_max_t
#exportmember Quilt get_point get_s_tan get_t_tan get_normal
#exportmember Quilt get_pn

#exportclass BicubicPatch
#exportmember BicubicPatch get_point get_s_tan get_t_tan get_normal
#exportmember BicubicPatch get_pn
$*/

////////////////////////////////////////////////////////////////////
// Defines 
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 	 Class : ParametricSurface
// Description : A virtual base class for parametric surfaces or
//               patches.  This encapsulates all surfaces in 3-d space
//               defined for two parameters s,t in the range
//               [0,get_max_s()] and [0,get_max_t()].
////////////////////////////////////////////////////////////////////
class ParametricSurface : public pfGroup {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  virtual boolean is_valid() const;

  virtual double get_max_s() const;
  virtual double get_max_t() const;

  virtual boolean get_point(double s, double t, VecType &point) const=0;
  virtual boolean get_s_tan(double s, double t, VecType &tangent) const=0;
  virtual boolean get_t_tan(double s, double t, VecType &tangent) const=0;
  virtual boolean get_normal(double s, double t, VecType &normal) const=0;
  virtual boolean get_pn(double s, double t, 
			 VecType &point, VecType &normal) const=0;

  float calc_s_length(double t) const;
  float calc_t_length(double s) const;

  void Print() const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  struct BezierPatch {
  public:
    pfVec3 _v[4][4];
    double _s, _t;
  };
  typedef vector<vector<BezierPatch> > BezierPatches;

  ParametricSurface();

  virtual boolean GetBezierPatches(BezierPatches &) const {
    return false;
  }

  virtual boolean GetBezierPatch(BezierPatch &) const {
    return false;
  }

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { dnassert(false); return NULL; }
  static int st_descend_pfb(pfNode *, void *) { return 0; }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((ParametricSurface *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((ParametricSurface *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

protected:
  virtual ~ParametricSurface();

private:
  static pfType *classType;
};


////////////////////////////////////////////////////////////////////
// 	 Class : SIsoCurve
// Description : An isoparm of the surface defined as all the points
//               for a specified fixed value of S.
////////////////////////////////////////////////////////////////////
class SIsoCurve : public ParametricCurve {
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

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  SIsoCurve(const ParametricSurface *surface, double s) :
    _surface(surface), _s(s) 
  {
    setType(classType);
  }

  virtual boolean GetBezierSegs(BezierSegs &bz_segs) const;

  static void init();
  static pfType *getClassType() { return classType; }

protected:

  const ParametricSurface *_surface;
  double _s;

private:
  static pfType *classType;
};
SIsoCurve *make_SIsoCurve(ParametricSurface *surface, double s);
void rm_SIsoCurve(SIsoCurve *curve);


////////////////////////////////////////////////////////////////////
// 	 Class : TIsoCurve
// Description : An isoparm of the surface defined as all the points
//               for a specified fixed value of S.
////////////////////////////////////////////////////////////////////
class TIsoCurve : public ParametricCurve {
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

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  TIsoCurve(const ParametricSurface *surface, double t) :
    _surface(surface), _t(t)
  {
    setType(classType);
  }

  virtual boolean GetBezierSegs(BezierSegs &bz_segs) const;

  static void init();
  static pfType *getClassType() { return classType; }

protected:

  const ParametricSurface *_surface;
  double _t;

private:
  static pfType *classType;
};
TIsoCurve *make_TIsoCurve(ParametricSurface *surface, double t);
void rm_TIsoCurve(TIsoCurve *curve);


////////////////////////////////////////////////////////////////////
// 	 Class : Quilt
// Description : A quilt is a 2-d array of patches.
////////////////////////////////////////////////////////////////////
class Quilt : public ParametricSurface {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  virtual boolean is_valid() const;

  int get_num_s() const;
  int get_num_t() const;

  virtual double get_max_s() const;
  virtual double get_max_t() const;

  void set_max_s(double s);
  void set_max_t(double t);

  virtual boolean get_point(double s, double t, VecType &point) const;
  virtual boolean get_s_tan(double s, double t, VecType &tangent) const;
  virtual boolean get_t_tan(double s, double t, VecType &tangent) const;
  virtual boolean get_normal(double s, double t, VecType &normal) const;
  virtual boolean get_pn(double s, double t, 
			 VecType &point, VecType &normal) const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  Quilt();
  Quilt(int num_s, int num_t);
  
  void reset_quilt(int num_s, int num_t);

  void set_patch(int si, int ti, ParametricSurface *patch);
  ParametricSurface *get_patch(int si, int ti);
  
  double get_slength(int si) const;
  boolean set_slength(int si, double tlength);
  
  double get_tlength(int ti) const;
  boolean set_tlength(int ti, double tlength);

  void make_nurbs(int s_order, int t_order,
		  int num_s_cvs, int num_t_cvs,
		  const double s_knots[], const double t_tknots[],
		  const pfVec4 cvs[]);

  virtual boolean GetBezierPatches(BezierPatches &bz_patches) const;

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new Quilt; }
  static int st_descend_pfb(pfNode *node, void *handle) {
    return ((Quilt *)node)->descend_pfb(handle);
  }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((Quilt *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((Quilt *)node)->load_pfb(handle);
  }

  int descend_pfb(void *handle);
  int store_pfb(void *handle);
  int load_pfb(void *handle);

protected:
  virtual ~Quilt();

  const ParametricSurface *find_patch(double &s, double &t) const;

  ParametricSurface **_patches;   // An array of (ParametricSurface *).
  int _num_s, _num_t;

  double *_sends, *_tends;        // Two arrays of doubles.
  int _last_si, _last_ti;

private:
  static pfType *classType;
};



////////////////////////////////////////////////////////////////////
// 	 Class : HermitePatchCV
// Description :
////////////////////////////////////////////////////////////////////
class HermitePatchCV {
public:
  HermitePatchCV();
  HermitePatchCV(const HermitePatchCV &c);
  
  VecType _p, _s_in, _s_out, _t_in, _t_out, _twist;
};



////////////////////////////////////////////////////////////////////
// 	 Class : BicubicPatch
// Description : A BicubicPatch is any patch that can be completely
//               described by four 4x4 basis matrices, one for each
//               dimension plus the homogeneous coordinate.  This
//               includes Beziers, Hermites, and NURBS.
////////////////////////////////////////////////////////////////////
class BicubicPatch : public ParametricSurface {
public:

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  virtual boolean get_point(double s, double t, VecType &point) const;
  virtual boolean get_s_tan(double s, double t, VecType &tangent) const;
  virtual boolean get_t_tan(double s, double t, VecType &tangent) const;
  virtual boolean get_normal(double s, double t, VecType &normal) const;
  virtual boolean get_pn(double s, double t, 
			 VecType &point, VecType &normal) const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  BicubicPatch();
  BicubicPatch(const HermitePatchCV &cv00,
	       const HermitePatchCV &cv10,
	       const HermitePatchCV &cv01,
	       const HermitePatchCV &cv11);
  BicubicPatch(const pfMatrix &Gx, 
	       const pfMatrix &Gy,
	       const pfMatrix &Gz);
  BicubicPatch(int s_order, int t_order,
	       const double s_knots[], const double t_knots[],
	       const pfVec4 cvs[], 
	       int si_scale, int si_offset, int ti_offset);

  void hermite_basis(const HermitePatchCV &cv00,
		     const HermitePatchCV &cv10,
		     const HermitePatchCV &cv01,
		     const HermitePatchCV &cv11);

  void bezier_basis(const pfMatrix &Gx, 
		    const pfMatrix &Gy,
		    const pfMatrix &Gz);

  void nurbs_basis(int s_order, int t_order,
		   const double s_knots[], const double t_knots[],
		   const pfVec4 cvs[], 
		   int si_scale, int si_offset, int ti_offset);

  // See curve.h for a comment on the meaning and difference between
  // evaluate_point() and evaluate_vector().

  void evaluate_point(const pfVec4 &sv, const pfVec4 &tv,
		      VecType &result) const {
    double h = (rational) ? tv.dot(sv * Bw) : 1.0;
    result.set(tv.dot(sv * Bx) / h,
	       tv.dot(sv * By) / h,
	       tv.dot(sv * Bz) / h);
  }

  void evaluate_vector(const pfVec4 &sv, const pfVec4 &tv,
		       VecType &result) const {
    result.set(tv.dot(sv * Bx),
	       tv.dot(sv * By),
	       tv.dot(sv * Bz));
  }

  virtual boolean GetBezierPatch(BezierPatch &patch) const;

  static void init();
  static pfType *getClassType() { return classType; }
  
  static pfNode *st_new_pfb() { return new BicubicPatch; }
  static int st_descend_pfb(pfNode *, void *) { return 0; }
  static int st_store_pfb(pfNode *node, void *handle) {
    return ((BicubicPatch *)node)->store_pfb(handle);
  }
  static int st_load_pfb(pfNode *node, void *handle) {
    return ((BicubicPatch *)node)->load_pfb(handle);
  }

  int store_pfb(void *handle);
  int load_pfb(void *handle);

protected:
  virtual ~BicubicPatch();

  pfMatrix Bx, By, Bz, Bw;
  boolean rational;

private:
  static pfType *classType;
};

#endif
