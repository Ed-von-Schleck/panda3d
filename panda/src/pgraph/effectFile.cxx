// Filename: effectFile.cxx
// Created by: Xidram (15Aug10)
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

#include "effectFile.h"

TypeHandle EffectFile::_type_handle;


////////////////////////////////////////////////////////////////////
//     Function: EffectFile::make_copy
//       Access: Public, Virtual
//  Description: Returns a newly-allocated Node that is a shallow copy
//               of this one.  It will be a different Node pointer,
//               but its internal data may or may not be shared with
//               that of the original Node.
////////////////////////////////////////////////////////////////////
PandaNode *EffectFile::
make_copy() const {
  return new EffectFile(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               EffectFile.
////////////////////////////////////////////////////////////////////
void EffectFile::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void EffectFile::
write_datagram(BamWriter *manager, Datagram &dg) {
  Effect::write_datagram(manager, dg);
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type EffectFile is encountered
//               in the Bam file.  It should create the EffectFile
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *EffectFile::
make_from_bam(const FactoryParams &params) {
  EffectFile *node = new EffectFile("");
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: EffectFile::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new EffectFile.
////////////////////////////////////////////////////////////////////
void EffectFile::
fillin(DatagramIterator &scan, BamReader *manager) {
  Effect::fillin(scan, manager);
}
