// Filename: tFlyer.C
// Created by:  cary (21Apr97)
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

////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////

#include "parametrics.h"
#include "tFlyer.h"
#include <math.h>
#include <event.h>
#include <linMathOutput.h>
#include <DConfig.h>
#include <DNotify.h>

DNCategory dntflysrc = { "tFlyerSource", DNTRUE, &dnall };
DNCategory dntflytgt = { "tFlyerTarget", DNTRUE, &dnall };

Configure( tFlyer );
EnvSpecs( tFlyer ) = DC_NOENV;
ArgSpecs( tFlyer ) = {
  {"epsilon", "nearly zero (0.00001)"},
  {"play_rate", "play rate relative to frame rate (1.0)"},
  {"stop_event", "event to throw on stop (NULL)"},
  {"loop_flag", "loop over paths (0)"},
  {"max_t", "max_t to use without curve (-1.0)"},
  {"max_extra_time", "max allowed extra_time bump (-1.)"},
  {"min_extra_time", "min allowed extra_time bump (-1.)"},
  {"max_delta_extra_time", "max allowed change in extra_time (-1.)"},
  {"real-time", "should we start out running off the real clock (1)"},
  {"framerate", "when not running realtime, how fast are we going? (30.)"},
  {NULL},
};
ScanFiles( tFlyer ) = DC_SCAN;
FilePrefix( tFlyer ) = NULL;

ConfigFn( tFlyer )
{
   DNRegister(dntflysrc);
   DNRegister(dntflytgt);
}

const float epsilon = 1. - tFlyer.GetFloat("epsilon", 0.00001);

typedef tFlyEventMap::iterator tFlyEventMapIter;

////////////////////////////////////////////////////////////////////
//     Function: tFlyerSource::Constructor
//  Description:
////////////////////////////////////////////////////////////////////
tFlyerSource* make_tFlyerSource( void )
{
    return ( new tFlyerSource() );
}

tFlyerSource* make_tFlyerSource( ParametricCurve* curve )
{
    return ( new tFlyerSource( curve ) );
}

void rm_tFlyerSource( tFlyerSource* source )
{
    delete source;
}

////////////////////////////////////////////////////////////////////
//     Function: tFlyerSource::Constructor
//       Access: Public, Scheme
//  Description: construct a tFlyer possibly with a time-warp curve
////////////////////////////////////////////////////////////////////
tFlyerSource::tFlyerSource() {
   _curve = NULL;
   _play_rate = tFlyer.GetDouble("play_rate", 1.0);
   _gmax_t = tFlyer.GetDouble("max_t", -1.0);
   _start_time = 0.0;
   _extra_time = 0.0;
   _cum_extra_time = 0.0;
   _stop_event = (tFlyer.GetString("stop_event", NULL) == NULL) ? NULL : strdup(tFlyer.GetString("stop_event", NULL));
   _loop_flag = tFlyer.GetInt("loop_flag", false);
   _stopped = true;
   _real_time = tFlyer.GetInt("real-time", true);
   _fake_framerate = tFlyer.GetFloat("framerate", 30.);
   _recip_framerate = 1. / _fake_framerate;
   _max_extra_time = tFlyer.GetFloat("max_extra_time", -1.);
   _min_extra_time = tFlyer.GetFloat("min_extra_time", -1.);
   _max_delta_extra_time = tFlyer.GetFloat("max_delta_extra_time", -1.);
   _prev_extra_time = 0.;
}

tFlyerSource::tFlyerSource(ParametricCurve *curve) {
   _curve = curve;
   _play_rate = tFlyer.GetDouble("play_rate", 1.0);
   _gmax_t = tFlyer.GetDouble("max_t", -1.0);
   _start_time = 0.0;
   _extra_time = 0.0;
   _cum_extra_time = 0.0;
   _stop_event = (tFlyer.GetString("stop_event", NULL) == NULL) ? NULL : strdup(tFlyer.GetString("stop_event", NULL));
   _loop_flag = tFlyer.GetInt("loop_flag", false);
   _stopped = true;
   _real_time = tFlyer.GetInt("real-time", true);
   _fake_framerate = tFlyer.GetFloat("framerate", 30.);
   _recip_framerate = 1. / _fake_framerate;
   _max_extra_time = tFlyer.GetFloat("max_extra_time", -1.);
   _min_extra_time = tFlyer.GetFloat("min_extra_time", -1.);
   _max_delta_extra_time = tFlyer.GetFloat("max_delta_extra_time", -1.);
   _prev_extra_time = 0.;
}

