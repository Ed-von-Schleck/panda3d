// Filename: colladaProfile.cxx
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

#include "colladaProfile.h"

TypeHandle ColladaProfile::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaProfile::load_xml
//       Access: Published, Virtual
//  Description: Loads <profile_*> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaProfile::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  nassertr(strncmp(xelement->Value(), "profile_", 8) == 0, false);
  nassertr(string_profile(xelement->Value() + 8) == _profile, false);

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaProfile::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing
//               this profile_* element.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaProfile::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  ostringstream pstr;
  pstr << "profile_" << _profile;
  xelement->SetValue(pstr.str());

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaProfile::string_profile
//       Access: Public
//  Description: Returns the Profile value associated with the given
//               string representation, or P_common if the string
//               does not match any known Profile value.
////////////////////////////////////////////////////////////////////
ColladaProfile::Profile ColladaProfile::
string_profile(const string &string) {
  if (cmp_nocase_uh(string, "COMMON") == 0) {
    return P_common;
  } else if (cmp_nocase_uh(string, "BRIDGE") == 0) {
    return P_bridge;
  } else if (cmp_nocase_uh(string, "CG") == 0) {
    return P_cg;
  } else if (cmp_nocase_uh(string, "GLES") == 0) {
    return P_gles;
  } else if (cmp_nocase_uh(string, "GLES2") == 0) {
    return P_gles2;
  } else if (cmp_nocase_uh(string, "GLSL") == 0) {
    return P_glsl;
  } else {
    return P_common;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: Profile output operator
//  Description:
////////////////////////////////////////////////////////////////////
ostream &operator << (ostream &out, ColladaProfile::Profile profile) {
  switch (profile) {
  case ColladaProfile::P_common:
    return out << "COMMON";
  case ColladaProfile::P_bridge:
    return out << "BRIDGE";
  case ColladaProfile::P_gles:
    return out << "GLES";
  case ColladaProfile::P_gles2:
    return out << "GLES2";
  case ColladaProfile::P_glsl:
    return out << "GLSL";
  }

  nassertr(false, out);
  return out << "(**invalid**)";
}

////////////////////////////////////////////////////////////////////
//     Function: Profile input operator
//  Description:
////////////////////////////////////////////////////////////////////
istream &operator >> (istream &in, ColladaProfile::Profile &profile) {
  string word;
  in >> word;
  profile = ColladaProfile::string_profile(word);
  return in;
}

