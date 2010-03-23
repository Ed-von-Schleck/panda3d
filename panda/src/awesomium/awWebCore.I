// Filename: awWebCore.I
// Created by:  rurbino (12Oct09)
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


INLINE void AwWebCore::set_base_directory(const string& baseDirectory) {
	Awesomium::WebCore::setBaseDirectory(baseDirectory);
}



INLINE void AwWebCore::set_custom_response_page(int status_code, const string& file_path) {
	Awesomium::WebCore::setCustomResponsePage(status_code, file_path);
}

INLINE void AwWebCore::update() {  Awesomium::WebCore::update(); }


INLINE AwWebCore::PixelFormat AwWebCore::get_pixel_format() const { return (AwWebCore::PixelFormat)Awesomium::WebCore::getPixelFormat(); }
INLINE bool AwWebCore::are_plugins_enabled() const { return Awesomium::WebCore::arePluginsEnabled(); }


INLINE void AwWebCore::pause(){  WebCore::pause(); }
INLINE void AwWebCore::resume() { WebCore::resume(); }


