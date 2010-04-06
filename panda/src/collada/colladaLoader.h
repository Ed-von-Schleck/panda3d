// Filename: colladaLoader.h
// Created by:  rdb (23Aug09)
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

#ifndef COLLADALOADER_H
#define COLLADALOADER_H

#include "pmap.h"
#include "filename.h"
#include "pandaNode.h"
#include "bamCacheRecord.h"

////////////////////////////////////////////////////////////////////
//       Class : ColladaLoader
// Description : 
////////////////////////////////////////////////////////////////////
class ColladaLoader {
public:
  ColladaLoader();
  INLINE virtual ~ColladaLoader() {};
  INLINE static ColladaLoader *make() { return new ColladaLoader; };
  
  virtual void build_graph();

public:
  PT(PandaNode) _root;
  PT(BamCacheRecord) _record;
  Filename _filename;
  bool _error;
};

#endif

