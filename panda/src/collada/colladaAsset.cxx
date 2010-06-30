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
//     Function: ColladaAsset::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaAsset to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaAsset::
clear() {
  ColladaElement::clear();
  _title.clear();
  _subject.clear();
  _keywords.clear();
  _revision.clear();
  _has_geographic_location = false;
  _longitude = NULL;
  _latitude = NULL;
  _altitude = NULL;
  _altitude_mode = AM_relativeToGround;
  _has_unit = false;
  _unit_meter = 1.0;
  _unit_name = "meter";
  _coordsys = CS_default;
  time_t now = time(NULL);
  _created = *gmtime(&now);
  _modified = *gmtime(&now);
  _contributors.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::load_xml
//       Access: Published, Virtual
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

  xchild = xelement->FirstChildElement("title");
  if (xchild != NULL && xchild->GetText()) {
    _title = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("subject");
  if (xchild != NULL && xchild->GetText()) {
    _subject = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("keywords");
  if (xchild != NULL && xchild->GetText()) {
    _keywords = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("revision");
  if (xchild != NULL && xchild->GetText()) {
    _revision = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("unit");
  if (xchild != NULL) {
    _has_unit = true;
    if (xchild->Attribute("name")) {
      _unit_name = xchild->Attribute("name");
    }
    if (xchild->QueryDoubleAttribute("meter", &_unit_meter) == TIXML_WRONG_TYPE) {
      collada_cat.error()
        << "Invalid value " << xchild->Attribute("name") << " for unit name\n";
    }
  }

  //TODO: read out geographic_location

  xchild = xelement->FirstChildElement("up_axis");
  if (xchild != NULL && xchild->GetText()) {
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
  if (xchild != NULL && xchild->GetText()) {
    string datetime (xchild->GetText());
    if (datetime[datetime.size() - 1] == 'Z') {
      datetime = datetime.substr(0, datetime.size() - 1);
    }
    const char* result = strptime(datetime.c_str(), TIME_FORMAT, &_created);
    if (result == NULL || strlen(result) != 0) {
      collada_cat.warning()
        << "Invalid <created> time: '" << xchild->GetText() << "'\n";
      if (result != NULL) {
        collada_cat.warning()
          << "Unprocessed characters: '" << result << "'\n";
      }
    }
  }

  xchild = xelement->FirstChildElement("modified");
  if (xchild != NULL && xchild->GetText()) {
    string datetime (xchild->GetText());
    if (datetime[datetime.size() - 1] == 'Z') {
      datetime = datetime.substr(0, datetime.size() - 1);
    }
    const char* result = strptime(datetime.c_str(), TIME_FORMAT, &_modified);
    if (result == NULL || strlen(result) != 0) {
      collada_cat.warning()
        << "Invalid <modified> time: '" << xchild->GetText() << "'\n";
      if (result != NULL) {
        collada_cat.warning()
          << "Unprocessed characters: '" << result << "'\n";
      }
    }
  }

  // Now the list of contributors
  xchild = xelement->FirstChildElement("contributor");
  while (xchild != NULL) {
    PT(ColladaContributor) contrib = new ColladaContributor;
    contrib->_parent = this;
    contrib->load_xml(xchild);
    _contributors.push_back(contrib);
    xchild = xchild->NextSiblingElement("contributor");
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               the asset.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaAsset::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("asset");

  TiXmlElement *xchild;

  if (!_title.empty()) {
    xchild = new TiXmlElement("title");
    xchild->LinkEndChild(new TiXmlText(_title));
    xelement->LinkEndChild(xchild);
  }

  if (!_subject.empty()) {
    xchild = new TiXmlElement("subject");
    xchild->LinkEndChild(new TiXmlText(_subject));
    xelement->LinkEndChild(xchild);
  }

  if (!_keywords.empty()) {
    xchild = new TiXmlElement("keywords");
    xchild->LinkEndChild(new TiXmlText(_keywords));
    xelement->LinkEndChild(xchild);
  }

  if (!_revision.empty()) {
    xchild = new TiXmlElement("revision");
    xchild->LinkEndChild(new TiXmlText(_revision));
    xelement->LinkEndChild(xchild);
  }

  // Specify the <unit> and <geographic_location> if requested
  if (_has_unit) {
    xchild = new TiXmlElement("unit");
    xchild->SetDoubleAttribute("meter", _unit_meter);
    xchild->SetAttribute("name", _unit_name);
    xelement->LinkEndChild(xchild);
  }
  if (_has_geographic_location) {
    xchild = new TiXmlElement("coverage");
    TiXmlElement *xgloc = new TiXmlElement("geographic_location");
    TiXmlElement *xglocchild;
    ostringstream slong, slat, salt;
    slong << _longitude;
    xglocchild = new TiXmlElement("longitude");
    xglocchild->LinkEndChild(new TiXmlText(slong.str()));
    xgloc->LinkEndChild(xglocchild);
    slat << _latitude;
    xglocchild = new TiXmlElement("latitude");
    xglocchild->LinkEndChild(new TiXmlText(slat.str()));
    xgloc->LinkEndChild(xglocchild);
    salt << _altitude;
    xglocchild = new TiXmlElement("altitude");
    switch (_altitude_mode) {
      case AM_absolute:
        xglocchild->SetAttribute("mode", "absolute");
        break;
      case AM_relativeToGround:
        xglocchild->SetAttribute("mode", "relativeToGround");
        break;
      default:
        collada_cat.error() << "Invalid altitude mode!\n";
    }
    xglocchild->LinkEndChild(new TiXmlText(salt.str()));
    xgloc->LinkEndChild(xglocchild);
    xchild->LinkEndChild(xgloc);
    xelement->LinkEndChild(xchild);
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

  // Add the coordinate system (via the <up_axis> element)
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

  for (int i = 0; i < _contributors.size(); ++i) {
    xelement->LinkEndChild(_contributors[i]->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_element_by_id
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaAsset::
get_element_by_id(const string &id) const {
  for (int i = 0; i < _contributors.size(); ++i) {
    if (_contributors[i]->get_id() == id) {
      return DCAST(ColladaElement, _contributors[i]);
    }
  }
  return NULL;
}
