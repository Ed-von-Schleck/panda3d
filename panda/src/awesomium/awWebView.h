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

#include "WebView.h"

#define DEFAULT_WEBVIEW_WIDTH 512
#define DEFAULT_WEBVIEW_HEIGHT 512

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
  void render(void* destination, int destRowSpan, int destDepth);
  bool is_dirty();

PUBLISHED:
  AwWebView(Awesomium::WebView* web_view=NULL, unsigned int width=DEFAULT_WEBVIEW_WIDTH, unsigned int height=DEFAULT_WEBVIEW_HEIGHT);
  virtual ~AwWebView();

  void destroy(void);
  //void setListener(Awesomium::WebViewListener* listener);
  //Awesomium::WebViewListener* getListener();
  void load_URL(const string& url, const wstring& frame_name = wstring(), const string& username="" , const string& password="");
  void load_HTML(const string& html, const wstring& frame_name = wstring());
  void load_file(const string& file, const wstring& frame_name = wstring() );
  void go_to_history_offset(int offset);
  void execute_javascript(const std::string& javascript, const wstring& frame_name = wstring() );
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
  void deselect_all();
  void resize(int width, int height);
  unsigned int get_width() const;
  unsigned int get_height() const;
  
  //void get_content_as_text(int max_chars);
  void zoom_in();
  void zoom_out();
  void reset_zoom();
  void unfocus();
  void focus();
  void set_transparent(bool is_transparent);

  
  void set_URL_filter_mode(URLFilteringMode mode);
  void add_URL_filter(const wstring& filter);
  void clear_all_URL_filters();
  void set_header_definition(const string& name, const std::map<std::string, std::string>& definition);
  void add_header_rewrite_rule(const wstring& rule, const string& name);
  void remove_header_rewrite_rule(const wstring& rule);
  void remove_header_rewrite_rules_by_definition_name(const string& name);
  void set_opens_external_links_in_calling_frame(bool is_enabled);
  
protected:
  Awesomium::WebView* _web_view;
  unsigned int _width;
  unsigned int _height;


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

#endif
