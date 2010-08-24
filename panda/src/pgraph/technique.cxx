// Filename: technique.cxx
// Created by: Xidram (13Aug10)
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

#include "technique.h"
#include "pandaNode.h"

TypeHandle Technique::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: Technique::set_pass
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Technique::
set_pass(int i, RenderPass *pass) {
  _passes[i] = pass;
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::add_pass
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Technique::
add_pass(RenderPass *pass) {
  _passes.push_back(pass);
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::remove_pass
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Technique::
remove_pass(int i) {
  _passes.erase(_passes.begin() + i);
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::clear_pass
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void Technique::
clear_pass() {
  _passes.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               Technique.
////////////////////////////////////////////////////////////////////
void Technique::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void Technique::
write_datagram(BamWriter *manager, Datagram &dg) {
  // First write out the amount of render passes, so we will
  // know how many to read when we're reading them later.
  dg.add_uint16((unsigned short) _passes.size());

  RenderPasses::const_iterator it;
  for (it = _passes.begin(); it != _passes.end(); ++it) {
    manager->write_pointer(dg, *it);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::complete_pointers
//       Access: Public, Virtual
//  Description: Receives an array of pointers, one for each time
//               manager->read_pointer() was called in fillin().
//               Returns the number of pointers processed.
////////////////////////////////////////////////////////////////////
int Technique::
complete_pointers(TypedWritable **p_list, BamReader *manager) {
  int pi = 0;

  RenderPasses::iterator ci;
  for (ci = _passes.begin(); ci != _passes.end(); ++ci) {
    RenderPass *ts = DCAST(RenderPass, p_list[pi++]);
    *ci = ts;
    ++pi;
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type Technique is encountered
//               in the Bam file.  It should create the Effect
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *Technique::
make_from_bam(const FactoryParams &params) {
  Technique *node = new Technique();
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new Technique.
////////////////////////////////////////////////////////////////////
void Technique::
fillin(DatagramIterator &scan, BamReader *manager) {
  int tech_count = (int) scan.get_uint16();
  _passes.clear();
  _passes.reserve(tech_count);
  for (int i = 0; i < tech_count; ++i) {
    manager->read_pointer(scan);
    _passes[i] = NULL;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Technique::write
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void Technique::
write(ostream &out, int indent_level) const {
  for (RenderPasses::const_iterator ti = _passes.begin();
       ti != _passes.end();
       ++ti) {
    (*ti)->write(out, indent_level + 2);
  }
}

