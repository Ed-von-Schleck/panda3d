// Filename: odeManager.h
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

#ifndef ODEMANAGER_H
#define ODEMANAGER_H

#include "pandabase.h"
#include "pointerTo.h"
#include "pvector.h"

#include "odeNode.h"

////////////////////////////////////////////////////////////////////
//       Class : OdeManager
// Description : 
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAODE OdeManager : public TypedObject {
PUBLISHED:
  OdeManager();
  virtual ~OdeManager();
  
  void remove_ode_node(OdeNode *node);
  void do_physics(float dt);
  
  INLINE OdeWorld get_world() const;
  INLINE static OdeManager *get_global_ptr();
  
  virtual void output(ostream &out) const;
  virtual void write(ostream &out, unsigned int indent=0) const;

private:
  typedef pvector<WPT(OdeNode)> OdeNodeVector;
  OdeNodeVector _ode_nodes;
  OdeWorld _world;
  static OdeManager* _global_ptr;

  friend class OdeNode;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedObject::init_type();
    register_type(_type_handle, "OdeManager",
                  TypedObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;

};

#include "odeManager.I"

#endif // ODEMANAGER_H

