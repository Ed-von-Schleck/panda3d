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
//     Function: ColladaAsset::has_geographic_location
//       Access: Public
//  Description: Returns true if <geographic_location> is defined in
//               this asset and false if it is not.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaAsset::
has_geographic_location() const {
  return _has_geographic_location;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_longitude
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE double ColladaAsset::
get_longitude() const {
  return _longitude;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_latitude
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE double ColladaAsset::
get_latitude() const {
  return _latitude;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_altitude
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE double ColladaAsset::
get_altitude() const {
  return _altitude;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_altitude_mode
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE ColladaAsset::AltitudeMode ColladaAsset::
get_altitude_mode() const {
  nassertr(_has_geographic_location, AM_relativeToGround);
  return _altitude_mode;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_geographic_location
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_geographic_location(double longitude, double latitude, double altitude,
                        AltitudeMode altitude_mode) {
  _longitude = longitude;
  _latitude = latitude;
  _altitude = altitude;
  _altitude_mode = altitude_mode;
  _has_geographic_location = true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::clear_geographic_location
//       Access: Public
//  Description: Disables the association of the asset to a
//               certain geographical location.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
clear_geographic_location() {
  _has_geographic_location = false;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::has_unit_meter
//       Access: Public
//  Description: Returns true if <unit> is defined in this asset and
//               false if it is not.
////////////////////////////////////////////////////////////////////
INLINE bool ColladaAsset::
has_unit() const {
  return _has_unit;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_unit_meter
//       Access: Public
//  Description: Returns the number of real-world meters in one unit of
//               distance for elements and objects. This unit of distance
//               applies to all spatial measurements within the scope of
//               the <asset>'s parent element unless overridden by a more
//               local <asset>/<unit>. The default value is 1.0.
////////////////////////////////////////////////////////////////////
INLINE double ColladaAsset::
get_unit_meter() const {
  nassertr(_has_unit, 1.0);
  return _unit_meter;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::get_unit_name
//       Access: Public
//  Description: Returns the name of the unit of distance for elements
//               and objects.
////////////////////////////////////////////////////////////////////
INLINE const string ColladaAsset::
get_unit_name() const {
  nassertr(_has_unit, "meter");
  return _unit_name;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::set_unit
//       Access: Public
//  Description: Sets the unit of distance for elements and objects.
//               The first parameter is the number of real-world meters
//               in one unit of distance. The second parameter is the
//               name of the unit of distance.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
set_unit(double unit_meter, const string &unit_name) {
  _unit_meter = unit_meter;
  _unit_name = unit_name;
  _has_unit = true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaAsset::clear_unit
//       Access: Public
//  Description: Disables the custom unit specification.
////////////////////////////////////////////////////////////////////
INLINE void ColladaAsset::
clear_unit() {
  _has_unit = false;
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

