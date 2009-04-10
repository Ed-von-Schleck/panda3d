// Filename: odeManager.cxx
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

#include "odeManager.h"
#include "odeNode.h"
#include "odeWorld.h"

#include <algorithm>
#include "pvector.h"

// We're creating the global manager in get_global_ptr()
OdeManager* OdeManager::_global_ptr = NULL;

TypeHandle OdeManager::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function : OdeManager
//       Access : Public
//  Description : Constructor.
////////////////////////////////////////////////////////////////////
OdeManager::
OdeManager() {
}

////////////////////////////////////////////////////////////////////
//     Function : ~OdeManager
//       Access : Public
//  Description : Destructor
////////////////////////////////////////////////////////////////////
OdeManager::
~OdeManager() {
  OdeNodeVector::iterator it;
  for (it = _ode_nodes.begin(); it != _ode_nodes.end(); ++it) {
    nassertv((*it)->_ode_manager == this);
    (*it)->_ode_manager = NULL;
  }
  _world.destroy();
}

////////////////////////////////////////////////////////////////////
//     Function : remove_ode_node
//       Access : Private
//  Description : Removes an OdeNode from the list of managed
//                objects. You should only call this in the
//                OdeNode destructor, really, otherwise weird
//                things may happen.
////////////////////////////////////////////////////////////////////
void OdeManager::
remove_ode_node(OdeNode *node) {
  nassertv(node);
  OdeNodeVector::iterator found;

  found = find(_ode_nodes.begin(), _ode_nodes.end(), node);
  if (found == _ode_nodes.end()) {
    return;
  }
  
  nassertv(node->_ode_manager == this);
  node->_ode_manager = NULL;
  _ode_nodes.erase(found);
}

////////////////////////////////////////////////////////////////////
//     Function : do_physics
//       Access : Public
//  Description : This should be called every frame.
//                Synchronizes the ODE scene with the Panda scene.
//                It also calls step() on the managed world.
////////////////////////////////////////////////////////////////////
void OdeManager::
do_physics(float dt) {
  _world.step(dt);
  // Now, run through each OdeNode in the set.
  OdeNodeVector::iterator it;
  for (it = _ode_nodes.begin(); it != _ode_nodes.end(); ++it) {
    (*it)->update_transform();
  }
}

////////////////////////////////////////////////////////////////////
//     Function : output
//       Access : Public
//  Description : Write a string representation of this instance to
//                <out>.
////////////////////////////////////////////////////////////////////
void OdeManager::
output(ostream &out) const {
  #ifndef NDEBUG //[
  out<<""<<"OdeManager";
  #endif //] NDEBUG
}

////////////////////////////////////////////////////////////////////
//     Function : write
//       Access : Public
//  Description : Write a string representation of this instance to
//                <out>.
////////////////////////////////////////////////////////////////////
void OdeManager::
write(ostream &out, unsigned int indent) const {
  #ifndef NDEBUG //[
  out.width(indent); out<<""<<"OdeManager:\n";
  for (int i = 0; i < _ode_nodes.size(); ++i) {
    _ode_nodes[i]->write(out, indent+2);
  }
  #endif //] NDEBUG
}

