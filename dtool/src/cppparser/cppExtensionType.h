// Filename: cppExtensionType.h
// Created by:  drose (21Oct99)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#ifndef CPPEXTENSIONTYPE_H
#define CPPEXTENSIONTYPE_H

#include "dtoolbase.h"

#include "cppType.h"
#include "cppInstance.h"

class CPPScope;
class CPPIdentifier;

///////////////////////////////////////////////////////////////////
//       Class : CPPExtensionType
// Description :
////////////////////////////////////////////////////////////////////
class CPPExtensionType : public CPPType {
public:
  enum Type {
    T_enum,
    T_class,
    T_struct,
    T_union,
  };

  CPPExtensionType(Type type, CPPIdentifier *ident, CPPScope *current_scope,
                   const CPPFile &file);

  virtual string get_simple_name() const;
  virtual string get_local_name(CPPScope *scope = NULL) const;
  virtual string get_fully_scoped_name() const;

  virtual bool is_incomplete() const;
  virtual bool is_tbd() const;

  virtual CPPDeclaration *substitute_decl(SubstDecl &subst,
                                          CPPScope *current_scope,
                                          CPPScope *global_scope);

  virtual bool is_equivalent(const CPPType &other) const;


  virtual void output(ostream &out, int indent_level, CPPScope *scope,
                      bool complete) const;
  virtual SubType get_subtype() const;

  virtual CPPExtensionType *as_extension_type();


  Type _type;
  CPPIdentifier *_ident;
};

ostream &operator << (ostream &out, CPPExtensionType::Type type);

#endif
