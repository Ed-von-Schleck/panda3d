// Filename: colladaLight.cxx
// Created by: Xidram (20Apr10)
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

#include "config_collada.h"
#include "colladaLight.h"
#include "ambientLight.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotlight.h"

TypeHandle ColladaLight::_type_handle;
const string ColladaLight::_element_name ("light");
const string ColladaLight::_library_name ("library_lights");

////////////////////////////////////////////////////////////////////
//     Function: ColladaLight::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaLight::
ColladaLight() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLight::load_xml
//       Access: Public
//  Description: Loads <light> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaLight::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "light", false);

  const TiXmlElement *xcommon_tech = xelement->FirstChildElement("technique_common");
  nassertr(xcommon_tech != NULL, false);
  const TiXmlElement *xlighttype = xcommon_tech->FirstChildElement();
  nassertr(xlighttype != NULL, false);

  if (xlighttype->ValueStr() == "ambient") {
    
    _light_type = LT_ambient;
  }
  else if (xlighttype->ValueStr() == "directional") {
    _light_type = LT_directional;
  }
  else if (xlighttype->ValueStr() == "point") {
    _light_type = LT_point;
  }
  else if (xlighttype->ValueStr() == "spot") {
    _light_type = LT_spot;
  }
  else {
    collada_cat.error() << "Unknown light type '" << xlighttype->ValueStr() << "'\n";
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLight::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the light.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaLight::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("light");

  TiXmlElement *xcommon_tech = new TiXmlElement("technique_common");
  TiXmlElement *xlight_type;
  switch (_light_type) {
    default:
      collada_cat.error() << "Invalid light type\n";
      break;
    case LT_ambient:
      xlight_type = new TiXmlElement("ambient");
      break;
    case LT_directional:
      xlight_type = new TiXmlElement("directional");
      break;
    case LT_point:
      xlight_type = new TiXmlElement("point");
      break;
    case LT_spot:
      xlight_type = new TiXmlElement("spot");
      break;
    }
  xcommon_tech->LinkEndChild(xlight_type);
  xelement->LinkEndChild(xcommon_tech);

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLight::make_node
//       Access: Public
//  Description: Returns a new PandaNode representing this
//               COLLADA light and the children below it.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaLight::
make_node() const {
  PT(PandaNode) light = NULL;
  switch (_light_type) {
    case LT_ambient:
      light = new AmbientLight(get_name());
      break;
    case LT_directional:
      light = new DirectionalLight(get_name());
      break;
    case LT_point:
      light = new PointLight(get_name());
      break;
    case LT_spot:
      light = new Spotlight(get_name());
      break;
  }

  return light;
}
