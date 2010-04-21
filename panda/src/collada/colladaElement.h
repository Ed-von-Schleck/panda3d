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

class ColladaDocument;

////////////////////////////////////////////////////////////////////
//       Class : ColladaElement
// Description : Object that represents the <element> COLLADA tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaElement {
PUBLISHED:
  INLINE ColladaElement();

  INLINE virtual void clear();
  INLINE virtual bool load_xml(const TiXmlElement *element);
  INLINE virtual TiXmlElement *make_xml() const;

  INLINE void set_name(const string &name);
  INLINE void clear_name();
  INLINE bool has_name() const;
  INLINE const string &get_name() const;

  INLINE void set_id(const string &id);
  INLINE void clear_id();
  INLINE bool has_id() const;
  INLINE const string &get_id() const;

protected:
  INLINE void attach(ColladaElement *child) const;
  INLINE void detach(ColladaElement *child) const;

public:
  const ColladaElement *_parent;

private:
  string _name;
  string _id;

};

#include "colladaElement.I"

#endif