////////////////////////////////////////////////////////////////////
//     Function: tFlyerSource::Destructor
//       Access: Public, Scheme
//  Description: destruct all local data
////////////////////////////////////////////////////////////////////

tFlyerSource::~tFlyerSource() {
   if (_stop_event != NULL) {
      delete[] _stop_event;
   }
}

void tFlyerSource::set_curve(ParametricCurve *curve) {
   _curve = curve;
}

ParametricCurve *tFlyerSource::get_curve() {
   return _curve;
}

void tFlyerSource::start() {
   if (_stopped) {
      _start_time = 0.0;
      cont();
   } else {
      // Reposition at begining
      _start_time = _real_time ? pfGetFrameTimeStamp() : pfGetFrameCount();
      _start_time *= _real_time ? 1. : _recip_framerate ;
   }
   _cum_extra_time = 0.0;
   _prev_extra_time = 0.0;
   _current = -1.0;
}

void tFlyerSource::stop() {
   if (!_stopped) {
      double now = _real_time ? pfGetFrameTimeStamp() : pfGetFrameCount();
      now *= _real_time ? 1. : _recip_framerate ;
      _start_time = now - _start_time;
      _stopped = true;
   }
}

void tFlyerSource::cont() {
   if (_stopped) {
      double now = _real_time ? pfGetFrameTimeStamp() : pfGetFrameCount();
      now *= _real_time ? 1. : _recip_framerate ;
      _start_time = now - _start_time;
      _stopped = false;
   }
}

bool tFlyerSource::playing() const {
   return !_stopped;
}

void tFlyerSource::set_loop(bool flag) {
   _loop_flag = flag;
}

bool tFlyerSource::get_loop() const {
   return _loop_flag;
}

void tFlyerSource::set_time(double time) {
   if (_stopped) {
      _start_time = time / _play_rate;
   } else {
      double now = _real_time ? pfGetFrameTimeStamp() : pfGetFrameCount();
      now *= _real_time ? 1. : _recip_framerate ;
      _start_time = now - time / _play_rate;
   }
   _cum_extra_time = 0.0;
   _prev_extra_time = 0.0;
}

double tFlyerSource::get_time() const {
   double now = _real_time ? pfGetFrameTimeStamp() : pfGetFrameCount();
   now *= _real_time ? 1. : _recip_framerate ;
   double t = _stopped ? _start_time : now - _start_time;
   t *= _play_rate;
   return t + _cum_extra_time;
}

void tFlyerSource::set_extra_time(double time) {
   _extra_time = time;
   if (_max_extra_time >= 0.) {
      _extra_time = (_extra_time > _max_extra_time) ? _max_extra_time :
                                                      _extra_time;
   }

   if (_min_extra_time >= 0.) {
      _extra_time = (_extra_time < _min_extra_time) ? _min_extra_time :
                                                      _extra_time;
   }

   if (_max_delta_extra_time >= 0.) {
      double d = fabs(_prev_extra_time - _extra_time);
      if (d > _max_delta_extra_time) {
	 if (_prev_extra_time < _extra_time) {
	    _extra_time = _prev_extra_time + _max_delta_extra_time;
	 } else {
	    _extra_time = _prev_extra_time - _max_delta_extra_time;
	 }
      }
   }
}

double tFlyerSource::get_extra_time() const {
   return _extra_time;
}

double tFlyerSource::get_prev_extra_time() const {
   return _prev_extra_time;
}


