// Filename: lerpfunctor.cxx
// Created by:  frang (26May00)
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

#if defined(WIN32_VC) && !defined(NO_PCH)
#include "lerp_headers.h"
#endif

#pragma hdrstop

#if !defined(WIN32_VC) || defined(NO_PCH)
#include "lerpfunctor.h"
#endif

TypeHandle LerpFunctor::_type_handle;
TypeHandle MultiLerpFunctor::_type_handle;

LerpFunctor::LerpFunctor(const LerpFunctor&)
{
}

LerpFunctor::~LerpFunctor(void)
{
}

LerpFunctor& LerpFunctor::operator=(const LerpFunctor&) {
  return *this;
}

void LerpFunctor::operator()(float) {
  // should not be here
}

MultiLerpFunctor::MultiLerpFunctor(const MultiLerpFunctor& c)
  : LerpFunctor(c), _funcs(c._funcs) {}

MultiLerpFunctor::~MultiLerpFunctor(void) {}

MultiLerpFunctor& MultiLerpFunctor::operator=(const MultiLerpFunctor& c) {
  _funcs = c._funcs;
  LerpFunctor::operator=(c);
  return *this;
}

void MultiLerpFunctor::operator()(float f) {
  for (Functors::iterator i=_funcs.begin(); i!=_funcs.end(); ++i)
    (*(*i))(f);
}

void MultiLerpFunctor::add_functor(LerpFunctor* func) {
  _funcs.insert(func);
}

void MultiLerpFunctor::remove_functor(LerpFunctor* func) {
  _funcs.erase(func);
}
