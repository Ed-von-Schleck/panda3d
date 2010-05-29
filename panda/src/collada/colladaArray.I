// Filename: colladaArray.I
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

#include "string_utils.h"

template <ColladaArrayType arrayType, class itemType>
TypeHandle ColladaArray<arrayType, itemType>::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::get_array_type
//       Access: Published, Virtual
//  Description: Method that returns the type of this array.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE ColladaArrayType ColladaArray<arrayType, itemType>::
get_array_type() const {
  return arrayType;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::clear
//       Access: Published, Virtual
//  Description: Empties the array, and resets the element to its
//               initial state. Also clears the ID and name
//               associated with the element! Use clear_array
//               if you just want to empty the array.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE void ColladaArray<arrayType, itemType>::
clear() {
  pvector<itemType>::clear();
  ColladaElement::clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::clear_array
//       Access: Published, Virtual
//  Description: Empties the array.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE void ColladaArray<arrayType, itemType>::
clear_array() {
  pvector<itemType>::clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::load_xml
//       Access: Published, Virtual
//  Description: Loads the contents of the indicated XML element
//               into the array. Returns false on failure.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE bool ColladaArray<arrayType, itemType>::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaArrayBase::load_xml(xelement)) {
    return false;
  }

  vector_string items;
  tokenize(trim(xelement->GetText()), items, COLLADA_WHITESPACE, true);

  for (vector_string::iterator it = items.begin(); it != items.end(); ++it) {
    pvector<itemType>::push_back(string_to_item(*it));
  }

  int count;
  if (xelement->QueryIntAttribute("count", &count) != TIXML_SUCCESS) {
    collada_cat.warning()
      << "Invalid count attribute in <" << xelement->Value() << ">\n";
  } else if (pvector<itemType>::size() != count) {
    collada_cat.warning()
      << "Count value " << count << " in <" << xelement->Value() << ">"
      << " does not match array length " << pvector<itemType>::size() << "\n";
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::load_xml
//       Access: Published, Virtual
//  Description: Loads the contents of the indicated XML element
//               into the array. Returns false on failure.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE TiXmlElement *ColladaArray<arrayType, itemType>::
make_xml() const {
  TiXmlElement *xelement = ColladaArrayBase::make_xml();
  switch (arrayType) {
    case AT_bool:
      xelement->SetValue("bool_array");
      break;
    case AT_float:
      xelement->SetValue("float_array");
      break;
    case AT_IDREF:
      xelement->SetValue("IDREF_array");
      break;
    case AT_int:
      xelement->SetValue("int_array");
      break;
    case AT_Name:
      xelement->SetValue("Name_array");
      break;
    case AT_SIDREF:
      xelement->SetValue("SIDREF_array");
      break;
    case AT_token:
      xelement->SetValue("token_array");
      break;
    default:
      collada_cat.error() << "Unknown array type!\n";
      return NULL;
  }

  xelement->SetAttribute("count", pvector<itemType>::size());

  ostringstream contents;
  for (size_t i = 0; i < pvector<itemType>::size(); ++i) {
    if (i != 0) {
      contents << " ";
    }
    contents << item_to_string(pvector<itemType>::at(i));
  }
  xelement->LinkEndChild(new TiXmlText(contents.str()));

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::string_to_item
//       Access: Private, Virtual
//  Description: Converts string to itemType.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE itemType ColladaArray<arrayType, itemType>::
string_to_item(const string &str) const {
  itemType item;
  istringstream buffer (str);
  buffer >> item;
  return item;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaArray::item_to_string
//       Access: Private, Virtual
//  Description: Converts itemType to string.
////////////////////////////////////////////////////////////////////
template <ColladaArrayType arrayType, class itemType>
INLINE string ColladaArray<arrayType, itemType>::
item_to_string(const itemType &val) const {
  ostringstream buffer;
  buffer << val;
  return buffer.str();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaBoolArray::string_to_item
//       Access: Private
//  Description: Converts string to boolean.
////////////////////////////////////////////////////////////////////
template<>
INLINE bool ColladaArray<AT_bool, bool>::
string_to_item(const string &str) const  {
  return (cmp_nocase(str, "true") == 0 || str == "1");
}

