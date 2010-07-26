// Filename: navMeshAttrib.h
// Created by:  rdb (26Jul10)
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

#ifndef NAVMESHATTRIB_H
#define NAVMESHATTRIB_H

#include "config_navigation.h"

#include "renderAttrib.h"

class FactoryParams;

////////////////////////////////////////////////////////////////////
//       Class : NavMeshAttrib
// Description : Specifies special properties for a node that
//               apply when it is used as navigation mesh.
////////////////////////////////////////////////////////////////////
class EXPCL_NAVIGATION NavMeshAttrib : public RenderAttrib {
protected:
  NavMeshAttrib(bool off, int area = 63);
  INLINE NavMeshAttrib(const NavMeshAttrib &copy);

PUBLISHED:
  static CPT(RenderAttrib) make(int area);
  static CPT(RenderAttrib) make_off();
  static CPT(RenderAttrib) make_default();

  INLINE bool is_off() const;
  INLINE int get_area() const;
  CPT(RenderAttrib) set_area(int area) const;

public:
  virtual bool lower_attrib_can_override() const;
  virtual void output(ostream &out) const;

protected:
  virtual int compare_to_impl(const RenderAttrib *other) const;

private:
  bool _off;
  int _area;

PUBLISHED:
  static int get_class_slot() {
    return _attrib_slot;
  }
  virtual int get_slot() const {
    return get_class_slot();
  }

public:
  static void register_with_read_factory();
  virtual void write_datagram(BamWriter *manager, Datagram &dg);

protected:
  static TypedWritable *make_from_bam(const FactoryParams &params);
  void fillin(DatagramIterator &scan, BamReader *manager);

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    RenderAttrib::init_type();
    register_type(_type_handle, "NavMeshAttrib",
                  RenderAttrib::get_class_type());
    _attrib_slot = register_slot(_type_handle, 100, make_default);
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
  static int _attrib_slot;
};

#include "navMeshAttrib.I"

#endif

