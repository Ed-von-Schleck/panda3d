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

