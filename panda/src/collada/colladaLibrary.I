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


template<class T>
TypeHandle ColladaLibrary<T>::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::clear
//       Access: Published, Virtual
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
//       Access: Published, Virtual
//  Description: Loads the items in the library from the given
//               XML element.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE bool ColladaLibrary<T>::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }

#ifndef NDEBUG
  nassertr(xelement->ValueStr() == T::_library_name || xelement->ValueStr() == "library", false);
  if (xelement->ValueStr() == "library") {
    nassertr(cmp_nocase(xelement->Attribute("type"), T::_element_name)== 0, false);
  }
#endif

  const TiXmlElement* xchild = xelement->FirstChildElement(T::_element_name);
  while (xchild != NULL) {
    PT(T) item = new T;
    item->_parent = this;
    item->load_xml(xchild);
    pvector<PointerTo<T> >::push_back(item);
    xchild = xchild->NextSiblingElement(T::_element_name);
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaLibrary::make_xml
//       Access: Published, Virtual
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
//               Searches recursively.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE PT(ColladaElement) ColladaLibrary<T>::
get_element_by_id(const string &id) const {
  PT(ColladaElement) res;
  for (int i = 0; i < pvector<PointerTo<T> >::size(); ++i) {
    res = pvector<PointerTo<T> >::at(i);
    if (res->get_id() == id) {
      return DCAST(ColladaElement, res);
    } else {
      res = res->get_element_by_id(id);
      if (res != NULL) {
        return DCAST(ColladaElement, res);
      }
    }
  }
  return NULL;
}
