// Filename: colladaInstance.I
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

#include "colladaGeometry.h"


template<class T>
TypeHandle ColladaInstance<T>::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::get_target_type
//       Access: Public
//  Description: Returns a TypeHandle value containing the
//               type of the target that this instance points to.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE TypeHandle ColladaInstance<T>::
get_target_type() const {
  return T::get_class_type();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::clear
//       Access: Published, Virtual
//  Description: Resets the ColladaInstance to its initial state.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE void ColladaInstance<T>::
clear () {
  ColladaElement::clear();
  _url.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::is_empty
//       Access: Public
//  Description: Returns true if this is a void instance, e.g.
//               it doesn't point to anything, has an empty URL.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE bool ColladaInstance<T>::
is_empty() const {
  return _url.empty();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::get_url
//       Access: Public
//  Description: Returns the URL associated with this instance.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE const string &ColladaInstance<T>::
get_url() const {
  return _url;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::set_url
//       Access: Public
//  Description: Sets the URL associated with this instance.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE void ColladaInstance<T>::
set_url(const string &url) {
  _url = url;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::get_target
//       Access: Public
//  Description: Returns the element of which this is an instance,
//               or NULL if it failed to resolve.
//               The instance needs to be in a document in order
//               to resolve itself - that is, get_document()
//               should not return NULL;
////////////////////////////////////////////////////////////////////
template<class T>
INLINE PT(ColladaElement) ColladaInstance<T>::
get_target() const {
  CPT(ColladaDocument) doc = get_document();
  nassertr(doc != NULL, NULL);
  return doc->resolve_instance(this);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::load_xml
//       Access: Published, Virtual
//  Description: Loads the items in the library from the given
//               XML element.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE bool ColladaInstance<T>::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  nassertr(xelement->ValueStr() == "instance_" + T::_element_name, false);

  const char* url = xelement->Attribute("url");
  if (url) {
    _url = url;
  } else {
    _url = "";
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaInstance::make_xml
//       Access: Published, Virtual
//  Description: Returns a new TiXmlElement representing the instance.
////////////////////////////////////////////////////////////////////
template<class T>
INLINE TiXmlElement *ColladaInstance<T>::
make_xml() const {
  TiXmlElement * xelement = ColladaElement::make_xml();
  xelement->SetValue("instance_" + T::_element_name);

  xelement->SetAttribute("url", _url);

  return xelement;
}

