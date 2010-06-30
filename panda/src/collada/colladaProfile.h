// Filename: colladaProfile.h
// Created by: Xidram (20Apr10)
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

#ifndef COLLADAPROFILE_H
#define COLLADAPROFILE_H

#include "colladaAssetElement.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaProfile
// Description : Base class for all the different COLLADA profiles.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaProfile : public ColladaAssetElement {
PUBLISHED:
  enum Profile {
    P_common,
    P_bridge,
    P_cg,
    P_gles,
    P_gles2,
    P_glsl
  };

  INLINE ColladaProfile(Profile profile);

  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  static Profile string_profile(const string &string);

private:
  Profile _profile;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaAssetElement::init_type();
    register_type(_type_handle, "ColladaProfile",
                  ColladaAssetElement::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

EXPCL_COLLADA ostream &operator << (ostream &out, ColladaProfile::Profile profile);
EXPCL_COLLADA istream &operator >> (istream &in, ColladaProfile::Profile &profile);

#include "colladaProfile.I"

#endif

