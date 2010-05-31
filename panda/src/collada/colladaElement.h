// Filename: colladaElement.h
// Created by: Xidram (15Apr10)
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

#ifndef COLLADAELEMENT_H
#define COLLADAELEMENT_H

#include "config_collada.h"
#include "colladaDocument.h"
#include "pointerTo.h"
#include "typedReferenceCount.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaElement
// Description : Object that represents the <element> COLLADA tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaElement : public TypedReferenceCount {
PUBLISHED:
  INLINE ColladaElement();
  virtual ~ColladaElement() {};

  INLINE virtual void clear();
  virtual bool load_xml(const TiXmlElement *element);
  virtual TiXmlElement *make_xml() const;

  INLINE void set_name(const string &name);
  INLINE void clear_name();
  INLINE bool has_name() const;
  INLINE const string &get_name() const;

  INLINE void set_id(const string &id);
  INLINE void clear_id();
  INLINE bool has_id() const;
  INLINE const string &get_id() const;

  INLINE PT(ColladaElement) get_parent() const;
  INLINE virtual PT(ColladaDocument) get_document() const;
  INLINE virtual PT(ColladaElement) get_element_by_id(const string &id) const;

protected:
  INLINE void attach(ColladaElement *child);
  INLINE void detach(ColladaElement *child) const;

public:
  PT(ColladaElement) _parent;

protected:
  string _name;
  string _id;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaElement",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "colladaElement.I"

#endif

