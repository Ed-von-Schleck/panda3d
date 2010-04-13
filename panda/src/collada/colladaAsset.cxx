// Filename: colladaAsset.cxx
// Created by: Xidram (13Apr10)
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

#include "colladaAsset.h"

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::Constructor
//       Access: Public
//  Description: 
////////////////////////////////////////////////////////////////////
ColladaAsset::
ColladaAsset() {
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::load_xml
//       Access: Public
//  Description: Loads <asset> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaAsset::
load_xml(const TiXmlElement *xelement) {
  nassertr(xelement != NULL, false);
  nassertr(xelement->ValueStr() == "asset", false);

  const char* coordsys = xelement->Attribute("up_axis");
  if (coordsys) {
    _coordsys = parse_coordinate_system_string(coordsys);
    if (_coordsys == CS_invalid || _coordsys == CS_default) {
      collada_cat.error()
        << "Invalid coordinate system value: " << coordsys << "\n";
      return false;
    }
  }
  
  return true;
}

