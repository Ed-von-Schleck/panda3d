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
#include "WebCore.h"

TypeHandle AwWebCore::_type_handle;

std::wstring convert_to_wide_string(string& s){
	return wstring(s.begin(), s.end());
}

AwWebCore::AwWebCore(const string& cache_path, const string& cookie_path, const string& plugin_path, 
		const string& log_path, bool enable_plugins, PixelFormat pixel_format,
		const std::string& user_agent_override):
Awesomium::WebCore(convert_to_wide_string(cache_path), convert_to_wide_string(cookie_path),
				   convert_to_wide_string(plugin_path), convert_to_wide_string(log_path),
				   enable_plugins, pixel_format, convert_to_wide_string(user_agent_override))
{
	//do nothing
}

AwWebCore::~AwWebCore() {
	//do nothing
}