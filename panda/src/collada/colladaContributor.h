// Filename: colladaContributor.h
// Created by: Xidram (18Apr10)
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

#ifndef COLLADACONTRIBUTOR_H
#define COLLADACONTRIBUTOR_H

#include "colladaElement.h"
#include <string.h>

////////////////////////////////////////////////////////////////////
//       Class : ColladaContributor
// Description : Object that represents the <contributor> COLLADA tag.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaContributor : public ColladaElement, public TypedReferenceCount {
PUBLISHED:
  virtual void clear();
  virtual bool load_xml(const TiXmlElement *xelement);
  virtual TiXmlElement *make_xml() const;

  INLINE const string &get_author() const;
  INLINE void set_author(const string &author);

  INLINE const string &get_author_email() const;
  INLINE void set_author_email(const string &author_email);

  INLINE const string &get_author_website() const;
  INLINE void set_author_website(const string &author_website);

  INLINE const string &get_authoring_tool() const;
  INLINE void set_authoring_tool(const string &authoring_tool);

  INLINE const string &get_comments() const;
  INLINE void set_comments(const string &comments);

  INLINE const string &get_copyright() const;
  INLINE void set_copyright(const string &copyright);

  INLINE const string &get_source_data() const;
  INLINE void set_source_data(const string &source_data);

private:
  string _author;
  string _author_email;
  string _author_website;
  string _authoring_tool;
  string _comments;
  string _copyright;
  string _source_data;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "ColladaContributor",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "colladaContributor.I"

#endif
