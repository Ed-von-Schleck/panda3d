// Filename: colladaInstance.h
// Created by:  rdb (14Apr10)
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

#ifndef COLLADAINSTANCE_H
#define COLLADAINSTANCE_H

#include "pandabase.h"

#include "config_collada.h"
#include "pointerTo.h"
#include "colladaElement.h"

class ColladaVisualScene;

////////////////////////////////////////////////////////////////////
//       Class : ColladaInstance
// Description : This is the template class for libraries in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
template <class T>
class EXPCL_COLLADA ColladaInstance : public ColladaElement {
PUBLISHED:
  INLINE virtual void clear();
  INLINE bool is_empty() const;
  INLINE const string &get_url() const;
  INLINE void set_url(const string &url);

  INLINE bool load_xml(const TiXmlElement *xelement);
  INLINE TiXmlElement * make_xml() const;

private:
  string _url;

};

typedef ColladaInstance<ColladaVisualScene> ColladaInstanceVisualScene;

#include "colladaInstance.I"

#endif

