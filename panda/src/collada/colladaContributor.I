// Filename: colladaContributor.I
// Created by: Xidram (18Apr10)
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
//     Function: ColladaContributor::clear
//       Access: Public
//  Description: Resets the ColladaContributor to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
clear() {
  ColladaElement::clear();
  _author.clear();
  _author_email.clear();
  _author_website.clear();
  _authoring_tool.clear();
  _comments.clear();
  _copyright.clear();
  _source_data.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::load_xml
//       Access: Public
//  Description: Loads <asset> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaContributor::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("author");
  if (xchild != NULL) {
    _author = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("author_email");
  if (xchild != NULL) {
    _author_email = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("author_website");
  if (xchild != NULL) {
    _author_website = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("authoring_tool");
  if (xchild != NULL) {
    _authoring_tool = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("comments");
  if (xchild != NULL) {
    _comments = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("copyright");
  if (xchild != NULL) {
    _copyright = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("source_data");
  if (xchild != NULL) {
    _source_data = xchild->GetText();
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               this element.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaContributor::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();

  TiXmlElement *xchild;

  if (!_author.empty()) {
    xchild = new TiXmlElement("author");
    xchild->LinkEndChild(new TiXmlText(_author));
    xelement->LinkEndChild(xchild);
  }

  if (!_author_email.empty()) {
    xchild = new TiXmlElement("author_email");
    xchild->LinkEndChild(new TiXmlText(_author_email));
    xelement->LinkEndChild(xchild);
  }

  if (!_author_website.empty()) {
    xchild = new TiXmlElement("author_website");
    xchild->LinkEndChild(new TiXmlText(_author_website));
    xelement->LinkEndChild(xchild);
  }

  if (!_authoring_tool.empty()) {
    xchild = new TiXmlElement("authoring_tool");
    xchild->LinkEndChild(new TiXmlText(_authoring_tool));
    xelement->LinkEndChild(xchild);
  }

  if (!_comments.empty()) {
    xchild = new TiXmlElement("comments");
    xchild->LinkEndChild(new TiXmlText(_comments));
    xelement->LinkEndChild(xchild);
  }

  if (!_copyright.empty()) {
    xchild = new TiXmlElement("copyright");
    xchild->LinkEndChild(new TiXmlText(_copyright));
    xelement->LinkEndChild(xchild);
  }

  if (!_source_data.empty()) {
    xchild = new TiXmlElement("source_data");
    xchild->LinkEndChild(new TiXmlText(_source_data));
    xelement->LinkEndChild(xchild);
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_author
//       Access: Public
//  Description: Returns the name of the contributor.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_author() const {
  return _author;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_author
//       Access: Public
//  Description: Sets the name of the contributor.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_author(const string &author) {
  _author = author;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_author_email
//       Access: Public
//  Description: Returns the e-mail of the contributor.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_author_email() const {
  return _author_email;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_author_email
//       Access: Public
//  Description: Sets the e-mail of the contributor.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_author_email(const string &author_email) {
  _author_email = author_email;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_author_website
//       Access: Public
//  Description: Returns the URI of the contributor's
//               website.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_author_website() const {
  return _author_website;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_author_website
//       Access: Public
//  Description: Sets the URI of the contributor's website.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_author_website(const string &author_website) {
  _author_website = author_website;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_authoring_tool
//       Access: Public
//  Description: Returns the name of the tool used to
//               author the asset in the file.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_authoring_tool() const {
  return _authoring_tool;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_authoring_tool
//       Access: Public
//  Description: Sets the name of the tool used to author the asset in
//               the file.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_authoring_tool(const string &authoring_tool) {
  _authoring_tool = authoring_tool;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_comments
//       Access: Public
//  Description: Returns the comments written by the
//               contributor in the file.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_comments() const {
  return _comments;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_comments
//       Access: Public
//  Description: Sets the comments written by the contributor in the
//               file.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_comments(const string &comments) {
  _comments = comments;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_copyright
//       Access: Public
//  Description: Returns the contributor's copyright
//               information.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_copyright() const {
  return _copyright;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_copyright
//       Access: Public
//  Description: Sets the contributor's copyright information.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_copyright(const string &copyright) {
  _copyright = copyright;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::get_source_data
//       Access: Public
//  Description: Returns the URI of the source data used
//               for the asset in this file.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaContributor::
get_source_data() const {
  return _source_data;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::set_source_data
//       Access: Public
//  Description: Sets the URI of the source data used for the asset
//               in this file.
////////////////////////////////////////////////////////////////////
INLINE void ColladaContributor::
set_source_data(const string &source_data) {
  _source_data = source_data;
}

