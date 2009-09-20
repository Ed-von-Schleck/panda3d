// Filename: physxShape.h
// Created by:  enn0x (16Sep09)
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

#ifndef PHYSXSHAPE_H
#define PHYSXSHAPE_H

#include "pandabase.h"
#include "pointerTo.h"
#include "lmatrix.h"
#include "lquaternion.h"

#include "physx_enumerations.h"
#include "physxObject.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

class PhysxActor;

////////////////////////////////////////////////////////////////////
//       Class : PhysxShape
// Description : Abstract base class for shapes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxShape : public PhysxObject {

PUBLISHED:
  INLINE PhysxShape();
  INLINE ~PhysxShape();

  PT(PhysxActor) get_actor() const;

  void set_name(const char *name);
  void set_flag(const PhysxShapeFlag flag, bool value);

  const char *get_name() const;
  bool get_flag(const PhysxShapeFlag flag) const;

////////////////////////////////////////////////////////////////////
PUBLISHED:
  void release();

public:
  static PT(PhysxShape) factory(NxShapeType shapeType);

  virtual NxShape *ptr() const = 0;

  virtual void link(NxShape *shapePtr) = 0;
  virtual void unlink() = 0;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    PhysxObject::init_type();
    register_type(_type_handle, "PhysxShape", 
                  PhysxObject::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

#include "physxShape.I"

#endif // PHYSXSHAPE_H
