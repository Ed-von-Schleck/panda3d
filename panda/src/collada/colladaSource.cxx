// Filename: colladaSource.cxx
// Created by:  rdb (29May10)
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

#include "colladaSource.h"

TypeHandle ColladaSource::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaSource::
ColladaSource() {
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::clear
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
void ColladaSource::
clear() {
  ColladaAssetElement::clear();
  _array = NULL;
  _accessor = NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::load_xml
//       Access: Public
//  Description: Loads <source> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaSource::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  bool okflag = true;
  const TiXmlElement* xchild;

  // Read out the array element
  xchild = xelement->FirstChildElement();
  while (xchild != NULL) {
    PT(ColladaArrayBase) array = NULL;
    if (cmp_nocase_uh(xchild->Value(), "bool_array") == 0) {
      array = new ColladaBoolArray;
    } else if (cmp_nocase_uh(xchild->Value(), "float_array") == 0) {
      array = new ColladaFloatArray;
    } else if (cmp_nocase_uh(xchild->Value(), "IDREF_array") == 0) {
      array = new ColladaIdrefArray;
    } else if (cmp_nocase_uh(xchild->Value(), "int_array") == 0) {
      array = new ColladaIntArray;
    } else if (cmp_nocase_uh(xchild->Value(), "Name_array") == 0) {
      array = new ColladaNameArray;
    } else if (cmp_nocase_uh(xchild->Value(), "SIDREF_array") == 0) {
      array = new ColladaSidrefArray;
    } else if (cmp_nocase_uh(xchild->Value(), "token_array") == 0) {
      array = new ColladaTokenArray;
    }
    if (array != NULL) {
      if (_array != NULL) {
        collada_cat.error() << "Multiple arrays found in <source> element!\n";
        okflag = false;
        break;
      }
      _array = array;
      _array->_parent = this;
      _array->load_xml(xchild);
    }
    xchild = xchild->NextSiblingElement();
  }

  xchild = xelement->FirstChildElement("technique_common");
  if (xchild != NULL) {
    xchild = xchild->FirstChildElement("accessor");
    if (xchild != NULL) {
      _accessor = new ColladaAccessor;
      _accessor->_parent = this;
      _accessor->load_xml(xchild);
    }
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement *ColladaSource::
make_xml() const {
  TiXmlElement *xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("source");

  if (_array != NULL) {
    xelement->LinkEndChild(_array->make_xml());
  }

  if (_accessor != NULL) {
    TiXmlElement *xchild = new TiXmlElement("technique_common");
    xchild->LinkEndChild(_accessor->make_xml());
    xelement->LinkEndChild(xchild);
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaSource::get_element_by_id
//       Access: Published, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
PT(ColladaElement) ColladaSource::
get_element_by_id(const string &id) const {
  PT(ColladaElement) result = NULL;

  if (_array != NULL) {
    if (_array->get_id() == id) {
      return DCAST(ColladaElement, _array);
    }
    result = _array->get_element_by_id(id);
    if (result != NULL) {
      return result;
    }
  }

  // We're not checking _accessor because according to
  // the COLLADA spec it is not supposed to have an ID.

  return NULL;
}

