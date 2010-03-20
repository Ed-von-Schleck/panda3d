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

AwWebView::AwWebView():
_web_view(NULL)
{
  //empty
}
AwWebView::AwWebView(Awesomium::WebView * web_view):
_web_view(web_view)
{
  //empty
}

AwWebView::~AwWebView(){
  //since this is a reference counted object we can just call destroy here.
  //technically Awesomium::Webcore should take care of the rest
  destroy();
}
