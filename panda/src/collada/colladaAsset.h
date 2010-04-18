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
#include "colladaElement.h"
#include "config_collada.h"
#include "coordinateSystem.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaAsset
// Description : Object that represents the <asset> COLLADA tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaAsset : public ColladaElement, public TypedReferenceCount {
PUBLISHED:
  ColladaAsset();
  INLINE virtual void clear();

  INLINE const string &get_title() const;
  INLINE void set_title(const string &title);

  INLINE const string &get_subject() const;
  INLINE void set_subject(const string &subject);

  INLINE const string &get_keywords() const;
  INLINE void set_keywords(const string &keywords);

  INLINE const string &get_revision() const;
  INLINE void set_revision(const string &revision);

  INLINE CoordinateSystem get_coordinate_system() const;

  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

private:
  string _title;
  string _subject;
  string _keywords;
  string _revision;

  CoordinateSystem _coordsys;

  struct tm _created;
  struct tm _modified;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaAsset",
                  TypedReferenceCount::get_class_type());
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

