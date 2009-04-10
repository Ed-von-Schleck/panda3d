// Filename: odeNode.h
// Created by:  pro-rsoft (10Apr09)
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

#ifndef ODENODE_H
#define ODENODE_H

#include "pandabase.h"
#include "pandaNode.h"
#include "odeBody.h"

class OdeManager;

////////////////////////////////////////////////////////////////////
//       Class : OdeNode
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAODE OdeNode : public OdeBody, public PandaNode {
PUBLISHED:
  OdeNode(const string &name);
  OdeNode(const string &name, OdeManager* manager);
  OdeNode(const OdeNode &copy);
  virtual ~OdeNode();

  void update_transform();
  virtual void write(ostream &out, unsigned int indent=0) const;

private:
  virtual void transform_changed();

  OdeManager* _ode_manager;

  friend class OdeManager;
  
// We need to redefine these because we're inheriting TypedObject twice.
PUBLISHED:
  INLINE int get_type_index() const {
    return OdeBody::get_type_index();
  }
  INLINE bool is_of_type(TypeHandle handle) const {
    return OdeBody::is_of_type(handle);
  }
  INLINE bool is_exact_type(TypeHandle handle) const {
    return OdeBody::is_exact_type(handle);
  }
  INLINE TypedObject *as_typed_object() {
    return OdeBody::as_typed_object();
  }
  INLINE const TypedObject *as_typed_object() const {
    return OdeBody::as_typed_object();
  }

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    OdeBody::init_type();
    PandaNode::init_type();
    register_type(_type_handle, "OdeNode",
                  OdeBody::get_class_type(),
                  PandaNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "odeNode.I"

#endif // ODENODE_H

