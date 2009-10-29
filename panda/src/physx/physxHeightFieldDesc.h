// Filename: physxHeightFieldDesc.h
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

#ifndef PHYSXHEIGHTFIELDDESC_H
#define PHYSXHEIGHTFIELDDESC_H

#include "pandabase.h"
#include "pnmImage.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

////////////////////////////////////////////////////////////////////
//       Class : PhysxHeightFieldDesc
// Description : Descriptor class for height fields. The height
//               field data is copied when a PhysxHeightField object
//               is created from this descriptor. After the call 
//               the user may discard the original height data (e.g.
//               release the PNGImage).
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxHeightFieldDesc {

PUBLISHED:
  INLINE PhysxHeightFieldDesc();
  INLINE ~PhysxHeightFieldDesc();

  INLINE void set_to_default();
  INLINE bool is_valid() const;

  void set_image(const PNMImage &image);
  void set_thickness(float thickness);
  void set_convex_edge_threshold(float threshold);

public:
  NxHeightFieldDesc _desc;

private:
  NxU32 *_samples;

  INLINE void alloc_samples(unsigned int n);
  INLINE void dealloc_samples();
};

#include "physxHeightFieldDesc.I"

#endif // PHYSXHEIGHTFIELDDESC_H
