// Filename: colladaAsset.I
// Created by: Xidram (13Apr10)
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
//     Function: ColladaAsset::clear
//       Access: Public
//  Description: Resets the ColladaAsset to its initial state.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
clear() {
  ColladaElement::clear();
  _title.clear();
  _subject.clear();
  _keywords.clear();
  _revision.clear();
  _coordsys = CS_default;
  time_t now = time(NULL);
  _created = *gmtime(&now);
  _modified = *gmtime(&now);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_title
//       Access: Public
//  Description: Returns title information for the asset.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaAsset::
get_title() const {
  return _title;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_title
//       Access: Public
//  Description: Sets title information for the asset.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_title(const string &title) {
  _title = title;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_subject
//       Access: Public
//  Description: Returns a description of the topical subject of the
//               asset.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaAsset::
get_subject() const {
  return _subject;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_subject
//       Access: Public
//  Description: Sets a description of the topical subject of the asset.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_subject(const string &subject) {
  _subject = subject;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_keywords
//       Access: Public
//  Description: Returns a string of a list of words used as search
//               criteria for the asset.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaAsset::
get_keywords() const {
  return _keywords;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_keywords
//       Access: Public
//  Description: Sets a string of a list of words used as search criteria
//               for the asset.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_keywords(const string &keywords) {
  _keywords = keywords;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_revision
//       Access: Public
//  Description: Returns revision information for the asset.
////////////////////////////////////////////////////////////////////
INLINE const string &ColladaAsset::
get_revision() const {
  return _revision;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_revision
//       Access: Public
//  Description: Sets revision information for the asset.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_revision(const string &revision) {
  _revision = revision;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_coordinate_system
//       Access: Public
//  Description: Returns the coordinate system in which the file
//               is defined.
////////////////////////////////////////////////////////////////////
INLINE CoordinateSystem ColladaAsset::
get_coordinate_system() const {
  return _coordsys;
}
