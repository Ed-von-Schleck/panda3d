// Filename: awWebView.I
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

INLINE bool AwWebView::isDirty(){
  return _web_view->isDirty();
}

INLINE void AwWebView::render(void* destination, int destRowSpan, int destDepth){
  _web_view->render( destination, destRowSpan, destDepth );
}

////////////
INLINE void AwWebView::destroy(void){
  _web_view->destroy();
}

INLINE void AwWebView::load_URL(const string& url, const string& frame_name, const string& username, const string& password){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadURL(url, wide_frame_name, username, password);
}

INLINE void AwWebView::load_HTML(const string& html, const string& frame_name){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadHTML(html, wide_frame_name);
}

INLINE void AwWebView::load_file(const string& file, const string& frame_name){
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->loadFile(file, wide_frame_name);
}

INLINE void AwWebView::go_to_history_offset(int offset) {
  _web_view->goToHistoryOffset(offset);
}

INLINE void AwWebView::execute_javascript(const std::string& javascript, const std::string& frameName ) {
	wstring wide_frame_name(frame_name.begin(), frame_name.end());
	_web_view->executeJavascript(javascript, wide_frame_name);
}


/////////
INLINE void AwWebView::inject_mouse_move(int x, int y){  _web_view->injectMouseMove(x,y); }
INLINE void AwWebView::inject_mouse_down(AwWebView::MouseButton button){ _web_view->injectMouseDown((Awesomium::MouseButton)button);}
INLINE void AwWebView::inject_mouse_up(AwWebView::MouseButton button){	_web_view->injectMouseUp(Awesomium::MouseButton)button); }
INLINE void AwWebView::inject_mouse_wheel(int scroll_amount){_web_view->injectMouseWheel(scroll_amount); }
INLINE void AwWebView::cut(){_web_view->cut(); }
INLINE void AwWebView::copy(){_web_view->copy(); }
INLINE void AwWebView::paste(){_web_view->paste(); }
INLINE void AwWebView::select_all(){_web_view->selectAll(); }
INLINE void AwWebView::deselect_all(){_web_view->deselectAll(); }


INLINE void AwWebView::setTransparent(bool is_transparent){ _web_view->deselectAll(is_transparent); }
