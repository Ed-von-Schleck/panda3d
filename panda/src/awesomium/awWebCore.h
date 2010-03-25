// Filename: awWebCore.h
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
#ifndef AWWEBCORE_H
#define AWWEBCORE_H

#include "pandabase.h"
#include "typedReferenceCount.h"
#include "luse.h"

#include "awesomium_includes.h"
#include "awWebView.h"


////////////////////////////////////////////////////////////////////
//       Class : AwWebCore
// Description : Thin wrappings arround WebCore.h
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAAWESOMIUM AwWebCore : public TypedReferenceCount, public Awesomium::WebCore {
PUBLISHED:
  /**
  * An enumeration of the two output pixel formats that WebView::render will use.
  */
  enum PixelFormat
  {
	PF_BGRA,	// BGRA byte ordering [Blue, Green, Red, Alpha]
	PF_RGBA		// RGBA byte ordering [Red, Green, Blue, Alpha]
  };

  AwWebCore(const wstring& cache_path = wstring(), 
	  const wstring& cookie_path = wstring(), 
	  const wstring& plugin_path = wstring(), 
	  const wstring& log_path = wstring(), 
	  bool enable_plugins = false,
	  const string& user_agent_override = "");
  virtual ~AwWebCore();

  void set_base_directory(const wstring& baseDirectory);
  AwWebView* create_web_view(int width, int height, bool is_transparent = false, bool enable_async_rendering = false, int max_async_render_per_sec = 70);
  void set_custom_response_page(int status_code, const string& file_path);
  const wstring& get_base_directory() const;
  void update();
  AwWebCore::PixelFormat get_pixel_format() const;
  bool are_plugins_enabled() const;
  void pause();
  void resume();
public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "AwWebCore",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif
