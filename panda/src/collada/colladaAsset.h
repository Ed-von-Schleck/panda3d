// Filename: colladaAsset.h
// Created by: Xidram (13Apr10)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef COLLADAASSET_H
#define COLLADAASSET_H

#include "typedReferenceCount.h"
#include "colladaContributor.h"
#include "colladaElement.h"
#include "config_collada.h"
#include "pvector.h"
#include "pointerTo.h"
#include "coordinateSystem.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaAsset
// Description : Object that represents the <asset> COLLADA element.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaAsset : public ColladaElement {
PUBLISHED:
  virtual void clear();

  INLINE const string &get_title() const;
  INLINE void set_title(const string &title);

  INLINE const string &get_subject() const;
  INLINE void set_subject(const string &subject);

  INLINE const string &get_keywords() const;
  INLINE void set_keywords(const string &keywords);

  INLINE const string &get_revision() const;
  INLINE void set_revision(const string &revision);

  enum AltitudeMode {
    AM_absolute,
    AM_relativeToGround
  };

  INLINE bool has_geographic_location() const;
  INLINE double get_longitude() const;
  INLINE double get_latitude() const;
  INLINE double get_altitude() const;
  INLINE AltitudeMode get_altitude_mode() const;
  INLINE void set_geographic_location(double longitude, double latitude,
    double altitude, AltitudeMode altitude_mode);
  INLINE void clear_geographic_location();

  INLINE bool has_unit() const;
  INLINE double get_unit_meter() const;
  INLINE const string get_unit_name() const;
  INLINE void set_unit(double unit_meter, const string &unit_name);
  INLINE void clear_unit();

  INLINE CoordinateSystem get_coordinate_system() const;

  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

private:
  string _title;
  string _subject;
  string _keywords;
  string _revision;

  bool _has_geographic_location;
  double _longitude;
  double _latitude;
  double _altitude;
  AltitudeMode _altitude_mode;

  bool _has_unit;
  double _unit_meter;
  string _unit_name;

  CoordinateSystem _coordsys;

  struct tm _created;
  struct tm _modified;

  //TODO: add accessors and mutators for _contributors
  pvector<PT(ColladaContributor)> _contributors;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaAsset",
                  ColladaElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaAsset.I"

#endif

