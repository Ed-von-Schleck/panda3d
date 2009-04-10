// Filename: odeNode.cxx
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

#include "odeNode.h"
#include "ode_includes.h"

#include "transformState.h"

TypeHandle OdeNode::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function : OdeNode
//       Access : Public
//  Description : Constructor, uses the global OdeManager.
////////////////////////////////////////////////////////////////////
OdeNode::
OdeNode(const string &name) : PandaNode(name),
  OdeBody(dBodyCreate(OdeManager::get_global_ptr()->get_world().get_id())) {
  
  OdeManager* manager = OdeManager::get_global_ptr();
  _ode_manager = manager;
  nassertv(_ode_manager);
  _ode_manager->_ode_nodes.push_back(this);
}

////////////////////////////////////////////////////////////////////
//     Function : OdeNode
//       Access : Public
//  Description : Constructor, uses a custom manager.
////////////////////////////////////////////////////////////////////
OdeNode::
OdeNode(const string &name, OdeManager* manager) :
  PandaNode(name), OdeBody(dBodyCreate(manager->get_world().get_id())) {
  
  _ode_manager = manager;
  nassertv(_ode_manager);
  _ode_manager->_ode_nodes.push_back(this);
}

////////////////////////////////////////////////////////////////////
//     Function : OdeNode
//       Access : Public
//  Description : Copy Constructor.
////////////////////////////////////////////////////////////////////
OdeNode::
OdeNode(const OdeNode &copy) : PandaNode(copy), OdeBody(copy) {
  _ode_manager = copy._ode_manager;
}

////////////////////////////////////////////////////////////////////
//     Function : ~OdeNode
//       Access : Public
//  Description : Destructor
////////////////////////////////////////////////////////////////////
OdeNode::
~OdeNode() {
  _ode_manager->remove_ode_node(this);
  destroy();
}

////////////////////////////////////////////////////////////////////
//     Function : update_transform
//       Access : Public
//  Description : Copies the transform from the OdeBody to the
//                PandaNode.
////////////////////////////////////////////////////////////////////
void OdeNode::
update_transform() {
  set_transform(TransformState::make_pos_quat_scale(
    get_position(), get_quaternion(), 1.0f));
}

////////////////////////////////////////////////////////////////////
//     Function : transform_changed
//       Access : Private, Virtual
//  Description : Copies the transform from the PandaNode to the
//                OdeBody.
////////////////////////////////////////////////////////////////////
void OdeNode::
transform_changed() {
  PandaNode::transform_changed();
  
  // Get and apply the transform
  CPT(TransformState) transform = get_transform();
  set_quaternion(transform->get_quat());
  set_position(transform->get_pos());
}

////////////////////////////////////////////////////////////////////
//     Function : write
//       Access : Public
//  Description : Write a string representation of this instance to
//                <out>.
////////////////////////////////////////////////////////////////////
void OdeNode::
write(ostream &out, unsigned int indent) const {
  #ifndef NDEBUG //[
  out.width(indent); out<<""; out<<"OdeNode:\n";
  OdeBody::write(out, indent+2);
  PandaNode::write(out, indent+2);
  #endif //] NDEBUG
}

