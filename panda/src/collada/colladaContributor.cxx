// Filename: colladaContributor.cxx
// Created by: rdb (18Apr10)
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

TypeHandle ColladaContributor::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaContributor::clear
//       Access: Public
//  Description: Resets the ColladaContributor to its initial state.
////////////////////////////////////////////////////////////////////
void ColladaContributor::
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
//  Description: Loads <contributor> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaContributor::
load_xml(const TiXmlElement *xelement) {
  if (!ColladaElement::load_xml(xelement)) {
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("author");
  if (xchild != NULL && xchild->GetText()) {
    _author = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("author_email");
  if (xchild != NULL && xchild->GetText()) {
    _author_email = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("author_website");
  if (xchild != NULL && xchild->GetText()) {
    _author_website = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("authoring_tool");
  if (xchild != NULL && xchild->GetText()) {
    _authoring_tool = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("comments");
  if (xchild != NULL && xchild->GetText()) {
    _comments = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("copyright");
  if (xchild != NULL && xchild->GetText()) {
    _copyright = xchild->GetText();
  }

  xchild = xelement->FirstChildElement("source_data");
  if (xchild != NULL && xchild->GetText()) {
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
TiXmlElement *ColladaContributor::
make_xml() const {
  TiXmlElement *xelement = ColladaElement::make_xml();
  xelement->SetValue("contributor");

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

