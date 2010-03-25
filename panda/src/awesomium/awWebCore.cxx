// Filename: awWebCore.cxx
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

#include "config_awesomium.h"
#include "awWebCore.h"

TypeHandle AwWebCore::_type_handle;

AwWebCore::AwWebCore(const wstring& cache_path, const wstring& cookie_path, const wstring& plugin_path, 
					 const wstring& log_path, bool enable_plugins,
					 const string& user_agent_override):
Awesomium::WebCore(cache_path, cookie_path, plugin_path, log_path, Awesomium::LOG_NORMAL, enable_plugins, 
				   Awesomium::PF_BGRA, user_agent_override)
{
	//do nothing
}

AwWebCore::~AwWebCore() {
	//do nothing
}

AwWebView* AwWebCore::create_web_view(int width, int height, 
											 bool is_transparent, bool enable_async_rendering, 
											 int max_async_render_per_sec)
{
	//since awesomium has no way to query the width and height,
	//we have cache it in the wrapper.  This is why we have to 
	//give the width and height twice.
	AwWebView* new_view  = new AwWebView(Awesomium::WebCore::createWebView(width, height, 
		is_transparent, enable_async_rendering, max_async_render_per_sec), width, height);
	return new_view;
}


void AwWebCore::set_base_directory(const wstring& baseDirectory) {
	Awesomium::WebCore::setBaseDirectory(baseDirectory);
}


const wstring& AwWebCore::get_base_directory() const{
	return Awesomium::WebCore::getBaseDirectory();
}


void AwWebCore::set_custom_response_page(int status_code, const string& file_path) {
	Awesomium::WebCore::setCustomResponsePage(status_code, file_path);
}

void AwWebCore::update() {  Awesomium::WebCore::update(); }


AwWebCore::PixelFormat AwWebCore::get_pixel_format() const { return (AwWebCore::PixelFormat)Awesomium::WebCore::getPixelFormat(); }
bool AwWebCore::are_plugins_enabled() const { return Awesomium::WebCore::arePluginsEnabled(); }


void AwWebCore::pause(){  WebCore::pause(); }
void AwWebCore::resume() { WebCore::resume(); }

