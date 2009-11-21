// Filename: physxObjectCollection.h
// Created by:  enn0x (08Nov09)
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

#ifndef PHYSXOBJECTCOLLECTION_H
#define PHYSXOBJECTCOLLECTION_H

#include "pandabase.h"

#include "config_physx.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxObjectCollection
// Description : 
////////////////////////////////////////////////////////////////////
template <class T>
class EXPCL_PANDAPHYSX PhysxObjectCollection {

public:
  unsigned int size() const;
  void add(PT(T) object);
  void remove(PT(T) object);
  T *get(unsigned int index) const;
  T *operator [] (unsigned int index) const;

  void ls() const;
  void ls(ostream &out, int indent_level=0) const;

private:
  pvector<PT(T)> _objects;
};

#include "physxObjectCollection.I"

#endif // PHYSOBJECTCOLLECTION_H
