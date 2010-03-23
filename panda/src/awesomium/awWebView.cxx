// Filename: awWebView.cxx
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
#include "awWebView.h"

TypeHandle AwWebView::_type_handle;


AwWebView::AwWebView(Awesomium::WebView * web_view, unsigned int width, unsigned int height):
_web_view(web_view)
{
	//this should be never be null
	nassertv(web_view != NULL);
	//then call resize to cache the value
	resize(width, height);
}

AwWebView::~AwWebView(){
  //since this is a reference counted object we can just call destroy here.
  //technically Awesomium::Webcore should take care of the rest
  destroy();
}

bool AwWebView::is_dirty(){
  return _web_view->isDirty();
}

void AwWebView::render(void* destination, int destRowSpan, int destDepth){
  _web_view->render( (unsigned char*)destination, destRowSpan, destDepth );
}

////////////
void AwWebView::destroy(void){
  _web_view->destroy();
}

void AwWebView::load_URL(const string& url, const string& frame_name, const string& username, const string& password){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadURL(url, wide_frame_name, username, password);
}

void AwWebView::load_HTML(const string& html, const string& frame_name){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadHTML(html, wide_frame_name);
}

void AwWebView::load_file(const string& file, const string& frame_name){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadFile(file, wide_frame_name);
}

void AwWebView::go_to_history_offset(int offset) {
  _web_view->goToHistoryOffset(offset);
}

void AwWebView::execute_javascript(const std::string& javascript, const std::string& frame_name ) {
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->executeJavascript(javascript, wide_frame_name);
}


/////////

void AwWebView::resize(int width, int height){
	_width = width;
	_height = height;
	_web_view->resize(width,height); 
}


unsigned int AwWebView::get_width() const {
	return _width;
}
unsigned int AwWebView::get_height() const {
	return _height;
}

void AwWebView::inject_mouse_move(int x, int y){  _web_view->injectMouseMove(x,y); }
void AwWebView::inject_mouse_down(AwWebView::MouseButton button){ _web_view->injectMouseDown((Awesomium::MouseButton)button);}
void AwWebView::inject_mouse_up(AwWebView::MouseButton button){	_web_view->injectMouseUp((Awesomium::MouseButton)button); }
void AwWebView::inject_mouse_wheel(int scroll_amount){_web_view->injectMouseWheel(scroll_amount); }
void AwWebView::cut(){ _web_view->cut(); }
void AwWebView::copy(){ _web_view->copy(); }
void AwWebView::paste(){ _web_view->paste(); }
void AwWebView::select_all(){ _web_view->selectAll(); }
void AwWebView::deselect_all(){ _web_view->deselectAll(); }
void AwWebView::zoom_in(){ _web_view->zoomIn(); }
void AwWebView::zoom_out(){ _web_view->zoomOut(); }
void AwWebView::reset_zoom(){ _web_view->resetZoom(); } 
void AwWebView::unfocus(){ _web_view->unfocus(); } 
void AwWebView::focus(){ _web_view->focus(); }
void AwWebView::set_transparent(bool is_transparent){ _web_view->setTransparent(is_transparent); }
