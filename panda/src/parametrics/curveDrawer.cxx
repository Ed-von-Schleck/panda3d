// Filename: curveDrawer.C
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
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////

#include "parametrics.h"
#include "curveDrawer.h"
#include "patch.h"

#include <math.h>
#include <Performer/pf/pfGeode.h>


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::Constructor
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////

ParametricCurveDrawer* make_ParametricCurveDrawer( ParametricCurve *curve )
{
  return ( new ParametricCurveDrawer(curve) );
}

ParametricCurveDrawer::
ParametricCurveDrawer(ParametricCurve *curve) {
  _curve = curve;
  _time_curve = NULL;
  _surface = NULL;
  _lines.set_color(1.0, 1.0, 1.0);
  _ticks.set_color(1.0, 0.0, 0.0);
  _tick_scale = 0.1;
  _num_segs = 100;
  _num_ticks = 0;
  _frame_accurate = false;
  _geode = new pfGeode;
  pfRef(_geode);
  _drawn = false;
  _mapper = DefaultMap;
  _curve->register_drawer(this);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::Destructor
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////

void rm_ParametricCurveDrawer( ParametricCurveDrawer* pcd )
{
  delete pcd;
}

ParametricCurveDrawer::
~ParametricCurveDrawer() {
  hide();
  pfUnrefDelete(_geode);
  if (_curve!=NULL) {
    _curve->unregister_drawer(this);
  }
  if (_time_curve!=NULL) {
    _time_curve->unregister_drawer(this);
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_curve(ParametricCurve *curve) {
  if (_curve!=NULL) {
    _curve->unregister_drawer(this);
  }
  _curve = curve;
  _curve->register_drawer(this);
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveDrawer::
get_curve() {
  return _curve;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_time_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_time_curve(ParametricCurve *curve) {
  if (_time_curve!=NULL) {
    _time_curve->unregister_drawer(this);
  }
  _time_curve = curve;
  if (_time_curve!=NULL) {
    _time_curve->register_drawer(this);
  }
  if (_drawn) {
    draw();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_time_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveDrawer::
get_time_curve() {
  return _time_curve;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_surface
//       Access: Public, Scheme
//  Description: Defines the curve as a curve-on-surface.  This curve
//               will be drawn on the surface of an arbitrary
//               ParametricSurface.  It is assumed that the curve will
//               be defined in the (x, y) plane with coordinates in
//               the range [0,1].
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_surface(ParametricSurface *curve) {
  _surface = curve;
  if (_drawn) {
    draw();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_surface
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurface *ParametricCurveDrawer::
get_surface() {
  return _surface;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_geode
//       Access: Public, Scheme
//  Description: Returns a pointer to the drawer's pfGeode.  This is
//               where the drawer will build the visible
//               representation of the curve.  This geode must be
//               inserted into the scene graph to make the curve
//               visible.  The geode remains connected to the drawer,
//               so that future updates to the drawer will reflect in
//               the geode, and the geode will be emptied when the
//               drawer destructs.  Also see detach_geode().
////////////////////////////////////////////////////////////////////
pfGeode *ParametricCurveDrawer::
get_geode() {
  return _geode;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::detach_geode
//       Access: Public, Scheme
//  Description: Detaches the pfGeode from the drawer so that the
//               drawing will remain after the death of the drawer.
//               Returns the now-static geode.  A new, dynamic geode
//               is created for the drawer's future use; get_geode()
//               will return this new geode which will be empty until
//               the next call to draw().
////////////////////////////////////////////////////////////////////
pfGeode *ParametricCurveDrawer::
detach_geode() {
  if (!_drawn) {
    draw();
  }
  pfGeode *g = _geode;
  pfUnref(g);
  _geode = new pfGeode;
  pfRef(_geode);
  _drawn = false;
  return g;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_num_segs
//       Access: Public, Scheme
//  Description: Specifies the number of line segments used to
//               approximate the curve for each parametric unit.  This
//               just affects the visual appearance of the curve as it
//               is drawn.  The total number of segments drawn for the
//               curve will be get_max_t() * get_num_segs().
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_num_segs(int num_segs) {
  _num_segs = num_segs;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_num_segs
//       Access: Public, Scheme
//  Description: Returns the number of line segments used to
//               approximate the curve for each parametric unit.  This
//               just affects the visual appearance of the curve as it
//               is drawn.  The total number of segments drawn for the
//               curve will be get_max_t() * get_num_segs().
////////////////////////////////////////////////////////////////////
int ParametricCurveDrawer::
get_num_segs() const {
  return _num_segs;
}




////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_num_ticks
//       Access: Public, Scheme
//  Description: Specifies the number of time tick marks drawn
//               for each unit of time.  These tick marks are drawn at
//               equal increments in time to give a visual
//               approximation of speed.  Specify 0 to disable drawing
//               of tick marks.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_num_ticks(int num_ticks) {
  _num_ticks = num_ticks;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_num_ticks
//       Access: Public, Scheme
//  Description: Returns the number of time tick marks per unit of
//               time drawn.
////////////////////////////////////////////////////////////////////
int ParametricCurveDrawer::
get_num_ticks() const {
  return _num_ticks;
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_color
//       Access: Public, Scheme
//  Description: Specifies the color of the curve when it is drawn.
//               The default is white.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_color(float r, float g, float b) {
  _lines.set_color(r, g, b);
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_color
//       Access: Public, Scheme
//  Description: Specifies the color of the time tick marks drawn on
//               the curve.  The default is red.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_tick_color(float r, float g, float b) {
  _ticks.set_color(r, g, b);
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_frame_accurate
//       Access: Public, Scheme
//  Description: Specifies whether the curve drawn is to be
//               frame-accurate.  If true, then changes made to the
//               curve dynamically after it has been drawn will be
//               reflected correctly in the render window.  If false,
//               dynamic updates may be drawn before the rest of the
//               scene has updated.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_frame_accurate(boolean frame_accurate) {
  _frame_accurate = frame_accurate;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_frame_accurate
//       Access: Public, Scheme
//  Description: Returns whether the curve is drawn in frame-accurate
//               mode.
////////////////////////////////////////////////////////////////////
boolean ParametricCurveDrawer::
get_frame_accurate() const {
  return _frame_accurate;
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::draw
//       Access: Public, Scheme, Virtual
//  Description: Creates a series of line segments that approximates
//               the curve.  These line segments may be made visible
//               by adding the geode returned by get_geode() into the
//               scene graph.
////////////////////////////////////////////////////////////////////
boolean ParametricCurveDrawer::
draw() {
  // First, remove the old drawing, if any.
  hide();

  // If there's no curve, draw nothing and return false.
  if (_curve==NULL || !_curve->is_valid()) {
    return false;
  }

  // Otherwise, let's go to town!
  int total_segs = floor(_curve->get_max_t() * _num_segs + 0.5);

  double scale = _curve->get_max_t() / (double)(total_segs-1);
  double t;
  VecType point, tangent;
  boolean last_in, next_in;

  last_in = false;
  int i;

  for (i = 0; i<total_segs; i++) {
    t = (double)i * scale;

    // Since we're just drawing the geometric shape of the curve, we
    // don't care at this point about the time curve.

    next_in = _curve->get_pt(t, point, tangent);

    pfVec3 p = _mapper(point, tangent, t);

    // But we do care about the surface, if we have such a thing.
    if (_surface) {
      _surface->get_point(p[0], p[1], p);
    }

    if (!next_in || !last_in) {
      _lines.move_to(p);
    } else {
      _lines.draw_to(p);
    }
    last_in = next_in;
  }

  _lines.create(_geode, _frame_accurate);
  _drawn = true;

  // Now draw the time tick marks.
  if (_num_ticks > 0) {
    VecType tangent2;
    int total_ticks = floor(_curve->get_max_t() * _num_ticks + 0.5);

    scale = get_max_t() / (double)(total_ticks-1);
    for (i = 0; i<total_ticks; i++) {
      t = (double)i * scale;
      if (_time_curve!=NULL) {
	_time_curve->get_point(t, point);
	t = point[0];
      }

      _curve->get_pt(t, point, tangent);
      _curve->get_2ndtangent(t, tangent2);
      
      pfVec3 pt = _mapper(point, tangent, t);
      if (_surface) {
	tangent += pt;
	tangent2 += pt;
	_surface->get_point(pt[0], pt[1], pt);
	_surface->get_point(tangent[0], tangent[1], tangent);
	_surface->get_point(tangent2[0], tangent2[1], tangent2);
	tangent -= pt;
	tangent2 -= pt;
      }
      pfVec3 t1, t2;
      get_tick_marks(_mapper(tangent, tangent2, t + 1.0), t1, t2);
      
      _ticks.move_to(pt - t1 * _tick_scale);
      _ticks.draw_to(pt + t1 * _tick_scale);
      _ticks.move_to(pt - t2 * _tick_scale);
      _ticks.draw_to(pt + t2 * _tick_scale);
    }
    _ticks.create(_geode, _frame_accurate);
  }
	

  return true;
}

    

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::recompute
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
boolean ParametricCurveDrawer::
recompute(double t1, double t2, ParametricCurve *curve) {
  if (!_drawn || _curve==NULL || !_curve->is_valid()) {
    return false;
  }

  boolean redraw_curve = true;

  if (_time_curve!=NULL) {
    if (curve != _time_curve) {
      // If the recompute call came from the basic curve, and not from
      // the time curve, the t1 and t2 it gave us aren't the t1 and t2
      // we need.  To be safe, we'll run over the whole range.
      t1 = 0.0;
      t2 = get_max_t();
    } else {
      // On the other hand, if the recompute call came from the time
      // curve, we won't be changing the curve's geometric shape at
      // all--we only need to move the tick marks.
      redraw_curve = false;
    }
  }

  // Scale t1 and t2 to [0, 1].
  t1 = min(max(t1 / get_max_t(), 0.0), 1.0);
  t2 = min(max(t2 / get_max_t(), 0.0), 1.0);

  int n1, n2, i;
  double scale, t;
  VecType point, tangent;

  if (redraw_curve) {
    // Compute the number of total segments we will draw.  This is based
    // on the parametric length of the curve, _curve->get_max_t().
    
    int total_segs = floor(_curve->get_max_t() * _num_segs + 0.5);
    
    n1 = (int)floor(t1 * (total_segs-1));
    n2 = (int)ceil(t2 * (total_segs-1));
    
    // This should be implied by the above t1, t2 bounds check.
    dnassert(n1>=0 && n1<total_segs);
    dnassert(n2>=0 && n2<total_segs);
    
    scale = _curve->get_max_t() / (double)(total_segs-1);
    
    for (i = n1; i<=n2; i++) {
      t = (double)i * scale;
      
      _curve->get_pt(t, point, tangent);

      pfVec3 p = _mapper(point, tangent, t);
      if (_surface) {
	_surface->get_point(p[0], p[1], p);
      }
      _lines.set_vertex(i, p);
    }
  }
    
  if (_num_ticks > 0) {
    VecType tangent2;
    int total_ticks = floor(_curve->get_max_t() * _num_ticks + 0.5);

    n1 = (int)floor(t1 * (total_ticks-1));
    n2 = (int)ceil(t2 * (total_ticks-1));

    scale = get_max_t() / (double)(total_ticks-1);

    for (i = n1; i<=n2; i++) {
      t = (double)i * scale;
      if (_time_curve!=NULL) {
	_time_curve->get_point(t, point);
	t = point[0];
      }
      _curve->get_pt(t, point, tangent);
      _curve->get_2ndtangent(t, tangent2);
      
      pfVec3 pt = _mapper(point, tangent, t);
      if (_surface) {
	tangent += pt;
	tangent2 += pt;
	_surface->get_point(pt[0], pt[1], pt);
	_surface->get_point(tangent[0], tangent[1], tangent);
	_surface->get_point(tangent2[0], tangent2[1], tangent2);
	tangent -= pt;
	tangent2 -= pt;
      }
      pfVec3 t1, t2;
      get_tick_marks(_mapper(tangent, tangent2, t + 1.0), 
		     t1, t2);
      
      int ti = i * 4;
      _ticks.set_vertex(ti, pt - t1 * _tick_scale);
      _ticks.set_vertex(ti+1, pt + t1 * _tick_scale);
      _ticks.set_vertex(ti+2, pt - t2 * _tick_scale);
      _ticks.set_vertex(ti+3, pt + t2 * _tick_scale);
    }
  }
  return true;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::hide
//       Access: Public, Scheme
//  Description: Removes the lines that were created by a previous
//               call to draw().
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
hide() {
  pfGeoSet *geoset;
  int ok;
  int n = _geode->getNumGSets();
  while (n > 0) {
    geoset = _geode->getGSet(--n);
    ok = _geode->removeGSet(geoset);
    dnassert(ok);
    ok = pfDelete(geoset);
    if (!ok) {
      DWARNING(dnparametrics) << "Geoset not deleted." << dnend;
    }
  }
  _drawn = false;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_tick_scale
//       Access: Public, Scheme
//  Description: Sets the visible size of the time tick marks.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_tick_scale(double scale) {
  _tick_scale = scale;
  if (_drawn) {
    draw();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_tick_scale
//       Access: Public, Scheme
//  Description: Returns the size of the time tick marks.
////////////////////////////////////////////////////////////////////
double ParametricCurveDrawer::
get_tick_scale() const {
  return _tick_scale;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_graph_type
//       Access: Public, Scheme
//  Description: Selects one of a handful of pre-canned graph types
//               the drawer can represent.  The default, PCD_DEFAULT,
//               draws the curve's shape in three-dimensional space;
//               other possibilites like PCD_XVST draw a graph of X(t)
//               vs. t in the Z and X axes, respectively.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_graph_type(int graph_type) {
  switch (graph_type) {
  case PCD_DEFAULT:
    set_mapper(DefaultMap);
    break;

  case PCD_XVST:
    set_mapper(XvsT);
    break;

  case PCD_YVST:
    set_mapper(YvsT);
    break;

  case PCD_ZVST:
    set_mapper(ZvsT);
    break;

  case PCD_DXVST:
    set_mapper(dXvsT);
    break;

  case PCD_DYVST:
    set_mapper(dYvsT);
    break;

  case PCD_DZVST:
    set_mapper(dZvsT);
    break;

  case PCD_IXVST:
    set_mapper(iXvsT);
    break;

  case PCD_IYVST:
    set_mapper(iYvsT);
    break;

  default:
    DWARNING(dnparametrics) << "Invalid graph_type " << graph_type << dnend;
  }
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::disable
//       Access: Public
//  Description: Called by the ParametricCurve destructor to indicate
//               that a curve we are depending on has just been
//               deleted.  We must no longer attempt to access this
//               curve.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
disable(ParametricCurve *curve) {
  if (curve==_time_curve) {
    _time_curve = NULL;
  } else if (curve==_curve) {
    // Hmm, the primary curve has destructed.  We're useless now.
    _curve = NULL;
    hide();
  } else {
    DWARNING(dnparametrics)
      << "ParametricCurveDrawer::disable() called on nonsensible curve"
      << dnend;
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::set_mapper
//       Access: Public
//  Description: This establishes the function that will be applied to
//               each point of the four-dimensional curve to translate
//               it to a three-dimensional representation.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
set_mapper(VecTypeMapper *mapper) {
  // If the mapper hasn't changed, don't force a redraw.
  if (_mapper != mapper) {
    _mapper = mapper;
    if (_drawn) {
      draw();
    }
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::DefaultMap
//       Access: Public, Static
//  Description: This mapping function returns the X,Y,Z component of
//               each point, showing the line's three-dimensional
//               shape.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
DefaultMap(const VecType &point, const VecType &, double) {
  return pfVec3(point[0], point[1], point[2]);
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::XvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of X(t), with the
//               x along the Y axis and t along the X axis.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
XvsT(const VecType &point, const VecType &, double t) {
  return pfVec3(t, point[0], 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::iXvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of X(t), with the
//               x along the X axis and t along the Y axis.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
iXvsT(const VecType &point, const VecType &, double t) {
  return pfVec3(point[0], t, 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::YvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of Y(t), with the
//               y along the Y axis and t along the X axis.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
YvsT(const VecType &point, const VecType &, double t) {
  return pfVec3(t, point[1], 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::YvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of Y(t), with the
//               y along the X axis and t along the Y axis.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
iYvsT(const VecType &point, const VecType &, double t) {
  return pfVec3(point[1], t, 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::ZvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of Z(t), with the
//               z along the Y axis and t along the X axis.
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
ZvsT(const VecType &point, const VecType &, double t) {
  return pfVec3(t, point[2], 0.0);
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::dXvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of dX(t), the
//               derivative of X(t).
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
dXvsT(const VecType &, const VecType &tangent, double t) {
  return pfVec3(t, tangent[0], 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::dYvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of dY(t), the
//               derivative of Y(t).
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
dYvsT(const VecType &, const VecType &tangent, double t) {
  return pfVec3(t, tangent[1], 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::dZvsT
//       Access: Public, Static
//  Description: This mapping function shows a graph of dZ(t), the
//               derivative of Z(t).
////////////////////////////////////////////////////////////////////
pfVec3 ParametricCurveDrawer::
dZvsT(const VecType &, const VecType &tangent, double t) {
  return pfVec3(t, tangent[2], 0.0);
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveDrawer::get_tick_marks
//       Access: Protected, Static
//  Description: Given a tangent vector, computes two vectors at right
//               angles to the tangent and to each other, suitable for
//               drawing as tick marks.
////////////////////////////////////////////////////////////////////
void ParametricCurveDrawer::
get_tick_marks(const pfVec3 &tangent, pfVec3 &t1, pfVec3 &t2) {
  pfVec3 tn = tangent;
  tn.normalize();

  // Decide the smallest axis of tn and cross with the corresponding
  // unit vector.
  if (fabs(tn[0]) <= fabs(tn[1]) && fabs(tn[0]) <= fabs(tn[2])) {
    // X is smallest.
    t1.cross(tn, pfVec3(1.0, 0.0, 0.0));

  } else if (fabs(tn[1]) <= fabs(tn[2])) {
    // Y is smallest.
    t1.cross(tn, pfVec3(0.0, 1.0, 0.0));

  } else {
    // Z is smallest.
    t1.cross(tn, pfVec3(0.0, 0.0, 1.0));
  }

  t2.cross(tn, t1);
}
