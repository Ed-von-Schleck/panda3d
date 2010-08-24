// Filename: effect.cxx
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

#include "effect.h"

TypeHandle Effect::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: Effect::cull_callback
//       Access: Public, Virtual
//  Description: This function will be called during the cull
//               traversal to perform any additional operations that
//               should be performed at cull time.  This may include
//               additional manipulation of render state or additional
//               visible/invisible decisions, or any other arbitrary
//               operation.
//
//               Note that this function will *not* be called unless
//               set_cull_callback() is called in the constructor of
//               the derived class.  It is necessary to call
//               set_cull_callback() to indicated that we require
//               cull_callback() to be called.
//
//               By the time this function is called, the node has
//               already passed the bounding-volume test for the
//               viewing frustum, and the node's transform and state
//               have already been applied to the indicated
//               CullTraverserData object.
//
//               The return value is true if this node should be
//               visible, or false if it should be culled.
////////////////////////////////////////////////////////////////////
bool Effect::
cull_callback(CullTraverser *trav, CullTraverserData &data) {
  //TODO: technique selection
  for (int i = 0; i < get_num_children(); ++i) {
    CullTraverserData next_data(data, get_child(i));
    trav->traverse(next_data);
  }

  // We return false because we already manually processed the children.
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::output
//       Access: Public, Virtual
//  Description: Writes a brief description of the node to the
//               indicated output stream.  This is invoked by the <<
//               operator.  It may be overridden in derived classes to
//               include some information relevant to the class.
////////////////////////////////////////////////////////////////////
void Effect::
output(ostream &out) const {
  PandaNode::output(out);
  out << " (" << _techniques.size() << " techniques)";
  if (!_filename.empty()) {
    out << " (from " << _filename << ")";
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::write
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
void Effect::
write(ostream &out, int indent_level) const {
  PandaNode::write(out, indent_level);

  for (Techniques::const_iterator ti = _techniques.begin();
       ti != _techniques.end();
       ++ti) {
    indent(out, indent_level + 2)
      << ti->second->get_type() << " " << ti->first->get_name() << "\n";
    ti->second->write(out, indent_level + 2);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::read
//       Access: Published, Virtual
//  Description: Should be overridden by an effect implementation
//               to read that particular effect file format.
////////////////////////////////////////////////////////////////////
bool Effect::
read(const Filename &fullpath, BamCacheRecord *record) {
  //TODO: read from bam
  nassertr_always(false, false);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               Effect.
////////////////////////////////////////////////////////////////////
void Effect::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void Effect::
write_datagram(BamWriter *manager, Datagram &dg) {
  PandaNode::write_datagram(manager, dg);

  // First write out the amount of techniques, so we will
  // know how many to read when we're reading them later.
  dg.add_uint16((unsigned short) _techniques.size());

  Techniques::const_iterator it;
  for (it = _techniques.begin(); it != _techniques.end(); ++it) {
    manager->write_pointer(dg, it->first);
    manager->write_pointer(dg, it->second);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::complete_pointers
//       Access: Public, Virtual
//  Description: Receives an array of pointers, one for each time
//               manager->read_pointer() was called in fillin().
//               Returns the number of pointers processed.
////////////////////////////////////////////////////////////////////
int Effect::
complete_pointers(TypedWritable **p_list, BamReader *manager) {
  int pi = PandaNode::complete_pointers(p_list, manager);

  int tech_count = manager->get_int_tag("tech_count");
  for (int i = 0; i < tech_count; ++i) {
    const InternalName *name = DCAST(InternalName, p_list[pi++]);
    Technique *t = DCAST(Technique, p_list[pi++]);
    _techniques[name] = t;
  }

  return pi;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type Effect is encountered
//               in the Bam file.  It should create the Effect
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *Effect::
make_from_bam(const FactoryParams &params) {
  Effect *node = new Effect("");
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: Effect::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new Effect.
////////////////////////////////////////////////////////////////////
void Effect::
fillin(DatagramIterator &scan, BamReader *manager) {
  PandaNode::fillin(scan, manager);

  int tech_count = (int) scan.get_uint16();
  manager->set_int_tag("tech_count", tech_count);
  _techniques.clear();
  for (int i = 0; i < tech_count; i++) {
    manager->read_pointer(scan);
    manager->read_pointer(scan);
  }
}

