// Filename: physxMaterialDesc.h
// Created by:  enn0x (21Sep09)
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

#ifndef PHYSXMATERIALDESC_H
#define PHYSXMATERIALDESC_H

#include "pandabase.h"
#include "typedReferenceCount.h"

#include "physxEnums.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxMaterialDesc
// Description : Descriptor class for materials. See PhysxMaterial.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxMaterialDesc : public TypedReferenceCount, public PhysxEnums {

PUBLISHED:
  INLINE PhysxMaterialDesc();
  INLINE ~PhysxMaterialDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_dynamic_friction(float coef);
  void set_static_friction(float coef);
  void set_restitution(float rest);
  void set_dynamic_friction_v(float coef);
  void set_static_friction_v(float coef);
  void set_dir_of_anisotropy(const LVector3f dir);
  void set_flag(PhysxMaterialFlag flag, bool value);
  void set_friction_combine_mode(PhysxCombineMode mode);
  void set_restitution_combine_mode(PhysxCombineMode mode);

  float get_dynamic_friction() const;
  float get_static_friction() const;
  float get_restitution() const;
  float get_dynamic_friction_v() const;
  float get_static_friction_v() const;
  LVector3f get_dir_of_anisotropy() const;
  bool get_flag(PhysxMaterialFlag flag) const;
  PhysxCombineMode get_friction_combine_mode() const;
  PhysxCombineMode get_restitution_combine_mode() const;

public:
  INLINE PhysxMaterialDesc(const NxMaterialDesc *desc);

  virtual NxMaterialDesc *ptr() { return &_desc; };

private:
  NxMaterialDesc _desc;

////////////////////////////////////////////////////////////////////
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "PhysxMaterialDesc", 
                  TypedReferenceCount::get_class_type());
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

#include "physxMaterialDesc.I"

#endif // PHYSXMATERIALDESC_H
