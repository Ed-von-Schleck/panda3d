// Filename: planeNode.cxx
// Created by:  drose (11Jul02)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "planeNode.h"
#include "geometricBoundingVolume.h"
#include "bamWriter.h"
#include "bamReader.h"
#include "datagram.h"
#include "datagramIterator.h"
#include "geomVertexWriter.h"
#include "geomVertexData.h"
#include "geomLines.h"
#include "geom.h"
#include "cullableObject.h"
#include "cullHandler.h"
#include "boundingPlane.h"

UpdateSeq PlaneNode::_sort_seq;

TypeHandle PlaneNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::CData::make_copy
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CycleData *PlaneNode::CData::
make_copy() const {
  return new CData(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::CData::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void PlaneNode::CData::
write_datagram(BamWriter *, Datagram &dg) const {
  _plane.write_datagram(dg);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::CData::fillin
//       Access: Public, Virtual
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new Light.
////////////////////////////////////////////////////////////////////
void PlaneNode::CData::
fillin(DatagramIterator &scan, BamReader *) {
  _plane.read_datagram(scan);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
PlaneNode::
PlaneNode(const string &name, const Planef &plane) :
  PandaNode(name),
  _priority(0)
{
  // PlaneNodes are hidden by default.
  set_overall_hidden(true);

  set_plane(plane);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::Copy Constructor
//       Access: Protected
//  Description:
////////////////////////////////////////////////////////////////////
PlaneNode::
PlaneNode(const PlaneNode &copy) :
  PandaNode(copy),
  _priority(copy._priority),
  _cycler(copy._cycler)
{
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::output
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void PlaneNode::
output(ostream &out) const {
  PandaNode::output(out);
  out << " " << get_plane();
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::make_copy
//       Access: Public, Virtual
//  Description: Returns a newly-allocated Node that is a shallow copy
//               of this one.  It will be a different Node pointer,
//               but its internal data may or may not be shared with
//               that of the original Node.
////////////////////////////////////////////////////////////////////
PandaNode *PlaneNode::
make_copy() const {
  return new PlaneNode(*this);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::xform
//       Access: Public, Virtual
//  Description: Transforms the contents of this PandaNode by the
//               indicated matrix, if it means anything to do so.  For
//               most kinds of PandaNodes, this does nothing.
////////////////////////////////////////////////////////////////////
void PlaneNode::
xform(const LMatrix4f &mat) {
  PandaNode::xform(mat);
  CDWriter cdata(_cycler);
  cdata->_plane = cdata->_plane * mat;
  cdata->_front_viz = NULL;
  cdata->_back_viz = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::has_cull_callback
//       Access: Public, Virtual
//  Description: Should be overridden by derived classes to return
//               true if cull_callback() has been defined.  Otherwise,
//               returns false to indicate cull_callback() does not
//               need to be called for this node during the cull
//               traversal.
////////////////////////////////////////////////////////////////////
bool PlaneNode::
has_cull_callback() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::cull_callback
//       Access: Public, Virtual
//  Description: If has_cull_callback() returns true, this function
//               will be called during the cull traversal to perform
//               any additional operations that should be performed at
//               cull time.  This may include additional manipulation
//               of render state or additional visible/invisible
//               decisions, or any other arbitrary operation.
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
bool PlaneNode::
cull_callback(CullTraverser *trav, CullTraverserData &data) {
  // Normally, a PlaneNode is invisible.  But if someone shows it, we
  // will draw a visualization, a nice yellow wireframe.

  CullableObject *plane_viz = 
    new CullableObject(get_viz(trav, data), data._state, 
                       data._modelview_transform);
  trav->get_cull_handler()->record_object(plane_viz, trav);

  // Now carry on to render our child nodes.
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::is_renderable
//       Access: Public, Virtual
//  Description: Returns true if there is some value to visiting this
//               particular node during the cull traversal for any
//               camera, false otherwise.  This will be used to
//               optimize the result of get_net_draw_show_mask(), so
//               that any subtrees that contain only nodes for which
//               is_renderable() is false need not be visited.
////////////////////////////////////////////////////////////////////
bool PlaneNode::
is_renderable() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::compute_internal_bounds
//       Access: Protected, Virtual
//  Description: Returns a newly-allocated BoundingVolume that
//               represents the internal contents of the node.  Should
//               be overridden by PandaNode classes that contain
//               something internally.
////////////////////////////////////////////////////////////////////
PT(BoundingVolume) PlaneNode::
compute_internal_bounds(int pipeline_stage) const {
  CDStageReader cdata(_cycler, pipeline_stage);
  return new BoundingPlane(cdata->_plane);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::get_viz
//       Access: Protected
//  Description: Returns a Geom that represents the visualization of
//               the PlaneNode.
////////////////////////////////////////////////////////////////////
PT(Geom) PlaneNode::
get_viz(CullTraverser *trav, CullTraverserData &data) {
  CDReader cdata(_cycler);
  
  // Figure out whether we are looking at the front or the back of the
  // plane.
  const Lens *lens = trav->get_scene()->get_lens();
  Planef eye_plane = cdata->_plane * data._modelview_transform->get_mat();
  bool front = (eye_plane.dist_to_plane(lens->get_nodal_point()) >= 0.0f);

  if (cdata->_front_viz != (Geom *)NULL) {
    return front ? cdata->_front_viz : cdata->_back_viz;
  }

  if (pgraph_cat.is_debug()) {
    pgraph_cat.debug()
      << "Recomputing viz for " << *this << "\n";
  }

  CDWriter cdataw(_cycler, cdata, false);
  const Planef &plane = cdataw->_plane;

  PT(GeomVertexData) vdata = new GeomVertexData
    (get_name(), GeomVertexFormat::get_v3cp(), Geom::UH_static);

  GeomVertexWriter vertex(vdata, InternalName::get_vertex());
  PT(GeomLines) lines = new GeomLines(Geom::UH_static);

  LVector3f a, b;

  if (fabs(plane[0]) > fabs(plane[1])) {
    // X > Y
    if (fabs(plane[0]) > fabs(plane[2])) {
      // X > Y && X > Z.  X is the largest.
      a.set(0, 1, 0);
      b.set(0, 0, 1);
    } else {
      // X > Y && Z > X.  Z is the largest.
      a.set(1, 0, 0);
      b.set(0, 1, 0);
    }
  } else {
    // Y > X
    if (fabs(plane[1]) > fabs(plane[2])) {
      // Y > X && Y > Z.  Y is the largest.
      a.set(1, 0, 0);
      b.set(0, 0, 1);
    } else {
      // Y > X && Z > Y.  Z is the largest.
      a.set(1, 0, 0);
      b.set(0, 1, 0);
    }
  }

  static const int num_segs = 10;
  a *= cdataw->_viz_scale / (num_segs * 2);
  b *= cdataw->_viz_scale / (num_segs * 2);
  
  for (int x = -num_segs; x <= num_segs; ++x) {
    vertex.add_data3f(plane.project(a * x - b * num_segs));
    vertex.add_data3f(plane.project(a * x + b * num_segs));
    lines->add_next_vertices(2);
    lines->close_primitive();
  }
  for (int y = -num_segs; y <= num_segs; ++y) {
    vertex.add_data3f(plane.project(b * y - a * num_segs));
    vertex.add_data3f(plane.project(b * y + a * num_segs));
    lines->add_next_vertices(2);
    lines->close_primitive();
  }

  cdataw->_front_viz = new Geom(vdata->set_color(Colorf(1.0f, 1.0f, 0.0f, 1.0f)));
  cdataw->_front_viz->add_primitive(lines);

  cdataw->_back_viz = new Geom(vdata->set_color(Colorf(0.4f, 0.4f, 0.0f, 1.0f)));
  cdataw->_back_viz->add_primitive(lines);

  return front ? cdataw->_front_viz : cdataw->_back_viz;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               PlaneNode.
////////////////////////////////////////////////////////////////////
void PlaneNode::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void PlaneNode::
write_datagram(BamWriter *manager, Datagram &dg) {
  PandaNode::write_datagram(manager, dg);
  manager->write_cdata(dg, _cycler);
  dg.add_int32(_priority);
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type PlaneNode is encountered
//               in the Bam file.  It should create the PlaneNode
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *PlaneNode::
make_from_bam(const FactoryParams &params) {
  PlaneNode *node = new PlaneNode("");
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  node->fillin(scan, manager);

  return node;
}

////////////////////////////////////////////////////////////////////
//     Function: PlaneNode::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new PlaneNode.
////////////////////////////////////////////////////////////////////
void PlaneNode::
fillin(DatagramIterator &scan, BamReader *manager) {
  PandaNode::fillin(scan, manager);
  manager->read_cdata(scan, _cycler);
  _priority = scan.get_int32();
}
