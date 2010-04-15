// Filename: colladaLibrary.I
// Created by:  rdb (14Apr10)
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

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::clear
//       Access: Public
//  Description: Resets the ColladaLibrary to its initial state.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE void ColladaLibrary<T>::
clear () {
  ColladaAssetElement::clear();
  pvector<PointerTo<T> >::clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::load_xml
//       Access: Public
//  Description: Loads the items in the library from the given
//               XML element.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE bool ColladaLibrary<T>::
load_xml(const TiXmlElement *xelement) {
  // First, dispense with any items we had previously.  We will
  // replace them with the new items.
  clear();

  nassertr(xelement != NULL, false);
  nassertr(xelement->ValueStr() == T::_library_name, false);

  const TiXmlElement* xchild = xelement->FirstChildElement(T::_element_name);
  while (xchild != NULL) {
    PT(T) item = new T;
    item->load_xml(xchild);
    pvector<PointerTo<T> >::push_back(item);
    xchild = xchild->NextSiblingElement(T::_element_name);
  }

  xchild = xelement->FirstChildElement("asset");
  if (xchild != NULL) {
    _asset = new ColladaAsset();
    _asset->load_xml(xchild);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the library and its children.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE TiXmlElement * ColladaLibrary<T>::
make_xml() const {
  TiXmlElement * xelement = new TiXmlElement(T::_library_name);

  if (_asset) {
    xelement->LinkEndChild(_asset->make_xml());
  }

  for (int i = 0; i < pvector<PointerTo<T> >::size(); ++i) {
    xelement->LinkEndChild(pvector<PointerTo<T> >::at(i)->make_xml());
  }

  return xelement;
}