////////////////////////////////////////////////////////////////////
//     Function: tFlyerSource::set_real_time
//       Access: Public, Scheme
//  Description: In real-time mode, the tFlyer advances according to
//               the amount of time that has elapsed, irrespective of
//               the frame rate.  This is the default mode.  In
//               real-time mode, set_play_rate() specifies the units
//               of tFlyer time that elapse per second.
//
//               When real-time mode is disabled via this function,
//               the tFlyer advances according to the number of frames
//               that are drawn--the speed of the tFlyer is tied to
//               the frame rate.  In non-real-time mode,
//               set_play_rate() specifies the units of tFlyer time
//               that elapse per frame.
////////////////////////////////////////////////////////////////////
void tFlyerSource::set_real_time(bool flag) {
   double t = get_time();
   _real_time = flag;
   set_time(t);
}

bool tFlyerSource::get_real_time() const {
   return _real_time;
}

void tFlyerSource::set_play_rate(double rate) {
   double t = get_time();
   _play_rate = rate;
   set_time(t);
}

double tFlyerSource::get_play_rate() const {
   return _play_rate;
}

void tFlyerSource::set_fake_framerate(double rate) {
   _fake_framerate = rate;
   _recip_framerate = 1. / rate;
}

double tFlyerSource::get_fake_framerate() const {
   return _fake_framerate;
}

void tFlyerSource::set_gmax_t(double time) {
   _gmax_t = time;
}

double tFlyerSource::get_gmax_t() const {
   return _gmax_t;
}

void tFlyerSource::set_max_extra_time(double time) {
   _max_extra_time = time;
}

double tFlyerSource::get_max_extra_time() const {
   return _max_extra_time;
}

void tFlyerSource::set_min_extra_time(double time) {
   _min_extra_time = time;
}

double tFlyerSource::get_min_extra_time() const {
   return _min_extra_time;
}


void tFlyerSource::set_max_delta_extra_time(double time) {
   _max_delta_extra_time = time;
}

double tFlyerSource::get_max_delta_extra_time() const {
   return _max_delta_extra_time;
}

void tFlyerSource::set_stop_event(const char *str) {
   if (_stop_event != NULL) {
      delete[] _stop_event;
      _stop_event = NULL;
   }
   if (str != NULL) {
      _stop_event = new char[strlen(str)+1];
      strcpy(_stop_event, str);
   }
}

void tFlyerSource::set_time_event(double t, const char *str)
{
   times[t] = str;
}

void tFlyerSource::show_time_events(void)
{
   for (tFlyEventMapIter i=times.begin(); i!=times.end(); ++i) {
      DINFO(dntflysrc) << (*i).first << " " << (*i).second << dnend;
   }
}

void tFlyerSource::remove_time_events(const char *str)
{
   tFlyEventMapIter i=times.begin();
   while (i!=times.end()) {
     if ((*i).second == str) {
       //       DDEBUG(dntflysrc) << "found event: '" << (*i).second << "' = '"
       //			 << str << "' at time: " << (*i).first << dnend;
       tFlyEventMapIter j = i;
       ++j;
       times.erase(i);
       i=j;
     } else {
       ++i;
     }
   }
}

void tFlyerSource::remove_time_events(void)
{
   times.erase(times.begin(), times.end());
}

//////////////////////////////////////////////////////////

void *tFlyerSource::get() {
  recompute_position();
  return (void *)&_current;
}

void tFlyerSource::set(void*) {
   DWARNING(dntflysrc) << "call to set!" << dnend;
}

