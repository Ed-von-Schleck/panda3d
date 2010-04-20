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
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

  nassertr(xelement->ValueStr() == T::_library_name, false);

  const TiXmlElement* xchild = xelement->FirstChildElement(T::_element_name);
  while (xchild != NULL) {
    PT(T) item = new T;
    item->load_xml(xchild);
    pvector<PointerTo<T> >::push_back(item);
    xchild = xchild->NextSiblingElement(T::_element_name);
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
  TiXmlElement * xelement = ColladaAssetElement::make_xml();
  xelement->SetValue(T::_library_name);

  for (int i = 0; i < pvector<PointerTo<T> >::size(); ++i) {
    xelement->LinkEndChild(pvector<PointerTo<T> >::at(i)->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::get_element_by_id
//       Access: Public
//  Description: Returns the element in the library that has
//               the given ID, or NULL if none found.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE PT(T) ColladaLibrary<T>::
get_element_by_id(const string &id) const {
  PT(T) res;
  for (int i = 0; i < pvector<PointerTo<T> >::size(); ++i) {
    res = pvector<PointerTo<T> >::at(i);
    if (res->get_id() == id) {
      return res;
    }
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibraryNodes::get_element_by_id
//       Access: Public
//  Description: Returns the element in the nodes library that has
//               the given ID, or NULL if none found.
//               Searches recursively for subnodes as well.
////////////////////////////////////////////////////////////////////
template<>
INLINE PT(ColladaNode) ColladaLibrary<ColladaNode>::
get_element_by_id(const string &id) const {
  PT(ColladaNode) res;
  for (int i = 0; i < pvector<PT(ColladaNode) >::size(); ++i) {
    res = pvector<PT(ColladaNode) >::at(i);
    if (res->get_id() == id) {
      return res;
    } else {
      res = res->get_element_by_id(id);
      if (res != NULL) {
        return res;
      }
    }
  }
  return NULL;
}
