// Filename: awWebViewListener.h
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
#ifndef AWWEBVIEWLISTENER_H
#define AWWEBVIEWLISTENER_H

#include "pandabase.h"
#include "typedReferenceCount.h"
#include "luse.h"

#include "awesomium_includes.h"

////////////////////////////////////////////////////////////////////
//       Class : AwWebViewListener
// Description : Thin bindings, wraps a WebViewListener
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAAWESOMIUM AwWebViewListener : public TypedReferenceCount, public Awesomium::WebCore {
PUBLISHED:
	AwWebViewListener();

	virtual ~AwWebViewListener() {}
	INLINE void onBeginNavigation(const std::string& url, const std::wstring& frameName) ;
	INLINE void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType);
	INLINE void onFinishLoading();
	//void onCallback(const std::string& name, const Awesomium::JSArguments& args);
	INLINE void onReceiveTitle(const std::wstring& title, const std::wstring& frameName) ;
	INLINE void onChangeTooltip(const std::wstring& tooltip);
	INLINE void onChangeKeyboardFocus(bool isFocused) ;
	INLINE void onChangeTargetURL(const std::string& url) ;

public:
	static TypeHandle get_class_type() {
		return _type_handle;
	}
	static void init_type() {
		TypedReferenceCount::init_type();
		register_type(_type_handle, "AwWebViewListener",
			TypedReferenceCount::get_class_type());
	}
	virtual TypeHandle get_type() const {
		return get_class_type();
	}
	virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
	static TypeHandle _type_handle;
};

 #include "awWebViewListener.I"

#endif
