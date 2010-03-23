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

  AwWebCore(const string& cache_path = "", const string& cookie_path = "", const string& plugin_path = "", 
		const string& log_path = "", bool enable_plugins = false, PixelFormat pixel_format = PF_BGRA,
		const std::string& user_agent_override = "");
  virtual ~AwWebCore();

  INLINE void set_base_directory(const string& baseDirectory);
  AwWebView* create_web_view(int width, int height, bool is_transparent = false, bool enable_async_rendering = false, int max_async_render_per_sec = 70);
  INLINE void set_custom_response_page(int status_code, const std::string& file_path);
  INLINE void update();
  INLINE AwWebCore::PixelFormat get_pixel_format() const;
  INLINE bool are_plugins_enabled() const;
  INLINE void pause();
  INLINE void resume();

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

#include "awWebCore.I"

#endif
