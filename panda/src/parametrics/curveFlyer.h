// Filename: curveFlyer.h
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
#ifndef CURVEFLYER_H
#define CURVEFLYER_H
//
////////////////////////////////////////////////////////////////////
// Includes 
////////////////////////////////////////////////////////////////////

#include "curve.h"
#include <joinable.h>
#include <Performer/pr/pfLinMath.h>

////////////////////////////////////////////////////////////////////
// Salivate interface
////////////////////////////////////////////////////////////////////
/*$ 
#typehint boolean int
#typehint VecType pfVec3

#exportclass ParametricCurveFlyer
#exportfunc make_ParametricCurveFlyer
#exportfunc rm_ParametricCurveFlyer

#exportmember ParametricCurveFlyer set_curve
#exportmember ParametricCurveFlyer get_curve
#wrappermember pf-downcast-null-convert ParametricCurveFlyer get_curve
#exportmember ParametricCurveFlyer set_hpr_curve get_hpr_curve
#wrappermember pf-downcast-null-convert ParametricCurveFlyer get_hpr_curve
#exportmember ParametricCurveFlyer set_time_curve get_time_curve
#wrappermember pf-downcast-null-convert ParametricCurveFlyer get_time_curve
#exportmember ParametricCurveFlyer start stop cont
#exportmember ParametricCurveFlyer playing
#exportmember ParametricCurveFlyer set_loop get_loop
#exportmember ParametricCurveFlyer set_face_forward get_face_forward
#exportmember ParametricCurveFlyer set_time get_time
#exportmember ParametricCurveFlyer set_play_rate get_play_rate
#exportmember ParametricCurveFlyer set_stop_event
#exportmember ParametricCurveFlyer get_matrix get_coord
$*/


////////////////////////////////////////////////////////////////////
// 	 Class : ParametricCurveFlyer
// Description : Flies a DCS smoothly along a parametric curve, using
//               w as the time dimension.
////////////////////////////////////////////////////////////////////
class ParametricCurveFlyer : public Joinable {

////////////////////////////////////////////////////////////////////
// Member functions visible to Scheme
////////////////////////////////////////////////////////////////////

public:
  ParametricCurveFlyer(ParametricCurve *curve);
  virtual ~ParametricCurveFlyer();

  void set_curve(ParametricCurve* curve);
  ParametricCurve *get_curve();

  void set_hpr_curve(ParametricCurve *curve);
  ParametricCurve *get_hpr_curve();

  void set_time_curve(ParametricCurve *curve);
  ParametricCurve *get_time_curve();

  void start();
  void stop();
  void cont();

  boolean playing() const;

  void set_loop(boolean flag);
  boolean get_loop() const;

  void set_face_forward(boolean flag);
  boolean get_face_forward() const;

  void set_time(double time);
  double get_time() const;

  void set_play_rate(double rate);
  double get_play_rate() const;

  void set_stop_event(const char *string);

  void get_matrix(pfMatrix &mat);
  void get_coord(pfVec3 &xyz, pfVec3 &hpr);
  

////////////////////////////////////////////////////////////////////
// Member functions not visible to Scheme
////////////////////////////////////////////////////////////////////
public:
  double get_max_t() const {
    return _time_curve==NULL ? _curve->get_max_t() : _time_curve->get_max_t();
  }

  virtual const char *ClassName() const { return "ParametricCurveFlyer"; }
  virtual void *get();
  virtual void set(void *data);


protected:
  void recompute_position();


  ParametricCurve *_curve, *_hpr_curve, *_time_curve;
  double _play_rate;
  char *_stop_event;
  boolean _loop_flag;
  boolean _face_forward;
  double _start_time;
  boolean _stopped;

  pfMatrix _mat;
  pfCoord _coord;
  double _h, _p;
};

ParametricCurveFlyer* make_ParametricCurveFlyer( ParametricCurve* curve );
void rm_ParametricCurveFlyer( ParametricCurveFlyer* pcf );


#endif
