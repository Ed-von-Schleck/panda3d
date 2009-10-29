// Filename: physxHeightFieldDesc.cxx
// Created by:  enn0x (15Oct09)
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

#include "physxHeightFieldDesc.h"

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::set_convex_edge_threshold
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxHeightFieldDesc::
set_convex_edge_threshold(float threshold) {

  _desc.convexEdgeThreshold = threshold;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::set_thickness
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxHeightFieldDesc::
set_thickness(float thickness) {

  _desc.thickness = thickness;
}

////////////////////////////////////////////////////////////////////
//     Function: PhysxHeightFieldDesc::set_image
//       Access: Published
//  Description: 
////////////////////////////////////////////////////////////////////
void PhysxHeightFieldDesc::
set_image(const PNMImage &image) {

  dealloc_samples();

  NxU32 _32K = 32767; // (1<<15)-1;
  NxU32 _64K = 65535; // (1<<16)-1;

  NxU32 nbRows = image.get_x_size();
  NxU32 nbColumns = image.get_y_size();

  _desc.format = NX_HF_S16_TM;
  _desc.nbColumns = nbColumns;
  _desc.nbRows = nbRows;
  _desc.sampleStride = sizeof(NxU32);

  //_desc.samples             = new NxU32[ nbColumns * nbRows ];
  alloc_samples(nbColumns * nbRows);

  NxU8 *currentByte = (NxU8 *)(_desc.samples);

  for (NxU32 row=0; row < nbRows; row++) {
    for (NxU32 column=0; column < nbColumns; column++) {

      NxHeightFieldSample* currentSample = (NxHeightFieldSample *)currentByte;

      NxReal fvalue = image.get_bright(row, column);
      NxI16 ivalue = (NxI16)((fvalue - 0.5f) * _32K);

      currentSample->height         = (NxI16) ivalue;
      currentSample->tessFlag       = (NxU8) 0;
      currentSample->materialIndex0 = (NxU8) 1;
      currentSample->materialIndex1 = (NxU8) 1;

      currentByte += _desc.sampleStride;
    }
  }
}

