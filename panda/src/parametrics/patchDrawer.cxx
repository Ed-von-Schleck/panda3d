// Filename: patchDrawer.C
// Created by:  drose (18Apr97)
// 
////////////////////////////////////////////////////////////////////
// Copyright (C) 1992,93,94,95,96,97  Walt Disney Imagineering, Inc.
// 
// These  coded  instructions,  statements,  data   structures   and
// computer  programs contain unpublished proprietary information of
// Walt Disney Imagineering and are protected by  Federal  copyright
// law.  They may  not be  disclosed to third  parties  or copied or
// duplicated in any form, in whole or in part,  without  the  prior
// written consent of Walt Disney Imagineering Inc.
////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////

#include "parametrics.h"
#include "patchDrawer.h"
#include <math.h>
#include <perfalloc.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pr/pfMaterial.h>


////////////////////////////////////////////////////////////////////
// Static variables 
////////////////////////////////////////////////////////////////////

// The same geostate will be shared by all surfaces, so we'll just create
// one statically and then never again.
pfGeoState *ParametricSurfaceDrawer::geostate = NULL;



////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::Constructor
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurfaceDrawer *
make_ParametricSurfaceDrawer(ParametricSurface *surface) {
  return new ParametricSurfaceDrawer(surface);
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::Constructor
//  Description:
////////////////////////////////////////////////////////////////////
void
rm_ParametricSurfaceDrawer(ParametricSurfaceDrawer *psd) {
  delete psd;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::Constructor
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurfaceDrawer::
ParametricSurfaceDrawer(ParametricSurface *surface) {
  if (geostate==NULL) {
    geostate = new pfGeoState;
    pfRef(geostate);

    // For some reason, I have to set these modes locally in order for
    // the surface to look right in perfly.  What could be going on
    // here?
    geostate->setMode(PFSTATE_ENLIGHTING, PF_ON);
    geostate->setMode(PFSTATE_ANTIALIAS, PF_OFF);
    pfMaterial *mat = new pfMaterial;
    mat->setSide(PFMTL_FRONT);
    geostate->setAttr(PFSTATE_FRONTMTL, mat);
  }

  _surface = surface;
  _color.set(1.0, 1.0, 1.0, 1.0);
  _num_s = 10;
  _num_t = 10;
  _geode = new pfGeode;
  pfRef(_geode);
  _drawn = false;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::Destructor
//       Access: Public, Scheme, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurfaceDrawer::
~ParametricSurfaceDrawer() {
  hide();
  pfUnrefDelete(_geode);
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::get_surface
//       Access: Public, Scheme
//  Description:
////////////////////////////////////////////////////////////////////
ParametricSurface *ParametricSurfaceDrawer::
get_surface() {
  return _surface;
}


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::get_geode
//       Access: Public, Scheme
//  Description: Returns a pointer to the drawer's pfGeode.  This is
//               where the drawer will build the visible
//               representation of the surface.  This geode must be
//               inserted into the scene graph to make the surface
//               visible.  The geode remains connected to the drawer,
//               so that future updates to the drawer will reflect in
//               the geode, and the geode will be emptied when the
//               drawer destructs.  Also see detach_geode().
////////////////////////////////////////////////////////////////////
pfGeode *ParametricSurfaceDrawer::
get_geode() {
  return _geode;
}



////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::detach_geode
//       Access: Public, Scheme
//  Description: Detaches the pfGeode from the drawer so that the
//               drawing will remain after the death of the drawer.
//               Returns the now-static geode.  A new, dynamic geode
//               is created for the drawer's future use; get_geode()
//               will return this new geode which will be empty until
//               the next call to draw().
////////////////////////////////////////////////////////////////////
pfGeode *ParametricSurfaceDrawer::
detach_geode() {
  if (!_drawn) {
    draw();
  }
  pfGeode *g = _geode;
  pfUnref(g);
  _geode = new pfGeode;
  pfRef(_geode);
  _drawn = false;
  return g;
}
  


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::set_num_s
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
void ParametricSurfaceDrawer::
set_num_s(int num_s) {
  _num_s = num_s;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::get_num_s
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
int ParametricSurfaceDrawer::
get_num_s() const {
  return _num_s;
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::set_num_t
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
void ParametricSurfaceDrawer::
set_num_t(int num_t) {
  _num_t = num_t;
  if (_drawn) {
    draw();
  }
}

////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::get_num_t
//       Access: Public, Scheme
//  Description: 
////////////////////////////////////////////////////////////////////
int ParametricSurfaceDrawer::
get_num_t() const {
  return _num_t;
}





////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::set_color
//       Access: Public, Scheme
//  Description: Specifies the color of the surface when it is drawn.
//               The default is white.
////////////////////////////////////////////////////////////////////
void ParametricSurfaceDrawer::
set_color(float r, float g, float b) {
  _color.set(r, g, b, 0.0);
}




////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::draw
//       Access: Public, Scheme, Virtual
//  Description: Creates a series of line segments that approximates
//               the surface.  These line segments may be made visible
//               by adding the geode returned by get_geode() into the
//               scene graph.
////////////////////////////////////////////////////////////////////
boolean ParametricSurfaceDrawer::
draw() {
  // First, remove the old drawing, if any.
  hide();

  // If there's no surface, draw nothing and return false.
  if (_surface==NULL || !_surface->is_valid()) {
    return false;
  }

  // Otherwise, let's go to town!

  // _num_s and _num_t are the number of patches.  Convert that to the
  // number of vertices.

  int num_s = _num_s + 1;
  int num_t = _num_t + 1;

  // Now how many vertices is that total, and how many vertices per
  // T-strip?
  int num_verts = num_s * num_t;
  int num_tstrips = (num_t-1);
  int tstrip_length = 2*(num_s-1)+2;

  // Create and allocate a bunch of arrays to put in the geoset.
  pfVec3 *verts;
  pfVec3 *norms;
  pfVec4 *colors;
  int *lengths;
  ushort *cindex;
  ushort *vindex;

  verts = (pfVec3 *)perf_allocate(num_verts * sizeof(pfVec3), verts);
  norms = (pfVec3 *)perf_allocate(num_verts * sizeof(pfVec3), norms);
  colors = (pfVec4 *)perf_allocate(1 * sizeof(pfVec4), colors);
  lengths = (int *)perf_allocate(num_tstrips * sizeof(int), lengths);
  cindex = (ushort *)perf_allocate(1 * sizeof(ushort), cindex);
  vindex = (ushort *)perf_allocate(num_tstrips * tstrip_length * sizeof(ushort), vindex);

  // Compute all the vertices.
  int si, ti, n;
  double s, t;
  for (ti = 0; ti < num_t; ti++) {
    t = _surface->get_max_t() * (double)ti / (double)(num_t-1);
    for (si = 0; si < num_s; si++) {
      s = _surface->get_max_s() * (double)si / (double)(num_s-1);
      n = ti * num_s + si;
      dnassert(n>=0 && n<num_verts);

      _surface->get_pn(s, t, verts[n], norms[n]);
    }
  }

  // Set the lengths array.  We are creating num_tstrips T-strips,
  // each of which has t_strip length vertices.
  for (n = 0; n < num_tstrips; n++) {
    lengths[n] = tstrip_length;
  }

  // Now fill up the index array into the vertices.  This lays out the
  // order of the vertices in each T-strip.
  n = 0;
  for (ti = 1; ti < num_t; ti++) {
    vindex[n++] = ti*num_s;
    for (si = 1; si < num_s; si++) {
      vindex[n++] = (ti-1)*num_s + (si-1);
      vindex[n++] = ti*num_s + si;
    }
    vindex[n++] = (ti-1)*num_s + (num_s-1);
  }
  dnassert(n==num_tstrips * tstrip_length);

  // And our one-element color array, with its one-element index.
  colors[0] = _color;
  cindex[0] = 0;

  // Now create the geoset.
  pfGeoSet *geoset = new pfGeoSet;
  geoset->setPrimType(PFGS_TRISTRIPS);
  geoset->setNumPrims(num_tstrips);
  geoset->setPrimLengths(lengths);
  geoset->setAttr(PFGS_COLOR4, PFGS_OVERALL, colors, cindex);
  geoset->setAttr(PFGS_NORMAL3, PFGS_PER_VERTEX, norms, vindex);
  geoset->setAttr(PFGS_TEXCOORD2, PFGS_OFF, NULL, NULL);
  geoset->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, verts, vindex);
  geoset->setGState(geostate);
  geoset->setLineWidth(1);

  _geode->addGSet(geoset);

  return true;
}

    


////////////////////////////////////////////////////////////////////
//     Function: ParametricSurfaceDrawer::hide
//       Access: Public, Scheme
//  Description: Removes the polys that were created by a previous
//               call to draw().
////////////////////////////////////////////////////////////////////
void ParametricSurfaceDrawer::
hide() {
  while (_geode->getNumGSets()>0) {
    _geode->removeGSet(_geode->getGSet(0));
  }
  _drawn = false;
}

