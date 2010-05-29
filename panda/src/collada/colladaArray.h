// Filename: colladaArray.h
// Created by:  rdb (29May10)
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

#ifndef COLLADAARRAY_H
#define COLLADAARRAY_H

#include "pandabase.h"

#include "config_collada.h"
#include "colladaElement.h"

enum ColladaArrayType {
  AT_bool,
  AT_float,
  AT_IDREF,
  AT_int,
  AT_Name,
  AT_SIDREF,
  AT_token
};

////////////////////////////////////////////////////////////////////
//       Class : ColladaArrayBase
// Description : This is the base class for arrays in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaArrayBase : public ColladaElement {
public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaArrayBase",
                  ColladaElement::get_class_type());
  }

private:
  static TypeHandle _type_handle;
};

////////////////////////////////////////////////////////////////////
//       Class : ColladaArray
// Description : This is the template class for arrays in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
class EXPCL_COLLADA ColladaArray : public ColladaArrayBase, public pvector <itemType> {
PUBLISHED:
  INLINE virtual ColladaArrayType get_array_type() const;
  INLINE virtual void clear();
  INLINE virtual void clear_array();
  INLINE virtual bool load_xml(const TiXmlElement *xelement);
  INLINE virtual TiXmlElement *make_xml() const;

  INLINE itemType string_to_item(const string &str) const;
  INLINE string item_to_string(const itemType &val) const;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type(const string &type_name = "UndefinedColladaArray") {
    ColladaArrayBase::init_type();
    _type_handle = register_dynamic_type
      (type_name, ColladaArrayBase::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    // In this case, we can't do anything, since we don't have the
    // class' type_name.
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

template<> INLINE bool ColladaArray<AT_bool, bool>::
string_to_item(const string &str) const;

// Note: when adding new types, be sure to add them to config_collada.cxx also

typedef ColladaArray<AT_bool, bool> ColladaBoolArray;
typedef ColladaArray<AT_float, double> ColladaFloatArray;
typedef ColladaArray<AT_IDREF, string> ColladaIdrefArray;
typedef ColladaArray<AT_int, int> ColladaIntArray;
typedef ColladaArray<AT_Name, string> ColladaNameArray;
typedef ColladaArray<AT_SIDREF, string> ColladaSidrefArray;
typedef ColladaArray<AT_token, string> ColladaTokenArray;

EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaBoolArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaFloatArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaIdrefArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaIntArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaNameArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaSidrefArray);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaTokenArray);

#include "colladaArray.I"

// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma interface
#endif

#endif

