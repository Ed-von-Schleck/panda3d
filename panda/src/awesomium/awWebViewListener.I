// Filename: awWebViewListener.I
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

INLINE void AwWebViewListener::onBeginNavigation(const std::string& url, const std::wstring& frameName){}
INLINE void AwWebViewListener::onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType) {}
INLINE void AwWebViewListener::onFinishLoading() {}
//void AwWebViewListener::onCallback(const std::string& name, const Awesomium::JSArguments& args) {}
INLINE void AwWebViewListener::onReceiveTitle(const std::wstring& title, const std::wstring& frameName) {}
INLINE void AwWebViewListener::onChangeTooltip(const std::wstring& tooltip) {}
INLINE void AwWebViewListener::onChangeKeyboardFocus(bool isFocused) {}
INLINE void AwWebViewListener::onChangeTargetURL(const std::string& url) {}
