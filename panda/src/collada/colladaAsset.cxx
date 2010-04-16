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

// Representation in an ISO 8601 format as per xs:dateTime
// Don't append the trailing 'Z' here.
#define TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaAsset::
ColladaAsset() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::load_xml
//       Access: Public
//  Description: Loads <asset> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaAsset::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  nassertr(xelement->ValueStr() == "asset", false);

  _coordsys = CS_default;
  bool okflag = true;
  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("up_axis");
  if (xchild != NULL) {
    const char* coordsys = xchild->GetText();
    _coordsys = parse_coordinate_system_string(coordsys);
    if (_coordsys == CS_invalid || _coordsys == CS_default) {
      collada_cat.error()
        << "Invalid coordinate system value: " << coordsys << "\n";
      okflag = false;
    }
  }

  // Get the created & modified timestamps
  // Note: we're assuming the time is stored as UTC.
  xchild = xelement->FirstChildElement("created");
  if (xchild != NULL) {
    string datetime (xchild->GetText());
    if (datetime[datetime.size() - 1] == 'Z') {
      datetime = datetime.substr(0, datetime.size() - 1);
    }
    const char* result = strptime(datetime.c_str(), TIME_FORMAT, &_created);
    if (result == NULL || strlen(result) != 0) {
      collada_cat.error()
        << "Invalid <created> time: '" << xchild->GetText() << "'\n";
      if (result != NULL) {
        collada_cat.error()
          << "Unprocessed characters: '" << result << "'\n";
      }
      okflag = false;
    }
  }

  xchild = xelement->FirstChildElement("modified");
  if (xchild != NULL) {
    string datetime (xchild->GetText());
    if (datetime[datetime.size() - 1] == 'Z') {
      datetime = datetime.substr(0, datetime.size() - 1);
    }
    const char* result = strptime(datetime.c_str(), TIME_FORMAT, &_modified);
    if (result == NULL || strlen(result) != 0) {
      collada_cat.error()
        << "Invalid <modified> time: '" << xchild->GetText() << "'\n";
      if (result != NULL) {
        collada_cat.error()
          << "Unprocessed characters: '" << result << "'\n";
      }
      okflag = false;
    }
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the asset.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaAsset::
make_xml() const {
  TiXmlElement * xelement = ColladaElement::make_xml();
  xelement->SetValue("asset");

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

  // Add the required <created> and <modified> elements
  time_t now = time(NULL);
  char created[512];
  char modified[512];
  strftime(created, 512, TIME_FORMAT, &_created);
  strftime(modified, 512, TIME_FORMAT, gmtime(&now));
  strcat(created, "Z");
  strcat(modified, "Z");
  TiXmlElement * xcreated = new TiXmlElement("created");
  TiXmlElement * xmodified = new TiXmlElement("modified");
  xcreated->LinkEndChild(new TiXmlText(created));
  xmodified->LinkEndChild(new TiXmlText(modified));
  xelement->LinkEndChild(xcreated);
  xelement->LinkEndChild(xmodified);

  return xelement;
}
