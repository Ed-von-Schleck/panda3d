// Filename: tFlyer.h
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

#ifndef __TFLYER_H__
#define __TFLYER_H__

////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curve.h"
#include <joinable.h>
#include <Performer/pr/pfLinMath.h>
#include <map.h>
#include <function.h>
#include <mstring.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////

/*$ 

#typehint bool int
#typehint VecType LVector3f

#exportclass tFlyerSource
#exportfunc make_tFlyerSource
#exportfunc rm_tFlyerSource
#exportmember tFlyerSource set_curve get_curve start stop cont playing
#exportmember tFlyerSource set_loop get_loop set_time get_time
#exportmember tFlyerSource set_extra_time get_extra_time get_prev_extra_time
#exportmember tFlyerSource set_real_time get_real_time
#exportmember tFlyerSource set_play_rate get_play_rate set_stop_event
#exportmember tFlyerSource set_fake_framerate get_fake_framerate
#exportmember tFlyerSource set_gmax_t get_gmax_t
#exportmember tFlyerSource set_max_extra_time get_max_extra_time
#exportmember tFlyerSource set_min_extra_time get_min_extra_time
#exportmember tFlyerSource set_max_delta_extra_time get_max_delta_extra_time
#exportmember tFlyerSource set_time_event remove_time_events
#exportmember tFlyerSource show_time_events

#exportclass tFlyerAdaptor

#exportclass tFlyerOffset
#exportfunc make_tFlyerOffset
#exportfunc rm_tFlyerOffset
#exportmember tFlyerOffset set_offset get_offset
#exportmember tFlyerOffset set_gmax_t get_gmax_t

#exportclass tFlyerWarp
#exportfunc make_tFlyerWarp
#exportfunc rm_tFlyerWarp
#exportmember tFlyerWarp set_curve get_curve

#exportclass tFlyerTarget
#exportfunc make_tFlyerTarget
#exportfunc rm_tFlyerTarget
#exportmember tFlyerTarget set_curve get_curve set_hpr_curve get_hpr_curve
#exportmember tFlyerTarget set_face_forward get_face_forward
#exportmember tFlyerTarget get_matrix get_coord

$*/

typedef map<double, string, less<double> > tFlyEventMap;

////////////////////////////////////////////////////////////////////
// 	 Class : tFlyerSource
// Description : optionally applies a time-warp to t.  Marches t along
//               per-frame
////////////////////////////////////////////////////////////////////
class tFlyerSource : public Joinable {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

   public:

      void set_curve(ParametricCurve *curve);
      ParametricCurve *get_curve();

      void start();
      void stop();
      void cont();

      bool playing() const;

      void set_loop(bool flag);
      bool get_loop() const;

      void set_time(double time);
      double get_time() const;

      void set_extra_time(double time);
      double get_extra_time() const;
      double get_prev_extra_time() const;

      void set_real_time(bool flag);
      bool get_real_time() const;

      void set_play_rate(double rate);
      double get_play_rate() const;

      void set_fake_framerate(double rate);
      double get_fake_framerate() const;

      void set_gmax_t(double time);
      double get_gmax_t() const;

      void set_max_extra_time(double time);
      double get_max_extra_time() const;

      void set_min_extra_time(double time);
      double get_min_extra_time() const;

      void set_max_delta_extra_time(double time);
      double get_max_delta_extra_time() const;

      void set_stop_event(const char *str);

      void set_time_event(double t, const char *str);
      void show_time_events(void);
      void remove_time_events(const char *str);
      void remove_time_events(void);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
   public:

      tFlyerSource();
      tFlyerSource(ParametricCurve *curve);
      virtual ~tFlyerSource();

      virtual const char *ClassName() const { return "tFlyerSource"; }
      virtual void *get();
      virtual void set(void *data);

   protected:
      void   recompute_position();
      inline double get_max_t() const {
	 return (_curve == NULL) ? _gmax_t : _curve->get_max_t() ;
      }