void tFlyerSource::recompute_position() {
   double t = get_time();
   double max_t = get_max_t();
   double pre_time = _current;
   VecType point, tangent;
   if (_extra_time != 0.) {
      if (_curve != NULL && _curve->is_valid()) {
	 _curve->get_pt(t, point, tangent);
	 // this next line could be a problem if the tangent ever goes to 0.
	 t += _extra_time / tangent[0];
	 _cum_extra_time += _extra_time / tangent[0];
      } else {
	 t += _extra_time;
	 _cum_extra_time += _extra_time;
      }
      _prev_extra_time = _extra_time;
      _extra_time = 0.;
   }
   if ((max_t >= 0.) && (t >= max_t)) {
      if (_loop_flag) {
	 t /= max_t;
	 t -= floor(t);
	 t *= max_t;
      } else {
	 t = max_t;
	 if (!_stopped) {
	    stop();
	    set_time(max_t);
	    if (_stop_event != NULL) {
	       Event temp(_stop_event);
	    }
	 }
      }
   }
   if (_curve != NULL && _curve->is_valid()) {
      _curve->get_point(t, point);
      t = point[0];
   }
   _current = t;
   for (tFlyEventMapIter i=times.begin(); i!=times.end(); ++i) {
      if ((pre_time < (*i).first) && (_current >= (*i).first)) {
	 Event temp((*i).second.c_str());
      }
   }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

tFlyerAdaptor::tFlyerAdaptor() : Joinable(), _current(0.)
{
}

tFlyerAdaptor::~tFlyerAdaptor()
{
}

void *tFlyerAdaptor::get()
{
   return (void*)&_current;
}

void tFlyerAdaptor::set(void *data)
{
   _current = *((double*)data);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

tFlyerOffset* make_tFlyerOffset(double os)
{
  return ( new tFlyerOffset( os ) );
}

tFlyerOffset* make_tFlyerOffset(double os, double gmax)
{
  return ( new tFlyerOffset( os, gmax ) );
}

void rm_tFlyerOffset( tFlyerOffset* offset )
{
  delete offset;
}


tFlyerOffset::tFlyerOffset(double os, double gmax) :
  tFlyerAdaptor(), _offset(os), _gmax_t(gmax)
{
}

tFlyerOffset::~tFlyerOffset()
{
}

double tFlyerOffset::get_offset() const
{
   return _offset;
}

void tFlyerOffset::set_offset(double os)
{
   _offset = os;
}


////////////////////////////////////////////////////////////////////
//     Function: tFlyerOffset::set_gmax_t
//       Access: Public, Scheme
//  Description: Sets the range length for a tFlyerOffset.  When
//               gmax_t is zero, the default, the offset knows nothing
//               about range: it simply adds its offset to its input,
//               and passes it along.  When gmax_t is set nonzero, it
//               indicates the valid range into which the input must
//               be constrained.  After adding the offset, the input
//               value is taken modulo gmax_t, to produce a number in
//               the range [0, gmax_t).
////////////////////////////////////////////////////////////////////
void tFlyerOffset::set_gmax_t(double time) {
   _gmax_t = time;
}

double tFlyerOffset::get_gmax_t() const {
   return _gmax_t;
}

void tFlyerOffset::set(void *data)
{
   tFlyerAdaptor::set(data);
   if (_gmax_t <= 0.0) {
     _current += _offset;
   } else {
     _current = fmod(_current + _offset, _gmax_t);
   }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

tFlyerWarp* make_tFlyerWarp(ParametricCurve *curve)
{
  return ( new tFlyerWarp( curve ) );
}

void rm_tFlyerWarp( tFlyerWarp* warp ) 
{
  delete warp;
}


tFlyerWarp::tFlyerWarp(ParametricCurve *curve) : tFlyerAdaptor(), _curve(curve)
{
  _last_data = 0.0;
  _changed = true;
}

tFlyerWarp::~tFlyerWarp()
{
}

ParametricCurve *tFlyerWarp::get_curve()
{
   return _curve;
}

void tFlyerWarp::set_curve(ParametricCurve *curve)
{
   _curve = curve;
   _changed = true;
}

void tFlyerWarp::set(void *data)
{
   LVector3f point;

   double next_data = *(double *)data;
   if (_changed || next_data != _last_data) {
     _changed = false;
     _last_data = next_data;
     if (_curve != NULL && _curve->is_valid()) {
       _curve->get_point(next_data, point);
       _current = point[0];
     }
   }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//     Function: tFlyerTarget::Constructor
//  Description:
////////////////////////////////////////////////////////////////////
tFlyerTarget* make_tFlyerTarget(ParametricCurve *curve)
{
    return ( new tFlyerTarget( curve ) );
}

tFlyerTarget* make_tFlyerTarget(ParametricCurve *curve, ParametricCurve *hpr)
{
    return ( new tFlyerTarget( curve, hpr ) );
}

void rm_tFlyerTarget( tFlyerTarget* target )
{
    delete target;
}

////////////////////////////////////////////////////////////////////
//     Function: tFlyerTarget::Constructor
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
tFlyerTarget::tFlyerTarget(ParametricCurve *curve) {
   _curve = curve;
   _hpr_curve = NULL;
   _face_forward = false;
   _h = _p = 0.;
   recompute_position(0.);
}

tFlyerTarget::tFlyerTarget(ParametricCurve *curve, ParametricCurve *hpr) {
   _curve = curve;
   _hpr_curve = hpr;
   _face_forward = false;
   _h = _p = 0.;
   recompute_position(0.);
}

////////////////////////////////////////////////////////////////////
//     Function: tFlyerTarget::Destructor
//       Access: Public, Scheme
//  Description: destruct all local data
////////////////////////////////////////////////////////////////////

tFlyerTarget::~tFlyerTarget() {
}

void tFlyerTarget::set_curve(ParametricCurve *curve) {
   _curve = curve;
   _changed = true;
}

ParametricCurve *tFlyerTarget::get_curve() {
   return _curve;
}

void tFlyerTarget::set_hpr_curve(ParametricCurve *curve) {
   _hpr_curve = curve;
   _changed = true;
}

ParametricCurve *tFlyerTarget::get_hpr_curve() {
   return _hpr_curve;
}

void tFlyerTarget::set_face_forward(bool flag) {
   _face_forward = flag;
   _changed = true;
}

bool tFlyerTarget::get_face_forward() const {
   return _face_forward;
}

void tFlyerTarget::get_matrix(LMatrix4f &mat) {
   mat = _mat;
}

void tFlyerTarget::get_coord(LVector3f &xyz, LVector3f &hpr) {
   xyz = _coord.xyz;
   hpr = _coord.hpr;
}

//////////////////////////////////////////////////////////

void *tFlyerTarget::get() {
  //   DDEBUG(dntflytgt) << "in get()" << dnend;
   return (void *)&_mat;
}

void tFlyerTarget::set(void *data) {
  //   DDEBUG(dntflytgt) << "in set()" << dnend;
  //   DDEBUG(dntflytgt) << "getting set with: " << *((double*)data) << dnend;
   double next_data = *(double *)data;
   if (_changed || next_data != _last_data) {
     recompute_position(next_data);
   }
}

void tFlyerTarget::recompute_position(double time) {
   if (!_curve->is_valid()) {
      return;
   }
   double max_t = get_max_t();
   if (time >= max_t) {
      time = max_t;
   }
   VecType point, tangent;
   _curve->get_pt(time, point, tangent);
   _coord.xyz.set(point[0], point[1], point[2]);
   _coord.hpr.set(0., 0., 0.);
   if (_face_forward) {
      LVector3f tn_xyz(tangent[0], tangent[1], tangent[2]);
      float d2 = tn_xyz.dot(tn_xyz);
      float dot;
      if (d2 > 0.) {
	 tn_xyz /= sqrt(d2);
	 pfVec2 tn_xy(tangent[0], tangent[1]);
	 d2 = tn_xy.dot(tn_xy);
	 if (d2 > 0.) {
	    tn_xy /= sqrt(d2);
	    dot = tn_xy.dot(pfVec2(0., 1.));
	    if (dot >= -epsilon & dot <= epsilon) {
	       _h = pfArcCos(dot);
	       if (tangent[0] >= 0.) {
		  _h = -_h;
	       }
	    } else if (dot < 0.) {
	       _h = (tangent[0] >= 0.) ? -180. : 180.;
	    }
	 }
	 dot = tn_xyz.dot(LVector3f(tn_xy[0], tn_xy[1], 0.));
	 if (dot <= epsilon) {
	    _p = pfArcCos(dot);
	    if (tangent[2] < 0.) {
	       _p = -_p;
	    }
	 }
      }
      _coord.hpr.set(_h, _p, 0.);
   }
   if (_hpr_curve != NULL && _hpr_curve->is_valid()) {
      VecType hprw;
      _hpr_curve->get_point(time, hprw);
      _coord.hpr += LVector3f(hprw[0], hprw[1], hprw[2]);
   }
   _mat.makeCoord(&_coord);
   //   DDEBUG(dntflytgt) << "position at time (" << time << "): {" << _coord.xyz[0] << ", " << _coord.xyz[1] << ", " << _coord.xyz[2] << "}" << dnend;
   _changed = false;
   _last_data = time;
}
