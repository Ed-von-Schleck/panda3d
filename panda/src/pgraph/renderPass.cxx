// Filename: renderPass.cxx
// Created by:  rdb (13Aug10)
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

#include "renderPass.h"

TypeHandle RenderPass::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               RenderPass.
////////////////////////////////////////////////////////////////////
void RenderPass::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void RenderPass::
write_datagram(BamWriter *manager, Datagram &dg) {
  dg.add_string(get_name());
  dg.add_uint8((unsigned char) _draw_type);

  dg.add_uint8((unsigned char) RTP_COUNT);
  for (int i = 0; i < RTP_COUNT; ++i) {
    dg.add_bool(_clear_active[i]);
    _clear_value[i].write_datagram(dg);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type RenderPass is encountered
//               in the Bam file.  It should create the RenderPass
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *RenderPass::
make_from_bam(const FactoryParams &params) {
  RenderPass *node = new RenderPass("");
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: RenderPass::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new RenderPass.
////////////////////////////////////////////////////////////////////
void RenderPass::
fillin(DatagramIterator &scan, BamReader *manager) {
  set_name(scan.get_string());
  _draw_type = (DrawType) scan.get_uint8();

  int num_rtps = (int) scan.get_uint8();
  if (num_rtps > RTP_COUNT) {
    num_rtps = RTP_COUNT;
  }
  for (int i = 0; i < num_rtps; i++) {
    _clear_active[i] = scan.get_bool();
    _clear_value[i].read_datagram(scan);
  }
}
