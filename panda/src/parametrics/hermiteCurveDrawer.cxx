// Filename: hermiteCurveDrawer.C
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
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////

#include "parametrics.h"
#include "hermiteCurveDrawer.h"
#include "patch.h"

#include <math.h>
#include <Performer/pf/pfGeode.h>


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::Constructor
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////

HermiteCurveDrawer* make_HermiteCurveDrawer( HermiteCurve *curve )
{
  return ( new HermiteCurveDrawer(curve) );
}

HermiteCurveDrawer::
HermiteCurveDrawer(HermiteCurve *curve) :
  ParametricCurveDrawer(curve) {
    _pr = 1.0; _pg = 0.0; _pb = 0.0;
    _points.set_color(_pr, _pg, _pb);
    _tangents.set_color(_pr, _pg, _pb);
    _points.set_thickness(4.0);
    _num_points = 0;
    _show_cvs = true;
    _tangent_scale = 1.0/3.0;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::Destructor
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////

void rm_HermiteCurveDrawer( HermiteCurveDrawer* hcd )
{
  delete hcd;
}

HermiteCurveDrawer::
~HermiteCurveDrawer() {
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::set_cv_color
//       Access: Public, Scheme
//  Description: Specifies the color of the CV's and tangents.  The
//               default is red.
////////////////////////////////////////////////////////////////////
void HermiteCurveDrawer::
set_cv_color(float r, float g, float b) {
  _pr = r;
  _pg = g;
  _pb = b;
  _points.set_color(_pr, _pg, _pb);
  _tangents.set_color(_pr, _pg, _pb);

  if (_drawn) {
    draw();
  }
}



////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::draw
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
boolean HermiteCurveDrawer::
draw() {
  // First, draw the curve itself.
  if (!ParametricCurveDrawer::draw()) {
    return false;
  }

  if (_show_cvs) {
    HermiteCurve *herm = (HermiteCurve *)_curve;
    
    // Now let's fill in the control vertices and tangents.
    _num_points = herm->get_num_cvs();

    double t;
    double in_length, out_length;
    VecType point;
    VecType in_tangent, out_tangent;
    for (int i = 0; i<_num_points; i++) {
      t = herm->get_cv_tstart(i);

      in_length = (i==0) ? 0.0 : herm->get_tlength(i-1);
      out_length = (i+1==_num_points) ? 0.0 : herm->get_tlength(i);

      point = herm->get_cv_point(i);
      in_tangent = herm->get_cv_in(i);
      out_tangent = herm->get_cv_out(i);

      pfVec3 pi = _mapper(point, in_tangent, t);
      pfVec3 po = _mapper(point, out_tangent, t);

      pfVec3 tan_pi = _mapper(point - in_tangent * _tangent_scale,
			      in_tangent,
			      t - in_length * _tangent_scale);
      pfVec3 tan_po = _mapper(point + out_tangent * _tangent_scale,
			      out_tangent,
			      t + out_length * _tangent_scale);

      if (_surface) {
	_surface->get_point(pi[0], pi[1], pi);
	_surface->get_point(po[0], po[1], po);
	_surface->get_point(tan_pi[0], tan_pi[1], tan_pi);
	_surface->get_point(tan_po[0], tan_po[1], tan_po);
      }

      _points.move_to(pi);
      _points.move_to(po);

      _tangents.move_to(pi);
      _tangents.draw_to(tan_pi);

      _tangents.move_to(po);
      _tangents.draw_to(tan_po);
    }

    _points.create(_geode, _frame_accurate);
    _tangents.create(_geode, _frame_accurate);
  }

  return true;
}

    

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::recompute
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
boolean HermiteCurveDrawer::
recompute(double t1, double t2, ParametricCurve *curve) {
  // First, recompute the curve itself.
  if (!ParametricCurveDrawer::recompute(t1, t2, curve)) {
    return false;
  }

  if (_show_cvs) {
    HermiteCurve *herm = (HermiteCurve *)_curve;
    
    // Now reposition the CV's and tangents.
    double t;
    double in_length, out_length;
    VecType point;
    VecType in_tangent, out_tangent;

    int n1 = (int)ceil(t1 * (_num_points-1));
    int n2 = (int)floor(t2 * (_num_points-1));

    for (int i = 0; i < _num_points; i++) {
      t = herm->get_cv_tstart(i);
      if (t > t2) {
	// All done.
	break;
      }
      if (t < t1) {
	// Still outside the adjusted region.
      } else {
	in_length = (i==0) ? 0.0 : herm->get_tlength(i-1);
	out_length = (i+1==_num_points) ? 0.0 : herm->get_tlength(i);

	point = herm->get_cv_point(i);
	in_tangent = herm->get_cv_in(i);
	out_tangent = herm->get_cv_out(i);
	
	pfVec3 pi = _mapper(point, in_tangent, t);
	pfVec3 po = _mapper(point, out_tangent, t);
	
	pfVec3 tan_pi = _mapper(point - in_tangent * _tangent_scale,
				in_tangent,
				t - in_length * _tangent_scale);
	pfVec3 tan_po = _mapper(point + out_tangent * _tangent_scale,
				out_tangent,
				t + out_length * _tangent_scale);
	
	if (_surface) {
	  _surface->get_point(pi[0], pi[1], pi);
	  _surface->get_point(po[0], po[1], po);
	  _surface->get_point(tan_pi[0], tan_pi[1], tan_pi);
	  _surface->get_point(tan_po[0], tan_po[1], tan_po);
	}
	
	int ti = 2*i;
	_points.set_vertex(ti, pi);
	_points.set_vertex(ti+1, po);
	
	ti = 4*i;
	_tangents.set_vertex(ti, pi);
	_tangents.set_vertex(ti+1, tan_pi);
	
	_tangents.set_vertex(ti+2, po);
	_tangents.set_vertex(ti+3, tan_po);
      }

    }
  }
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::set_show_cvs
//       Access: Public, Scheme
//  Description: Sets the flag that hides or shows the CV's and
//               tangents.  True shows them; false hides them (in
//               which case HermiteCurveDrawer works just like
//               ParametricCurveDrawer).
////////////////////////////////////////////////////////////////////
void HermiteCurveDrawer::
set_show_cvs(boolean flag) {
  _show_cvs = flag;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::get_show_cvs
//       Access: Public, Scheme
//  Description: Returns the current state of the show-CV's flag.
////////////////////////////////////////////////////////////////////
boolean HermiteCurveDrawer::
get_show_cvs() const {
  return _show_cvs;
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::hilight
//       Access: Public, Scheme
//  Description: Hilights a particular CV by showing it in a
//               different color.  Returns true if the CV exists and
//               has been drawn, false otherwise.
////////////////////////////////////////////////////////////////////
boolean HermiteCurveDrawer::
hilight(int n, float hr, float hg, float hb) {
  // If there's no curve, do nothing and return false.
  if (_curve==NULL || !_curve->is_valid()) {
    return false;
  }

  if (n<0 || 2*n>=_points.get_num_vertices()) {
    // Also return false if we're out of range.
    return false;
  }

  int ti = 2*n;

  _points.set_vertex_color(ti, hr, hg, hb);
  _points.set_vertex_color(ti+1, hr, hg, hb);

  ti = 4*n;
  _tangents.set_vertex_color(ti, hr, hg, hb);
  _tangents.set_vertex_color(ti+1, hr, hg, hb);
  _tangents.set_vertex_color(ti+2, hr, hg, hb);
  _tangents.set_vertex_color(ti+3, hr, hg, hb);

  return true;
}
  

////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::unhilight
//       Access: Public, Scheme
//  Description: Removes the hilight previously set on a CV.
////////////////////////////////////////////////////////////////////
boolean HermiteCurveDrawer::
unhilight(int n) {
  return hilight(n, _pr, _pg, _pb);
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::set_tangent_scale
//       Access: Public, Scheme
//  Description: Sets the amount by which the tangent vectors are
//               scaled visibly on the screen.  This only affects the
//               appearance of the tangents, not their affect on the
//               curve.  This is useful for making the tangents more
//               visible or reducing clutter.
//
//               The default value is 0.33333, which scales the
//               tangents so that their endpoints mark the positions
//               of the control points of a corresponding Bezier
//               curve.
////////////////////////////////////////////////////////////////////
void HermiteCurveDrawer::
set_tangent_scale(double scale) {
  _tangent_scale = scale;
  if (_drawn) {
    draw();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: HermiteCurveDrawer::get_tangent_scale
//       Access: Public, Scheme
//  Description: Returns the amount by which the tangent vectors are
//               scaled visibly on the screen.
////////////////////////////////////////////////////////////////////
double HermiteCurveDrawer::
get_tangent_scale() const {
  return _tangent_scale;
}

