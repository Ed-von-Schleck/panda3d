// Filename: navMeshAttrib.cxx
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

#include "navMeshAttrib.h"
#include "dcast.h"
#include "bamReader.h"
#include "bamWriter.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "config_pgraph.h"

TypeHandle NavMeshAttrib::_type_handle;
int NavMeshAttrib::_attrib_slot;

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::Constructor
//       Access: Protected
//  Description: Use NavMeshAttrib::make() to construct a new
//               NavMeshAttrib object.
////////////////////////////////////////////////////////////////////
NavMeshAttrib::
NavMeshAttrib(bool off, int area) :
  _off(off),
  _area(area)
{
  nassertv(_area <= 255);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::make
//       Access: Published, Static
//  Description: Constructs a new NavMeshAttrib object that indicates
//               the area code for this geometry. If the area code
//               is zero, it will be unwalkable.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) NavMeshAttrib::
make(int area) {
  NavMeshAttrib *attrib = new NavMeshAttrib(false, area);
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::make_off
//       Access: Published, Static
//  Description: Constructs a new NavMeshAttrib object that indicates
//               that this geometry should not be included in any
//               kind of navigation mesh generation.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) NavMeshAttrib::
make_off() {
  NavMeshAttrib *attrib =
    new NavMeshAttrib(true, 0);
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::make_default
//       Access: Published, Static
//  Description: Returns a RenderAttrib that corresponds to whatever
//               the standard default properties for render attributes
//               of this type ought to be.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) NavMeshAttrib::
make_default() {
  return return_new(new NavMeshAttrib(false));
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::set_area
//       Access: Published
//  Description: Returns a new NavMeshAttrib, just like this one, but
//               with the area changed to the indicated value.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) NavMeshAttrib::
set_area(int area) const {
  NavMeshAttrib *attrib = new NavMeshAttrib(*this);
  attrib->_area = area;
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::lower_attrib_can_override
//       Access: Public, Virtual
//  Description: Intended to be overridden by derived RenderAttrib
//               types to specify how two consecutive RenderAttrib
//               objects of the same type interact.
//
//               This should return false if a RenderAttrib on a
//               higher node will compose into a RenderAttrib on a
//               lower node that has a higher override value, or false
//               if the lower RenderAttrib will completely replace the
//               state.
//
//               The default behavior is false: normally, a
//               RenderAttrib in the graph cannot completely override
//               a RenderAttrib above it, regardless of its override
//               value--instead, the two attribs are composed.  But
//               for some kinds of RenderAttribs, it is useful to
//               allow this kind of override.
//
//               This method only handles the one special case of a
//               lower RenderAttrib with a higher override value.  If
//               the higher RenderAttrib has a higher override value,
//               it always completely overrides.  And if both
//               RenderAttribs have the same override value, they are
//               always composed.
////////////////////////////////////////////////////////////////////
bool NavMeshAttrib::
lower_attrib_can_override() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::output
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void NavMeshAttrib::
output(ostream &out) const {
  out << get_type() << ":";
  if (is_off()) {
    out << "off";
  }
  if (_area == 0) {
    out << "(unwalkable)";
  } else {
    out << "(area " << (int)_area << ")";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::compare_to_impl
//       Access: Protected, Virtual
//  Description: Intended to be overridden by derived NavMeshAttrib
//               types to return a unique number indicating whether
//               this NavMeshAttrib is equivalent to the other one.
//
//               This should return 0 if the two NavMeshAttrib objects
//               are equivalent, a number less than zero if this one
//               should be sorted before the other one, and a number
//               greater than zero otherwise.
//
//               This will only be called with two NavMeshAttrib
//               objects whose get_type() functions return the same.
////////////////////////////////////////////////////////////////////
int NavMeshAttrib::
compare_to_impl(const RenderAttrib *other) const {
  const NavMeshAttrib *ta;
  DCAST_INTO_R(ta, other, 0);

  if (is_off() != ta->is_off()) {
    if (navigation_cat.is_spam()) {
      navigation_cat.spam()
        << "Comparing " << (int)is_off() << " to " << (int)ta->is_off() << " result = "
        << (int)is_off() - (int)ta->is_off() << "\n";
    }

    return (int)is_off() - (int)ta->is_off();
  }

  if (navigation_cat.is_spam()) {
    navigation_cat.spam()
      << "Comparing " << _area << " to " << ta->_area << " result = "
      << (_area < ta->_area) << "\n";
  }

  return (_area < ta->_area);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               NavMeshAttrib.
////////////////////////////////////////////////////////////////////
void NavMeshAttrib::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void NavMeshAttrib::
write_datagram(BamWriter *manager, Datagram &dg) {
  RenderAttrib::write_datagram(manager, dg);

  // We cheat, and modify the bam stream without upping the bam
  // version.  We can do this since we know that no existing bam files
  // have a NavMeshAttrib in them.
  dg.add_bool(_off);
  dg.add_uint8(_area);
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type NavMeshAttrib is encountered
//               in the Bam file.  It should create the NavMeshAttrib
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *NavMeshAttrib::
make_from_bam(const FactoryParams &params) {
  NavMeshAttrib *attrib = new NavMeshAttrib(false);
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  attrib->fillin(scan, manager);

  return attrib;
}

////////////////////////////////////////////////////////////////////
//     Function: NavMeshAttrib::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new NavMeshAttrib.
////////////////////////////////////////////////////////////////////
void NavMeshAttrib::
fillin(DatagramIterator &scan, BamReader *manager) {
  RenderAttrib::fillin(scan, manager);

  _off = scan.get_bool();
  _area = scan.get_uint8();
}
