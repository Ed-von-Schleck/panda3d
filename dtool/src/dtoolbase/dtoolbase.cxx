// Filename: dtoolbase.cxx
// Created by:  drose (12Sep00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#include "dtoolbase.h"


#ifndef NDEBUG

void *default_operator_new(size_t size) {
  return malloc(size);
}

void default_operator_delete(void *ptr) {
  free(ptr);
}

// We absolutely depend on the static initialization of these pointers
// to happen at load time, before any static constructors are called.
void *(*global_operator_new)(size_t size) = &default_operator_new;
void (*global_operator_delete)(void *ptr) = &default_operator_delete;

#endif  // NDEBUG
