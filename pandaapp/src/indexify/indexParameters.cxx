// Filename: indexParameters.cxx
// Created by:  drose (04Apr02)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://www.panda3d.org/license.txt .
//
// To contact the maintainers of this program write to
// panda3d@yahoogroups.com .
//
////////////////////////////////////////////////////////////////////

#include "indexParameters.h"

// User parameters
int max_index_width = 700;
int max_index_height = 700;

int thumb_width = 100;
int thumb_height = 100;
int thumb_caption_height = 16;
int thumb_x_space = 16;
int thumb_y_space = 16;

int caption_font_size = 14;

int reduced_width = 800;
int reduced_height = 700;

Filename prev_icon;
Filename next_icon;
Filename up_icon;

bool force_regenerate = false;
bool format_rose = false;

// Computed parameters
int thumb_count_x;
int thumb_count_y;
int max_thumbs;
int actual_index_width;

////////////////////////////////////////////////////////////////////
//     Function: finalize_parameters
//  Description: This is called after all user parameters have been
//               changed, to do whatever computations are required for
//               the other parameters that are based on the user
//               parameters.
////////////////////////////////////////////////////////////////////
void
finalize_parameters() {
  thumb_count_x = 
    (max_index_width - thumb_x_space) / (thumb_width + thumb_x_space);
  thumb_count_y = 
    (max_index_height - thumb_y_space) / (thumb_height + thumb_caption_height + thumb_y_space);
  
  max_thumbs = thumb_count_x * thumb_count_y;
  
  actual_index_width = thumb_x_space + thumb_count_x * (thumb_width + thumb_x_space);
}
