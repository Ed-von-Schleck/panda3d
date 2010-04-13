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

TypeHandle ColladaAsset::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaAsset::
ColladaAsset() {
  _coordsys = CS_default;
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

  const TiXmlElement *up_axis = xelement->FirstChildElement("up_axis");
  if (up_axis) {
    const char* coordsys = up_axis->GetText();
    _coordsys = parse_coordinate_system_string(coordsys);
    if (_coordsys == CS_invalid || _coordsys == CS_default) {
      collada_cat.error()
        << "Invalid coordinate system value: " << coordsys << "\n";
      return false;
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the asset.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaAsset::
make_xml() const {
  TiXmlElement * xelement = new TiXmlElement("asset");

  TiXmlElement * up_axis = NULL;

  switch (_coordsys) {
    case CS_default:
      break;
    case CS_yup_right:
      up_axis = new TiXmlElement("up_axis");
      up_axis->LinkEndChild(new TiXmlText("Y_UP"));
      break;
    case CS_zup_right:
      up_axis = new TiXmlElement("up_axis");
      up_axis->LinkEndChild(new TiXmlText("Z_UP"));
      break;
    case CS_zup_left:
    case CS_yup_left:
    case CS_invalid:
    default:
      collada_cat.error() << "Invalid coordinate system value!\n";
  }

  if (up_axis) {
    xelement->LinkEndChild(up_axis);
  }

  //FIXME: add required <created> and <modified> elements

  return xelement;
}
