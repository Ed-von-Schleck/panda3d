// Filename: patch.C
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
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////

#include "parametrics.h"
#include "patch.h"
#include <initReg.h>
#include <math.h>
#include <fstream.h>
#include <alloca.h>
#include <Performer/pf/pfBuffer.h>
#include <linMathOutput.h>
#include <DConfig.h>
#include <perfalloc.h>

////////////////////////////////////////////////////////////////////
// Prototypes 
////////////////////////////////////////////////////////////////////


// This is declared in curve.C.
void
compute_nurbs_basis(int order, 
		    const double knots_in[],
		    LMatrix4f &basis);


////////////////////////////////////////////////////////////////////
// Statics
////////////////////////////////////////////////////////////////////


static const VecType zero(0.0, 0.0, 0.0);
// This is returned occasionally from some of the functions, and is
// used from time to time as an initializer.




////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::Constructor
//       Access: Public
//  Description: This is a virtual base class.  Don't try to construct
//               one from Scheme.
////////////////////////////////////////////////////////////////////
ParametricSurface::
ParametricSurface() {
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::is_valid
//       Access: Public, Scheme, Virtual
//  Description: Returns true if the surface is defined.  The default
//               here is always true; a derived class may override
//               this function to occasionally return false.
////////////////////////////////////////////////////////////////////
bool ParametricSurface::
is_valid() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::get_max_s
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of s for the entire surface.
//               The surface is defined in the range 0.0 <= s <=
//               get_max_s().  This base class function always returns
//               1.0; derived classes might override this to return
//               something else.
////////////////////////////////////////////////////////////////////
double ParametricSurface::
get_max_s() const {
  return 1.0;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::get_max_t
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of t for the entire surface.
//               The surface is defined in the range 0.0 <= t <=
//               get_max_t().  This base class function always returns
//               1.0; derived classes might override this to return
//               something else.
////////////////////////////////////////////////////////////////////
double ParametricSurface::
get_max_t() const {
  return 1.0;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::calc_s_length
//       Access: Public, Scheme
//  Description: Approximates the length of an isoparm of the surface
//               to within a few decimal places.  t should be in the
//               range 0 <= t <= get_max_t().  This is a very
//               expensive calculation.
////////////////////////////////////////////////////////////////////
float ParametricSurface::
calc_s_length(double t) const {
  TIsoCurve *curve = new TIsoCurve(this, t);
  float length = curve->calc_length();
  pfDelete(curve);
  return length;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::calc_t_length
//       Access: Public, Scheme
//  Description: Approximates the length of an isoparm of the surface
//               to within a few decimal places.  s should be in the
//               range 0 <= s <= get_max_s().  This is a very
//               expensive calculation.
////////////////////////////////////////////////////////////////////
float ParametricSurface::
calc_t_length(double s) const {
  SIsoCurve *curve = new SIsoCurve(this, s);
  float length = curve->calc_length();
  pfDelete(curve);
  return length;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::Print
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
void ParametricSurface::
Print() const {
  if (!is_valid()) {
    DINFO(dnparametrics) 
      << "Incomplete surface." << dnend;
  } else {
    LVector3f c00, c01, c10, c11;
    get_point(0.0, 0.0, c00);
    get_point(0.0, 1.0, c01);
    get_point(1.0, 0.0, c10);
    get_point(1.0, 1.0, c11);
    
    DINFO(dnparametrics)
      << "Surface corners at:\n"
      << c00 << "\n" << c01 << "\n" << c10 << "\n" << c11 
      << dnend;
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurface::Destructor
//       Access: Protected, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurface::
~ParametricSurface() {
}


SIsoCurve *make_SIsoCurve(ParametricSurface *surface, double s) {
  return new SIsoCurve(surface, s);
}

void rm_SIsoCurve(SIsoCurve *curve) {
  pfDelete(curve);
}

////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::is_valid
//       Access: Public, Scheme, Virtual
//  Description: Returns true if the curve is defined.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
is_valid() const {
  return _surface->is_valid() && _s >= 0.0 && _s <= _surface->get_max_s();
}

////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::get_max_t
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of t for the entire curve.
//               The curve is defined in the range 0.0 <= t <=
//               get_max_t().
////////////////////////////////////////////////////////////////////
double SIsoCurve::
get_max_t() const {
  return _surface->get_max_t();
}


////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::get_point
//       Access: Public, Scheme, Virtual
//  Description: Returns the point of the curve at a given parametric
//               point t.  Returns true if t is in the valid range 0.0
//               <= t <= get_max_t(); if t is outside this range, sets
//               point to the value of the curve at the beginning or
//               end (whichever is nearer) and returns false.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
get_point(double t, VecType &point) const {
  return _surface->get_point(_s, t, point);
}


////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::get_tangent
//       Access: Public, Scheme, Virtual
//  Description: Returns the tangent of the curve at a given parametric
//               point t.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
get_tangent(double t, VecType &tangent) const {
  return _surface->get_s_tan(_s, t, tangent);
}


////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::get_2ndtangent
//       Access: Public, Scheme, Virtual
//  Description: Returns the tangent of the first derivative of the
//               curve at the point t.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
get_2ndtangent(double t, VecType &tangent2) const {
  // Not implemented.  Does anyone care?
  return false;
}


////////////////////////////////////////////////////////////////////
//     Function: SIsoCurve::get_pt
//       Access: Public, Scheme, Virtual
//  Description: Simultaneously returns the point and tangent of the
//               curve at a given parametric point t.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
get_pt(double t, VecType &point, VecType &tangent) const {
  return get_point(t, point) && get_tangent(t, tangent);
}

////////////////////////////////////////////////////////////////////
//     Function: SIsoCurveCurve::GetBezierSegs
//       Access: Public, Virtual
//  Description: Fills up the indicated vector with a list of
//               BezierSeg structs that describe the curve.  Returns
//               true if successful, false otherwise.
////////////////////////////////////////////////////////////////////
bool SIsoCurve::
GetBezierSegs(BezierSegs &bz_segs) const {
  if (!is_valid()) {
    return false;
  }
  // Get all the Bezier patches in the surface.
  ParametricSurface::BezierPatches patches;
  _surface->GetBezierPatches(patches);
  
  int num_sp = patches.size();
  int num_tp = (num_sp>0) ? patches[0].size() : 0;

  int si, ti;
  // Which row in the patches do we care about?
  for (si = 0; si < num_sp; si++) {
    if (patches[si][0]._s + 0.00001 > _s) {
      break;
    }
  }

  // Adjust s to the range [0,1].
  double s = _s;
  if (si >= num_sp) {
    si = num_sp-1;
  }
  if (si>0) {
    s = (s - patches[si-1][0]._s) / (patches[si][0]._s - patches[si-1][0]._s);
  } else {
    si = 0;
    s /= patches[0][0]._s;
  }

  // Now walk along the row.  At each point, we can compute the
  // corresponding Bezier control point in our own curve by
  // constructing a temporary Bezier curve segment in the
  // perpendicular direction and evaluating it.
  int i, j;
  BezierSeg seg1, seg2;
  for (ti = 0; ti < num_tp; ti++) {
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
	seg1._v[j] = patches[si][ti]._v[j][i];
      }
      CubicCurveseg *cseg = new CubicCurveseg(seg1);
      cseg->get_point(s, seg2._v[i]);
      pfDelete(cseg);
    }
    seg2._t = patches[si][ti]._t;
    bz_segs.push_back(seg2);
  }

  return true;
}


TIsoCurve *make_TIsoCurve(ParametricSurface *surface, double t) {
  return new TIsoCurve(surface, t);
}

void rm_TIsoCurve(TIsoCurve *curve) {
  pfDelete(curve);
}

////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::is_valid
//       Access: Public, Scheme, Virtual
//  Description: Returns true if the curve is defined.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
is_valid() const {
  return _surface->is_valid() && _t >= 0.0 && _t <= _surface->get_max_t();
}

////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::get_max_t
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of t for the entire curve.
//               The curve is defined in the range 0.0 <= t <=
//               get_max_t().
////////////////////////////////////////////////////////////////////
double TIsoCurve::
get_max_t() const {
  return _surface->get_max_s();
}


////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::get_point
//       Access: Public, Scheme, Virtual
//  Description: Returns the point of the curve at a given parametric
//               point t.  Returns true if t is in the valid range 0.0
//               <= t <= get_max_t(); if t is outside this range, sets
//               point to the value of the curve at the beginning or
//               end (whichever is nearer) and returns false.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
get_point(double t, VecType &point) const {
  return _surface->get_point(t, _t, point);
}


////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::get_tangent
//       Access: Public, Scheme, Virtual
//  Description: Returns the tangent of the curve at a given parametric
//               point t.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
get_tangent(double t, VecType &tangent) const {
  return _surface->get_t_tan(t, _t, tangent);
}


////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::get_2ndtangent
//       Access: Public, Scheme, Virtual
//  Description: Returns the tangent of the first derivative of the
//               curve at the point t.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
get_2ndtangent(double t, VecType &tangent2) const {
  // Not implemented.  Does anyone care?
  return false;
}


////////////////////////////////////////////////////////////////////
//     Function: TIsoCurve::get_pt
//       Access: Public, Scheme, Virtual
//  Description: Simultaneously returns the point and tangent of the
//               curve at a given parametric point t.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
get_pt(double t, VecType &point, VecType &tangent) const {
  return get_point(t, point) && get_tangent(t, tangent);
}

////////////////////////////////////////////////////////////////////
//     Function: TIsoCurveCurve::GetBezierSegs
//       Access: Public, Virtual
//  Description: Fills up the indicated vector with a list of
//               BezierSeg structs that describe the curve.  Returns
//               true if successful, false otherwise.
////////////////////////////////////////////////////////////////////
bool TIsoCurve::
GetBezierSegs(BezierSegs &bz_segs) const {
  if (!is_valid()) {
    return false;
  }
  // Get all the Bezier patches in the surface.
  ParametricSurface::BezierPatches patches;
  _surface->GetBezierPatches(patches);
  
  int num_sp = patches.size();
  int num_tp = (num_sp>0) ? patches[0].size() : 0;

  int si, ti;
  // Which column in the patches do we care about?
  for (ti = 0; ti < num_tp; ti++) {
    if (patches[0][ti]._t + 0.00001 > _t) {
      break;
    }
  }

  // Adjust t to the range [0,1].
  double t = _t;
  if (ti >= num_tp) {
    ti = num_tp-1;
  }
  if (ti>0) {
    t = (t - patches[0][ti-1]._t) / (patches[0][ti]._t - patches[0][ti-1]._t);
  } else {
    ti = 0;
    t /= patches[0][0]._t;
  }

  // Now walk along the column.  At each point, we can compute the
  // corresponding Bezier control point in our own curve by
  // constructing a temporary Bezier curve segment in the
  // perpendicular direction and evaluating it.
  int i, j;
  BezierSeg seg1, seg2;
  for (si = 0; si < num_sp; si++) {
    for (i = 0; i < 4; i++) {
      for (j = 0; j < 4; j++) {
	seg1._v[j] = patches[si][ti]._v[i][j];
      }
      CubicCurveseg *cseg = new CubicCurveseg(seg1);
      cseg->get_point(t, seg2._v[i]);
      pfDelete(cseg);
    }
    seg2._t = patches[si][ti]._s;
    bz_segs.push_back(seg2);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::Constructor
//       Access: Public
//  Description: Sets up an empty quilt.
////////////////////////////////////////////////////////////////////
Quilt::
Quilt() {
  _patches = NULL;
  _num_s = _num_t = 0;

  _last_si = 0;
  _last_ti = 0;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::Constructor
//       Access: Public
//  Description: Sets up a quilt of the specified number of patches.
//               Once created, a quilt may not presently be resized;
//               it may only be recreated via reset_quilt().
////////////////////////////////////////////////////////////////////
Quilt::
Quilt(int num_s, int num_t) {
  _patches = NULL;
  reset_quilt(num_s, num_t);
}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::is_valid
//       Access: Public, Scheme, Virtual
//  Description: Returns true if the surface is defined.  In the case
//               of a Quilt, this means we have at least one patch.
////////////////////////////////////////////////////////////////////
bool Quilt::
is_valid() const {
  return (_num_s>0 && _num_t>0);
}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_num_s
//       Access: Public, Scheme
//  Description: Returns the number of patches in the s direction.
//               This does not necessarily have anything to do with
//               the parametric length.
////////////////////////////////////////////////////////////////////
int Quilt::
get_num_s() const {
  return _num_s;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_num_t
//       Access: Public, Scheme
//  Description: Returns the number of patches in the t direction.
//               This does not necessarily have anything to do with
//               the parametric length.
////////////////////////////////////////////////////////////////////
int Quilt::
get_num_t() const {
  return _num_t;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_max_s
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of s for the entire surface.
////////////////////////////////////////////////////////////////////
double Quilt::
get_max_s() const {
  return _num_s==0 ? 0.0 : _sends[_num_s-1];
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_max_t
//       Access: Public, Scheme, Virtual
//  Description: Returns the upper bound of t for the entire surface.
////////////////////////////////////////////////////////////////////
double Quilt::
get_max_t() const {
  return _num_t==0 ? 0.0 : _tends[_num_t-1];
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::set_max_s
//       Access: Public, Scheme, Virtual
//  Description: Scales the surface's parametric coordinates so that
//               the upper bound in s is as specified.
////////////////////////////////////////////////////////////////////
void Quilt::
set_max_s(double s) {
  if (_num_s>0) {
    double scale = s / get_max_s();
    for (int i = 0; i < _num_s; i++) {
      _sends[i] *= scale;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::set_max_t
//       Access: Public, Scheme, Virtual
//  Description: Scales the surface's parametric coordinates so that
//               the upper bound in t is as specified.
////////////////////////////////////////////////////////////////////
void Quilt::
set_max_t(double t) {
  if (_num_t>0) {
    double scale = t / get_max_t();
    for (int i = 0; i < _num_t; i++) {
      _tends[i] *= scale;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_point
//       Access: Public, Scheme, Virtual
//  Description: Computes the surface point at a given parametric
//               point s,t.  
////////////////////////////////////////////////////////////////////
bool Quilt::
get_point(double s, double t, VecType &point) const {
  const ParametricSurface *patch = find_patch(s, t);
  return (patch==NULL) ? false : patch->get_point(s, t, point);
}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_s_tan
//       Access: Public, Scheme, Virtual
//  Description: Computes the tangent to the surface in the s
//               direction at a given parametric point s,t.
////////////////////////////////////////////////////////////////////
bool Quilt::
get_s_tan(double s, double t, VecType &tangent) const {
  const ParametricSurface *patch = find_patch(s, t);
  return (patch==NULL) ? false : patch->get_s_tan(s, t, tangent);
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_t_tan
//       Access: Public, Scheme, Virtual
//  Description: Computes the tangent to the surface in the t
//               direction at a given parametric point s,t.
////////////////////////////////////////////////////////////////////
bool Quilt::
get_t_tan(double s, double t, VecType &tangent) const {
  const ParametricSurface *patch = find_patch(s, t);
  return (patch==NULL) ? false : patch->get_t_tan(s, t, tangent);
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_normal
//       Access: Public, Scheme, Virtual
//  Description: Computes the surface normal at a given parametric
//               point s,t.  
////////////////////////////////////////////////////////////////////
bool Quilt::
get_normal(double s, double t, VecType &normal) const {
  const ParametricSurface *patch = find_patch(s, t);
  return (patch==NULL) ? false : patch->get_normal(s, t, normal);
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_pn
//       Access: Public, Scheme, Virtual
//  Description: Computes the point and the normal of the patch at a
//               given parametric point s,t.  
////////////////////////////////////////////////////////////////////
bool Quilt::
get_pn(double s, double t, VecType &point, VecType &normal) const {
  const ParametricSurface *patch = find_patch(s, t);
  return (patch==NULL) ? false : patch->get_pn(s, t, point, normal);
}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::reset_quilt
//       Access: Public
//  Description: Recreates the quilt at the given size.  All patches
//               in the quilt are deleted and a new quilt is created.
////////////////////////////////////////////////////////////////////
void Quilt::
reset_quilt(int num_s, int num_t) {

  if (_patches!=NULL) {
    // First, free all the old patches.
    for (int i = 0; i < _num_s * _num_t; i++) {
      if (_patches[i]!=NULL) {
	pfDelete(_patches[i]);
      }
    }
    perf_deallocate(_patches, -1);
    perf_deallocate(_sends, -1);
    perf_deallocate(_tends, -1);
    _patches = NULL;
    _sends = NULL;
    _tends = NULL;
  }

  _num_s = num_s;
  _num_t = num_t;
  if (_num_s > 0 && _num_t > 0) {
    _patches = (ParametricSurface **)perf_allocate(_num_s * _num_t * sizeof(ParametricSurface *), _patches);
    memset(_patches, 0, sizeof(ParametricSurface *) * _num_s * _num_t);
    _sends = (double *)perf_allocate(_num_s * sizeof(double), _sends);
    _tends = (double *)perf_allocate(_num_t * sizeof(double), _tends);

    // Now fill up the sends and tends arrays so the quilt's patches
    // are evenly distributed over [0, 1].
    int i;
    for (i = 0; i<_num_s; i++) {
      _sends[i] = ((double)(i+1)) / (double)_num_s;
    }
    for (i = 0; i<_num_t; i++) {
      _tends[i] = ((double)(i+1)) / (double)_num_t;
    }
  }

  _last_si = 0;
  _last_ti = 0;
}





////////////////////////////////////////////////////////////////////
//     Function: Quilt::set_patch
//       Access: Public
//  Description: Sets the patch corresponding to the given s, t index.
////////////////////////////////////////////////////////////////////
void Quilt::
set_patch(int si, int ti, ParametricSurface *patch) {
  dnassert(si>=0 && si<_num_s);
  dnassert(ti>=0 && ti<_num_t);

  _patches[si*_num_t + ti] = patch;
}



////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_patch
//       Access: Public
//  Description: Returns the patch corresponding to the given s, t
//               index, or NULL.
////////////////////////////////////////////////////////////////////
ParametricSurface *Quilt::
get_patch(int si, int ti) {
  return (si<0 || si>=_num_s || ti<0 || ti>=_num_t) ? NULL
    : _patches[si*_num_t + ti];

}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_tlength
//       Access: Public
//  Description: Returns the parametric width in t of the given
//               segment of the surface.
////////////////////////////////////////////////////////////////////
double Quilt::
get_tlength(int ti) const {
  dnassert(ti>=0 && ti<_num_t);
  return (ti==0) ? _tends[ti] : _tends[ti] - _tends[ti-1];
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::set_tlength
//       Access: Public
//  Description: Sets the parametric width in t of the given segment
//               of the surface.  Later segments are moved
//               accordingly.
////////////////////////////////////////////////////////////////////
bool Quilt::
set_tlength(int ti, double tlength) {
  if (ti<0 || ti>=_num_t) {
    return false;
  }

  double diff = tlength - get_tlength(ti);
  while (ti < _num_t) {
    _tends[ti] += diff;
    ti++;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::get_slength
//       Access: Public
//  Description: Returns the parametric width in t of the given
//               segment of the surface.
////////////////////////////////////////////////////////////////////
double Quilt::
get_slength(int si) const {
  dnassert(si>=0 && si<_num_s);
  return (si==0) ? _sends[si] : _sends[si] - _sends[si-1];
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::set_slength
//       Access: Public
//  Description: Sets the parametric width in t of the given segment
//               of the surface.  Later segments are moved
//               accordingly.
////////////////////////////////////////////////////////////////////
bool Quilt::
set_slength(int si, double tlength) {
  if (si<0 || si>=_num_s) {
    return false;
  }

  double diff = tlength - get_slength(si);
  while (si < _num_s) {
    _sends[si] += diff;
    si++;
  }
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::make_nurbs
//       Access: Public
//  Description: Recreates the quilt as a NURBS surface.  cvs is an
//               array of num_s_cvs * num_t_cvs, with the s index
//               changing faster.
////////////////////////////////////////////////////////////////////
void Quilt::
make_nurbs(int s_order, int t_order,
	   int num_s_cvs, int num_t_cvs,
	   const double s_knots[], const double t_knots[],
	   const pfVec4 cvs[]) {
  int sn, tn, si, ti;

  int num_s = 0;
  int num_t = 0;

  // Count up the number of patches in each direction we'll need.
  for (sn=0; sn<num_s_cvs - s_order + 1; sn++) {
    if (s_knots[sn+s_order] > s_knots[sn+s_order-1]) {
      num_s++;
    }
  }
  for (tn=0; tn<num_t_cvs - t_order + 1; tn++) {
    if (t_knots[tn+t_order] > t_knots[tn+t_order-1]) {
      num_t++;
    }
  }

  reset_quilt(num_s, num_t);

  // Now go through and make all the patches.
  si = 0;
  for (sn=0; sn<num_s_cvs - s_order + 1; sn++) {
    if (s_knots[sn+s_order] > s_knots[sn+s_order-1]) {
      set_slength(si, s_knots[sn+s_order] - s_knots[sn+s_order-1]);
      ti = 0;
      for (tn=0; tn<num_t_cvs - t_order + 1; tn++) {
	if (t_knots[tn+t_order] > t_knots[tn+t_order-1]) {
	  set_tlength(ti, t_knots[tn+t_order] - t_knots[tn+t_order-1]);
	  set_patch(si, ti, 
		    new BicubicPatch(s_order, t_order,
				     s_knots+sn, t_knots+tn,
				     cvs,
				     num_s_cvs, sn, tn));
	  ti++;
	}
      }
      dnassert(ti==num_t);
      si++;
    }
  }
  dnassert(si==num_s);
}


////////////////////////////////////////////////////////////////////
//     Function: Quilt::GetBezierSegs
//       Access: Public, Virtual
//  Description: Fills up the patches 2-d vector with the a
//               representation of the surface as a set of Bezier
//               patches, if possible.  Returns true if successful,
//               false otherwise.
////////////////////////////////////////////////////////////////////
bool Quilt::
GetBezierPatches(BezierPatches &bz_patches) const {
  bz_patches.erase(bz_patches.begin(), bz_patches.end());
  int si, ti, n;
  pfVec4 Gx, Gy, Gz, Gw;
  BezierPatch patch;
  for (si = 0; si < _num_s; si++) {
    bz_patches.push_back(vector<BezierPatch>());
    vector<BezierPatch> &row = bz_patches.back();
    for (ti = 0; ti < _num_t; ti++) {
      ParametricSurface *p = _patches[si*_num_t + ti];
      if (p==NULL || !p->GetBezierPatch(patch)) {
	return false;
      }
      patch._s = _sends[si];
      patch._t = _tends[ti];
      row.push_back(patch);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::Destructor
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
Quilt::
~Quilt() {
  reset_quilt(0, 0);
}

////////////////////////////////////////////////////////////////////
//     Function: Quilt::find_patch
//       Access: Protected
//  Description: Finds the patch corresponding to the given values of
//               s and t.  It returns either a pointer to the patch or
//               NULL, if no patch was defined there or s and t are
//               out of range.
//
//               If a patch is found, s and t are translated to [0,1]
//               to index into the patch's coordinate system
////////////////////////////////////////////////////////////////////
const ParametricSurface *Quilt::
find_patch(double &s, double &t) const {
  // Check the index computed by the last call to find_curve().  If
  // it's still a reasonable starting value, start searching from
  // there.  This way, we take advantage of locality of reference: the
  // search is trivial it is the same segment as last time, or the
  // next segment after the last one.
  if (_last_si>0 && _sends[_last_si-1]>=s) {
    // However, if the new t value precedes that of last time, we'll
    // have to start over.

    // We do some messy casting so we can get away with assigning a
    // value to a member within a const function.  This assignment
    // doesn't really count as a const violation since we're just
    // updating a cached value, not changing any real data of the
    // class.
    ((Quilt *)this)->_last_si = 0;
  }
  if (_last_ti>0 && _tends[_last_ti-1]>=t) {
    // Ditto in the t direction.
    ((Quilt *)this)->_last_ti = 0;
  }

  int si, ti;
  for (si = _last_si; si<_num_s; si++) {
    if (_sends[si]+0.00001 > s) {
      break;
    }
  }
  for (ti = _last_ti; ti<_num_t; ti++) {
    if (_tends[ti]+0.00001 > t) {
      break;
    }
  }

  // Adjust s and t to the range [0,1).
  if (si>0) {
    s = (s - _sends[si-1]) / (_sends[si] - _sends[si-1]);
  } else {
    s /= _sends[0];
  }
  if (ti>0) {
    t = (t - _tends[ti-1]) / (_tends[ti] - _tends[ti-1]);
  } else {
    t /= _tends[0];
  }

  if (s < 0 || t < 0) {
    // Oops.
    return NULL;
  }

  if (si>=_num_s || ti>=_num_t || _patches[si*_num_t + ti]==NULL) {
    // If we're out of bounds, or the patch is undefined, we're probably
    // screwed.  There's one exception: if we were right on a border between
    // patches, try the patch before.

    if (si>0 && s==0.0) {
      si--;
      s = 1.0;
    }
    if (ti>0 && t==0.0) {
      ti--;
      t = 1.0;
    }
    
    if (si>=_num_s || ti>=_num_t || _patches[si*_num_t + ti]==NULL) {
      return NULL;
    }
  }

  // Again, some messy casting so we can get away with updating the
  // cached index value for next time.
  ((Quilt *)this)->_last_si = si;
  ((Quilt *)this)->_last_ti = ti;

  // Now scale s and t back into the patch's own valid range.
  ParametricSurface *patch = _patches[si*_num_t + ti];
  s *= patch->get_max_s();
  t *= patch->get_max_t();

  return patch;
}



////////////////////////////////////////////////////////////////////
//     Function: HermitePatchCV::constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
HermitePatchCV::
HermitePatchCV() {
}


////////////////////////////////////////////////////////////////////
//     Function: HermitePatchCV::copy_constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
HermitePatchCV::
HermitePatchCV(const HermitePatchCV &c) :
  _p(c._p), _s_in(c._s_in), _s_out(c._s_out), 
  _t_in(c._t_in), _t_out(c._t_out), 
  _twist(c._twist) {
}





////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
BicubicPatch::
BicubicPatch() {
}


////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::Constructor
//       Access: Public
//  Description: Constructs a Hermite patch.
////////////////////////////////////////////////////////////////////
BicubicPatch::
BicubicPatch(const HermitePatchCV &cv00,
	     const HermitePatchCV &cv10,
	     const HermitePatchCV &cv01,
	     const HermitePatchCV &cv11) {
  hermite_basis(cv00, cv10, cv01, cv11);
}


////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::Constructor
//       Access: Public
//  Description: Constructs a Bezier patch.
////////////////////////////////////////////////////////////////////
BicubicPatch::
BicubicPatch(const LMatrix4f &Gx, 
	     const LMatrix4f &Gy,
	     const LMatrix4f &Gz) {
  bezier_basis(Gx, Gy, Gz);
}

////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::Constructor
//       Access: Public
//  Description: Constructs a NURBS patch.
////////////////////////////////////////////////////////////////////
BicubicPatch::
BicubicPatch(int s_order, int t_order,
	     const double s_knots[], const double t_knots[],
	     const pfVec4 cvs[], 
	     int ti_scale, int si_offset, int ti_offset) {
  nurbs_basis(s_order, t_order,
	      s_knots, t_knots,
	      cvs,
	      ti_scale, si_offset, ti_offset);
}



////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::get_point
//       Access: Public, Scheme, Virtual
//  Description: Computes the surface point at a given parametric
//               point s,t.  
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
get_point(double s, double t, VecType &point) const {
  evaluate_point(pfVec4(s*s*s, s*s, s, 1.0),
		 pfVec4(t*t*t, t*t, t, 1.0),
		 point);
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::get_s_tan
//       Access: Public, Scheme, Virtual
//  Description: Computes the tangent to the surface in the s
//               direction at a given parametric point s,t.
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
get_s_tan(double s, double t, VecType &tangent) const {
  evaluate_vector(pfVec4(3.0*s*s, 2.0*s, 1.0, 0.0),
		  pfVec4(t*t*t, t*t, t, 1.0),
		  tangent);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::get_t_tan
//       Access: Public, Scheme, Virtual
//  Description: Computes the tangent to the surface in the t
//               direction at a given parametric point s,t.
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
get_t_tan(double s, double t, VecType &tangent) const {
  evaluate_vector(pfVec4(s*s*s, s*s, s, 1.0),
		  pfVec4(3.0*t*t, 2.0*t, 1.0, 0.0),
		  tangent);
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::get_normal
//       Access: Public, Scheme, Virtual
//  Description: Computes the surface normal at a given parametric
//               point s,t.  
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
get_normal(double s, double t, VecType &normal) const {
  pfVec4 sv(s*s*s, s*s, s, 1.0);
  pfVec4 tv(t*t*t, t*t, t, 1.0);
  pfVec4 dsv(3.0*s*s, 2.0*s, 1.0, 0.0);
  pfVec4 dtv(3.0*t*t, 2.0*t, 1.0, 0.0);

  VecType s_tan, t_tan;
  evaluate_vector(dsv, tv, s_tan);
  evaluate_vector(sv, dtv, t_tan);
  normal.cross(s_tan, t_tan);
  normal.normalize();

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::get_pn
//       Access: Public, Scheme, Virtual
//  Description: Computes the point and the normal of the patch at a
//               given parametric point s,t.  
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
get_pn(double s, double t, VecType &point, VecType &normal) const {
  pfVec4 sv(s*s*s, s*s, s, 1.0);
  pfVec4 tv(t*t*t, t*t, t, 1.0);
  pfVec4 dsv(3.0*s*s, 2.0*s, 1.0, 0.0);
  pfVec4 dtv(3.0*t*t, 2.0*t, 1.0, 0.0);

  evaluate_point(sv, tv, point);

  VecType s_tan, t_tan;
  evaluate_vector(dsv, tv, s_tan);
  evaluate_vector(sv, dtv, t_tan);
  normal.cross(s_tan, t_tan);
  normal.normalize();

  return true;
}



////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::hermite_basis
//       Access: Public
//  Description: Recomputes the basis matrix as a Hermite patch.  The
//               four CV's are in the order of (s=0,t=0), (s=1,t=0),
//               (s=0,t=1), and (s=1,t=1).
////////////////////////////////////////////////////////////////////
void BicubicPatch::
hermite_basis(const HermitePatchCV &cv00,
	      const HermitePatchCV &cv10,
	      const HermitePatchCV &cv01,
	      const HermitePatchCV &cv11) {
  static LMatrix4f
    Mh(2, -3, 0, 1,
       -2, 3, 0, 0,
       1, -2, 1, 0,
       1, -1, 0, 0);
  static LMatrix4f 
    MhT(2, -2, 1, 1,
	-3, 3, -2, -1,
	0, 0, 1, 0,
	1, 0, 0, 0);

  LMatrix4f
    Ghx(cv00._p[0], cv01._p[0], cv00._t_out[0], cv01._t_in[0],
	cv10._p[0], cv11._p[0], cv10._t_out[0], cv11._t_in[0],
	cv00._s_out[0], cv01._s_out[0], cv00._twist[0], cv01._twist[0],
	cv10._s_in[0], cv11._s_in[0], cv10._twist[0], cv11._twist[0]);

  LMatrix4f
    Ghy(cv00._p[1], cv01._p[1], cv00._t_out[1], cv01._t_in[1],
	cv10._p[1], cv11._p[1], cv10._t_out[1], cv11._t_in[1],
	cv00._s_out[1], cv01._s_out[1], cv00._twist[1], cv01._twist[1],
	cv10._s_in[1], cv11._s_in[1], cv10._twist[1], cv11._twist[1]);

  LMatrix4f
    Ghz(cv00._p[2], cv01._p[2], cv00._t_out[2], cv01._t_in[2],
	cv10._p[2], cv11._p[2], cv10._t_out[2], cv11._t_in[2],
	cv00._s_out[2], cv01._s_out[2], cv00._twist[2], cv01._twist[2],
	cv10._s_in[2], cv11._s_in[2], cv10._twist[2], cv11._twist[2]);


  Bx = MhT * Ghx * Mh;
  By = MhT * Ghy * Mh;
  Bz = MhT * Ghz * Mh;
  rational = false;
}


////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::bezier_basis
//       Access: Public
//  Description: Recomputes the basis matrix as a Bezier patch.  The
//               matrices represent the sixteen Bezier control points.
//               Gx[0][0] is the x-coordinate of point (s=0,t=0),
//               Gy[1][0] is the y-coordinate of point (s=1,t=0), etc.
////////////////////////////////////////////////////////////////////
void BicubicPatch::
bezier_basis(const LMatrix4f &Gx, 
	     const LMatrix4f &Gy,
	     const LMatrix4f &Gz) {
  static LMatrix4f
    Mb(-1, 3, -3, 1,
       3, -6, 3, 0,
       -3, 3, 0, 0,
       1, 0, 0, 0);
  static LMatrix4f
    MbT(-1, 3, -3, 1,
       3, -6, 3, 0,
       -3, 3, 0, 0,
       1, 0, 0, 0);


  Bx = MbT * Gx * Mb;
  By = MbT * Gy * Mb;
  Bz = MbT * Gz * Mb;
  rational = false;
}

////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::nurbs_basis
//       Access: Public
//  Description: Recomputes the basis matrix as a NURBS patch.  Order
//               is one more than the degree, and must be 1, 2, 3, or
//               4; knots is an array of order*2 values, in both
//               directions.  cvs is a pool of CV's; cv(s,t) is
//               cvs[(t+ti_offset) * ti_scale + (s+si_offset)].
////////////////////////////////////////////////////////////////////
void BicubicPatch::
nurbs_basis(int s_order, int t_order,
	    const double s_knots[], const double t_knots[],
	    const pfVec4 cvs[], 
	    int ti_scale, int si_offset, int ti_offset) {
  dnassert(s_order>=1 && s_order<=4);
  dnassert(t_order>=1 && t_order<=4);

  LMatrix4f sB, tB, sBT;

  compute_nurbs_basis(s_order, s_knots, sB);
  compute_nurbs_basis(t_order, t_knots, tB);
  sBT.transpose(sB);

  // Create a local copy of our CV's, so we can zero out the unused
  // elements.
  pfVec4 c[4][4];
  for (int s = 0; s < 4; s++) {
    for (int t = 0; t < 4; t++) {
      c[s][t] = (s<s_order && t<t_order) ? 
		 cvs[(t+ti_offset) * ti_scale + (s+si_offset)] :
		 pfVec4(0.0, 0.0, 0.0, 0.0);
    }
  }

  LMatrix4f 
    Gx(c[0][0][0], c[0][1][0], c[0][2][0], c[0][3][0],
       c[1][0][0], c[1][1][0], c[1][2][0], c[1][3][0],
       c[2][0][0], c[2][1][0], c[2][2][0], c[2][3][0],
       c[3][0][0], c[3][1][0], c[3][2][0], c[3][3][0]);

  LMatrix4f 
    Gy(c[0][0][1], c[0][1][1], c[0][2][1], c[0][3][1],
       c[1][0][1], c[1][1][1], c[1][2][1], c[1][3][1],
       c[2][0][1], c[2][1][1], c[2][2][1], c[2][3][1],
       c[3][0][1], c[3][1][1], c[3][2][1], c[3][3][1]);

  LMatrix4f 
    Gz(c[0][0][2], c[0][1][2], c[0][2][2], c[0][3][2],
       c[1][0][2], c[1][1][2], c[1][2][2], c[1][3][2],
       c[2][0][2], c[2][1][2], c[2][2][2], c[2][3][2],
       c[3][0][2], c[3][1][2], c[3][2][2], c[3][3][2]);

  LMatrix4f 
    Gw(c[0][0][3], c[0][1][3], c[0][2][3], c[0][3][3],
       c[1][0][3], c[1][1][3], c[1][2][3], c[1][3][3],
       c[2][0][3], c[2][1][3], c[2][2][3], c[2][3][3],
       c[3][0][3], c[3][1][3], c[3][2][3], c[3][3][3]);

  Bx = sBT * Gx * tB;
  By = sBT * Gy * tB;
  Bz = sBT * Gz * tB;
  Bw = sBT * Gw * tB;

  rational = true;
}


////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::GetBezierPatch
//       Access: Public, Virtual
//  Description: Fills the BezierPatch structure with a description of
//               the patch as a Bezier, if possible, but does not
//               change the _s or _t members of the structure.
//               Returns true if successful, false otherwise.
////////////////////////////////////////////////////////////////////
bool BicubicPatch::
GetBezierPatch(BezierPatch &patch) const {
  static LMatrix4f
    Mbi(0.0, 0.0, 0.0, 1.0,
	0.0, 0.0, 1.0/3.0, 1.0,
	0.0, 1.0/3.0, 2.0/3.0, 1.0,
	1.0, 1.0, 1.0, 1.0);

  LMatrix4f Gx = Mbi * Bx * Mbi;
  LMatrix4f Gy = Mbi * By * Mbi;
  LMatrix4f Gz = Mbi * Bz * Mbi;
  LMatrix4f Gw(1.0, 1.0, 1.0, 1.0,
	      1.0, 1.0, 1.0, 1.0,
	      1.0, 1.0, 1.0, 1.0,
	      1.0, 1.0, 1.0, 1.0);

  if (rational) {
    Gw = Mbi * Bw * Mbi;
  }
  
  patch._v[0][0].set(Gx[0][0]/Gw[0][0], Gy[0][0]/Gw[0][0], Gz[0][0]/Gw[0][0]);
  patch._v[0][1].set(Gx[0][1]/Gw[0][1], Gy[0][1]/Gw[0][1], Gz[0][1]/Gw[0][1]);
  patch._v[0][2].set(Gx[0][2]/Gw[0][2], Gy[0][2]/Gw[0][2], Gz[0][2]/Gw[0][2]);
  patch._v[0][3].set(Gx[0][3]/Gw[0][3], Gy[0][3]/Gw[0][3], Gz[0][3]/Gw[0][3]);
  
  patch._v[1][0].set(Gx[1][0]/Gw[1][0], Gy[1][0]/Gw[1][0], Gz[1][0]/Gw[1][0]);
  patch._v[1][1].set(Gx[1][1]/Gw[1][1], Gy[1][1]/Gw[1][1], Gz[1][1]/Gw[1][1]);
  patch._v[1][2].set(Gx[1][2]/Gw[1][2], Gy[1][2]/Gw[1][2], Gz[1][2]/Gw[1][2]);
  patch._v[1][3].set(Gx[1][3]/Gw[1][3], Gy[1][3]/Gw[1][3], Gz[1][3]/Gw[1][3]);
  
  patch._v[2][0].set(Gx[2][0]/Gw[2][0], Gy[2][0]/Gw[2][0], Gz[2][0]/Gw[2][0]);
  patch._v[2][1].set(Gx[2][1]/Gw[2][1], Gy[2][1]/Gw[2][1], Gz[2][1]/Gw[2][1]);
  patch._v[2][2].set(Gx[2][2]/Gw[2][2], Gy[2][2]/Gw[2][2], Gz[2][2]/Gw[2][2]);
  patch._v[2][3].set(Gx[2][3]/Gw[2][3], Gy[2][3]/Gw[2][3], Gz[2][3]/Gw[2][3]);
  
  patch._v[3][0].set(Gx[3][0]/Gw[3][0], Gy[3][0]/Gw[3][0], Gz[3][0]/Gw[3][0]);
  patch._v[3][1].set(Gx[3][1]/Gw[3][1], Gy[3][1]/Gw[3][1], Gz[3][1]/Gw[3][1]);
  patch._v[3][2].set(Gx[3][2]/Gw[3][2], Gy[3][2]/Gw[3][2], Gz[3][2]/Gw[3][2]);
  patch._v[3][3].set(Gx[3][3]/Gw[3][3], Gy[3][3]/Gw[3][3], Gz[3][3]/Gw[3][3]);

  return true;
}



////////////////////////////////////////////////////////////////////
//     Function: BicubicPatch::Destructor
//       Access: Protected, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
BicubicPatch::
~BicubicPatch() {
}