      ParametricCurve *_curve;
      tFlyEventMap times;
      double  _play_rate;
      double  _gmax_t;
      double  _start_time;
      double  _extra_time;
      double  _prev_extra_time;
      double  _max_extra_time;
      double  _min_extra_time;
      double  _max_delta_extra_time;
      double  _cum_extra_time;
      double  _current;
      double  _fake_framerate;
      double  _recip_framerate;
      char   *_stop_event;
      bool _loop_flag;
      bool _stopped;
      bool _real_time;
};

tFlyerSource* make_tFlyerSource( void );
tFlyerSource* make_tFlyerSource( ParametricCurve* curve );
void rm_tFlyerSource( tFlyerSource* source );

////////////////////////////////////////////////////////////////////
// 	 Class : tFlyerAdaptor
// Description : modifies the time comming out of a tFlyerSource in
//               some way
////////////////////////////////////////////////////////////////////
class tFlyerAdaptor : public Joinable {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

   public:
      tFlyerAdaptor();
      virtual ~tFlyerAdaptor();

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
   public:
      virtual const char *ClassName() const { return "tFlyerAdaptor"; }
      virtual void *get();
      virtual void set(void *data);

   protected:
      double  _current;
};

////////////////////////////////////////////////////////////////////
// 	 Class : tFlyerOffset
// Description : offset the time from a tFlyerSource by some amount
////////////////////////////////////////////////////////////////////
class tFlyerOffset : public tFlyerAdaptor {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

   public:
      tFlyerOffset(double os, double gmax = 0.0);
      virtual ~tFlyerOffset();

      double get_offset() const;
      void   set_offset(double os);

      void set_gmax_t(double time);
      double get_gmax_t() const;

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
   public:
      virtual void set(void *data);

   protected:
      double  _offset;
      double  _gmax_t;
};

tFlyerOffset* make_tFlyerOffset( double os );
tFlyerOffset* make_tFlyerOffset( double os, double gmax );
void rm_tFlyerOffset( tFlyerOffset* offset );


////////////////////////////////////////////////////////////////////
// 	 Class : tFlyerWarp
// Description : offset the time from a tFlyerSource by some amount
////////////////////////////////////////////////////////////////////
class tFlyerWarp : public tFlyerAdaptor {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

   public:
      tFlyerWarp(ParametricCurve *curve);
      virtual ~tFlyerWarp();

      ParametricCurve *get_curve();
      void             set_curve(ParametricCurve *curve);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
   public:
      virtual const char *ClassName() const { return "tFlyerWarp"; }
      virtual void set(void *data);

   protected:
      ParametricCurve *_curve;
      double _last_data;
      bool _changed;
};


tFlyerWarp* make_tFlyerWarp( ParametricCurve *curve );
void rm_tFlyerWarp( tFlyerWarp* warp );


////////////////////////////////////////////////////////////////////
// 	 Class : tFlyerTarget
// Description : Joined to a tFlyerSource, takes the 'adjusted' time
//               and computes a matrix from it.
////////////////////////////////////////////////////////////////////
class tFlyerTarget : public Joinable {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

   public:
      tFlyerTarget(ParametricCurve *curve);
      tFlyerTarget(ParametricCurve *curve, ParametricCurve *hpr);
      virtual ~tFlyerTarget();

      void set_curve(ParametricCurve *curve);
      ParametricCurve *get_curve();

      void set_hpr_curve(ParametricCurve *curve);
      ParametricCurve *get_hpr_curve();

      void set_face_forward(bool flag);
      bool get_face_forward() const;

      void get_matrix(LMatrix4f &mat);
      void get_coord(LVector3f &xyz, LVector3f &hpr);

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
   public:
      virtual const char *ClassName() const { return "tFlyerTarget"; }
      virtual void *get();
      virtual void set(void *data);

   protected:
      void recompute_position(double time);
      inline double get_max_t() const {
	 return _curve->get_max_t();
      }

      ParametricCurve *_curve, *_hpr_curve;
      bool  _face_forward;
      double   _h, _p;
      LMatrix4f _mat;
      pfCoord  _coord;
      double _last_data;
      bool _changed;
};

tFlyerTarget* make_tFlyerTarget(ParametricCurve *curve);
tFlyerTarget* make_tFlyerTarget(ParametricCurve *curve, ParametricCurve *hpr);
void rm_tFlyerTarget( tFlyerTarget* target );

#endif /* __TFLYER_H__ */
