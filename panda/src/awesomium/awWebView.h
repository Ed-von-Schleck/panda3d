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
#ifndef AWWEBVIEW_H
#define AWWEBVIEW_H

#include "pandabase.h"
#include "typedReferenceCount.h"
#include "luse.h"

#include "WebCore.h"

class WebViewListener;

////////////////////////////////////////////////////////////////////
//       Class : AwWebView
// Description : Thin bindings, wraps a WebView * returned from WebCore.createWebView
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAAWESOMIUM AwWebView : public TypedReferenceCount{
PUBLISHED:

/**
* Mouse button enumerations, used with WebView::injectMouseDown 
* and WebView::injectMouseUp
*/
enum MouseButton {
	LEFT_MOUSE_BTN,
	MIDDLE_MOUSE_BTN,
	RIGHT_MOUSE_BTN
};

/**
* URL Filtering mode enumerations, used by WebView::setURLFilteringMode
*/
enum URLFilteringMode {
	/**
	* All resource requests and page navigations are ALLOWED 
	* except those that match the URL filters specified.
	*/
	UFM_BLACKLIST, 

	/**
	* All resource requests and page navigations are DENIED 
	* except those that match the URL filters specified.
	*/
	UFM_WHITELIST,

	// No filtering
	UFM_NONE
};

public:
  INLINE void render(void* destination, int destRowSpan, int destDepth);
  INLINE bool is_dirty();

PUBLISHED:
  AwWebView(Awesomium::WebView* web_view);
  AwWebView();
  virtual ~AwWebView();

  INLINE void destroy(void);
  //INLINE void setListener(Awesomium::WebViewListener* listener);
  //INLINE Awesomium::WebViewListener* getListener();
  INLINE void load_URL(const string& url, const string& frame_name ="", const string& username="" , const string& password="");
  INLINE void load_HTML(const string& html, const string& frame_name = "");
  INLINE void load_file(const string& file, const string& frame_name = "" );
  INLINE void go_to_history_offset(int offset);
  INLINE void execute_javascript(const std::string& javascript, const std::string& frame_name = "" );
  //TODO: implement javascript calling commented blocks properly
  /*
  Awesomium::FutureJSValue execute_javascript_with_result(const std::string& javascript, const std::string& frame_name = "");
  void call_javascript_function(const string& function, const JSArguments& args, const string frame_name="");
  void create_object(const string& object_name);
  void destroy_object(const string& object_name);
  void set_object_property(const string& object_name, const string& prop_name, const JSValue& value);
  void set_object_callback(const string& object_name, const string& callback_name);
  */

  void inject_mouse_move(int x, int y);
  void inject_mouse_down(AwWebView::MouseButton button);
  void inject_mouse_up(AwWebView::MouseButton button);
  void inject_mouse_wheel(int scroll_amount);
  //TODO: implement inject_keyboard_event
  //void inject_keyboard_event(const );
  void cut();
  void copy();
  void paste();
  void select_all();
  void deselect_all()
  
  //TODO: implement these non-critical methods
  /*
  void get_content_as_text(int max_chars);
  void zoom_in();
  void zoom_out();
  void reset_zoom();
  void resize(int width, int height);
  void unfocus();
  void focus();
  */
  void set_transparent(bool is_transparent);
  /*
  void set_URL_filter_mode(URLFilteringMode mode);
  void add_URL_filter(const string& filter);
  void clear_all_URL_filters();
  void set_header_definition(const string& name, const HeaderDefinition& definition);
  void add_header_rewrite_rule(const string& rule, const string& name);
  void remove_header_rewrite_rule(const string& rule);
  void remove_header_rewrite_rules_by_definition_name(const string& name);
  void set_opens_external_links_in_calling_frame(bool is_enabled);
  */
  
protected:
  Awesomium::WebView* _web_view


public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    TypedReferenceCount::init_type();
    register_type(_type_handle, "AwWebView",
                  TypedReferenceCount::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#include "awWebView.I"

#endif
