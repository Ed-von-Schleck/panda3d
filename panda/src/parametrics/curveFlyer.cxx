// Filename: curveFlyer.C
// Created by:  drose (18Mar97)
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
#include "curveFlyer.h"
#include "lookAt.h"
#include <math.h>
#include <event.h>
#include <linMathOutput.h>


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::Constructor
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////

ParametricCurveFlyer* make_ParametricCurveFlyer( ParametricCurve* curve )
{
  return ( new ParametricCurveFlyer(curve) );
}

ParametricCurveFlyer::
ParametricCurveFlyer(ParametricCurve *curve) {
  _curve = curve;
  _hpr_curve = NULL;
  _time_curve = NULL;
  _play_rate = 1.0;
  _stop_event = NULL;
  _loop_flag = false;
  _face_forward = false;
  _start_time = 0.0;
  _stopped = true;
  _mat.makeIdent();
  _h = _p = 0.0;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::Destructor
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////

void rm_ParametricCurveFlyer( ParametricCurveFlyer* pcf )
{
  delete pcf;
}

ParametricCurveFlyer::
~ParametricCurveFlyer() {
  if (_stop_event!=NULL) {
    delete[] _stop_event;
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_curve(ParametricCurve* curve) {
  _curve = curve;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveFlyer::
get_curve() {
  return _curve;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_hpr_curve
//       Access: Public, Scheme
//  Description: Sets the curve that rotates the DCS each frame.  The
//               curve must be defined over the same range as the
//               primary curve.  Each point on the curve is taken as a
//               three-value h,p,r which is applied to the DCS's
//               rotation for the current frame.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_hpr_curve(ParametricCurve *curve) {
  _hpr_curve = curve;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_hpr_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveFlyer::
get_hpr_curve() {
  return _hpr_curve;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_time_curve
//       Access: Public, Scheme
//  Description: Sets the curve that scales the parametric time on the
//               primary curve.  The X coordinate of the time curve
//               must evaluate to a number 0.0 <= x <=
//               curve->get_max_t() over its entire range.  This
//               number is used as the value of t for the primary
//               curve.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_time_curve(ParametricCurve *curve) {
  _time_curve = curve;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_time_curve
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricCurve *ParametricCurveFlyer::
get_time_curve() {
  return _time_curve;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::start
//       Access: Public, Scheme
//  Description: Starts a flight sequence from the beginning.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
start() {
  if (_stopped) {
    _start_time = 0.0;
    cont();

  } else {
    // Reposition at beginning
    _start_time = pfGetFrameTimeStamp();
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::stop
//       Access: Public, Scheme
//  Description: Stops a flight sequence.  The DCS position is left
//               at the frame at the moment of stopping; the animation
//               may later be resumed by calling cont().
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
stop() {
  if (!_stopped) {
    _start_time = pfGetFrameTimeStamp() - _start_time;
    _stopped = true;
  }
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::cont
//       Access: Public, Scheme
//  Description: Continues a flight sequence from the point at which
//               it was previously stopped, or from the frame set by a
//               call to set_frame().
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
cont() {
  if (_stopped) {
    _start_time = pfGetFrameTimeStamp() - _start_time;
    _stopped = false;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::playing
//       Access: Public, Scheme
//  Description: Returns true if the flyer is currently flying,
//               false if it is stopped.
////////////////////////////////////////////////////////////////////
bool ParametricCurveFlyer::
playing() const {
  return !_stopped;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_loop
//       Access: Public, Scheme
//  Description: Sets the flyer's automatic loop mode.  True means
//               a DCS flying to the end of the curve will
//               automatically begin the next frame at the beginning;
//               false means it will stop (and possibly throw an
//               event).
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_loop(bool flag) {
  _loop_flag = flag;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_loop
//       Access: Public, Scheme
//  Description: Returns the flyer's automatic loop mode setting.
////////////////////////////////////////////////////////////////////
bool ParametricCurveFlyer::
get_loop() const {
  return _loop_flag;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_face_forward
//       Access: Public, Scheme
//  Description: Sets the flyer's face-forward mode.  True means the
//               DCS will be rotated each frame to face in the
//               direction of motion; any further rotation supplied by
//               a second rotation curve will be an offset of this.
//               False means the DCS will not automatically rotate in
//               to the direction of motion; a second rotation curve
//               will supply the entire rotation component.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_face_forward(bool flag) {
  _face_forward = flag;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_face_forward
//       Access: Public, Scheme
//  Description: Returns the flyer's face-forward setting.
////////////////////////////////////////////////////////////////////
bool ParametricCurveFlyer::
get_face_forward() const {
  return _face_forward;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_time
//       Access: Public, Scheme
//  Description: Sets the DCS to the position along the curve
//               corresponding to the given value for the curve's w
//               dimension.  Conceptually, this sets the time to the
//               given value.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_time(double time) {
  if (_stopped) {
    _start_time = time / _play_rate;
  } else {
    _start_time = pfGetFrameTimeStamp() - time / _play_rate;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_time
//       Access: Public, Scheme
//  Description: Returns the elapsed time on the curve since the start
//               of the flyer's animation.
////////////////////////////////////////////////////////////////////
double ParametricCurveFlyer::
get_time() const {
  return 
    (_stopped ? _start_time : pfGetFrameTimeStamp() - _start_time)
    * _play_rate;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_play_rate
//       Access: Public, Scheme
//  Description: Sets the rate of the flyer's animation.  This is the
//               number of seconds per unit time of the curve.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer:: 
set_play_rate(double rate) { 
  double t = get_time();
  _play_rate = rate;
  set_time(t);
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_play_rate
//       Access: Public, Scheme
//  Description: Returns the playback rate of the flyer's animation, in
//               seconds per curve time.
////////////////////////////////////////////////////////////////////
double ParametricCurveFlyer:: 
get_play_rate() const { 
  return _play_rate;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set_stop_event
//       Access: Public, Scheme
//  Description: Sets the event that will be thrown when the
//               DCS runs to the end and stops.  The default is not to
//               throw an event; NULL restores this default.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set_stop_event(const char *string) {
  if (_stop_event!=NULL) {
    delete[] _stop_event;
    _stop_event = NULL;
  }

  if (string!=NULL) {
    _stop_event = new char[strlen(string)+1];
    strcpy(_stop_event, string);
  }
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_matrix
//       Access: Public, Scheme
//  Description: Returns the transform matrix associated with the
//               flyer's current position along the curve.  This will
//               be an orthonormal transform.  Also see get_coord().
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
get_matrix(LMatrix4f &mat) {
  recompute_position();
  mat = _mat;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get_coord
//       Access: Public, Scheme
//  Description: Returns the position and direction associated with
//               the flyer's current position along the curve.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
get_coord(LVector3f &xyz, LVector3f &hpr) {
  recompute_position();
  xyz = _coord.xyz;
  hpr = _coord.hpr;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::get
//       Access: Public, Virtual
//  Description: The Joinable output function; returns the LMatrix4f
//               that should be applied to the DCS.
////////////////////////////////////////////////////////////////////
void *ParametricCurveFlyer::
get() {
  recompute_position();
  return (void *)&_mat;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::set
//       Access: Public, Virtual
//  Description: The Joinable input function.  Does nothing.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
set(void *) {
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricCurveFlyer::recompute_position
//       Access: Protected
//  Description: Computes the position of the flyer according to 
//               the current time.  Will stop motion if the current
//               time has extended beyond the end of the curve and
//               loop mode is not set.  Sets _mat and _coord
//               appropriately.
////////////////////////////////////////////////////////////////////
void ParametricCurveFlyer::
recompute_position() {
  if (!_curve->is_valid()) {
    return;
  }

  double t = get_time();
  double orig_t = t;
  double max_t = get_max_t();
  if (t >= max_t) {
    if (_loop_flag) {
      // If we're looping, we need to set time modulo max_t.  In
      // floating-point arithmetic, this must be done thusly:
      t /= max_t;    // Scale [0,max_t) to the range [0,1)
      t -= floor(t);  // Now get just the fractional part
      t *= max_t;    // And scale back up.
    } else {
      // If we're not looping, clamp.
      t = max_t;
    }
  }
  
  VecType point, tangent;
  if (_time_curve!=NULL && _time_curve->is_valid()) {
    _time_curve->get_point(t, point);
    t = point[0];
  }
  _curve->get_pt(t, point, tangent);

  _coord.xyz.set(point[0], point[1], point[2]);
  _coord.hpr.set(0.0, 0.0, 0.0);

  if (_face_forward) {
    if (tangent.dot(tangent) > 0.0) {
      // Recompute the hpr if the tangent is not zero.
      LMatrix4f mat;
      look_at(mat, tangent, LVector3f(0.0, 0.0, 1.0));
      pfCoord c;
      mat.getOrthoCoord(&c);
      _h = c.hpr[0];
      _p = c.hpr[1];
    }
    _coord.hpr.set(_h, _p, 0.0);
  }

  if (_hpr_curve!=NULL && _hpr_curve->is_valid()) {
    // Now apply the hpr curve.
    VecType hprw;
    _hpr_curve->get_point(t, hprw);
    _coord.hpr += LVector3f(hprw[0], hprw[1], hprw[2]);
  }
    
  _mat.makeCoord(&_coord);

  if (!_stopped && orig_t >= max_t && !_loop_flag) {
    stop();
    set_time(max_t);
    if (_stop_event!=NULL) {
      Event temp(_stop_event);
    }
  }
}
