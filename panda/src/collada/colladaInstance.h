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

class ColladaCamera;
class ColladaDocument;
class ColladaEffect;
class ColladaGeometry;
class ColladaLight;
class ColladaNode;
class ColladaVisualScene;

////////////////////////////////////////////////////////////////////
//       Class : ColladaInstanceBase
// Description : This is the base class for instances in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
class EXPCL_COLLADA ColladaInstanceBase : public ColladaElement {
PUBLISHED:
  INLINE virtual TypeHandle get_target_type() const
    { return _target_type; };

protected:
  string _url;
  TypeHandle _target_type;

  friend class ColladaDocument;
};

////////////////////////////////////////////////////////////////////
//       Class : ColladaInstance
// Description : This is the template class for instances in the
//               COLLADA document.
////////////////////////////////////////////////////////////////////
template <class T>
class EXPCL_COLLADA ColladaInstance : public ColladaInstanceBase {
PUBLISHED:
  INLINE virtual TypeHandle get_target_type() const;
  INLINE virtual void clear();
  INLINE bool is_empty() const;
  INLINE const string &get_url() const;
  INLINE void set_url(const string &url);

  INLINE PT(ColladaElement) get_target() const;

  INLINE virtual bool load_xml(const TiXmlElement *xelement);
  INLINE virtual TiXmlElement *make_xml() const;

private:
  friend class ColladaDocument;
};

typedef ColladaInstance<ColladaCamera> ColladaInstanceCamera;
typedef ColladaInstance<ColladaEffect> ColladaInstanceEffect;
typedef ColladaInstance<ColladaGeometry> ColladaInstanceGeometry;
typedef ColladaInstance<ColladaLight> ColladaInstanceLight;
typedef ColladaInstance<ColladaNode> ColladaInstanceNode;
typedef ColladaInstance<ColladaVisualScene> ColladaInstanceVisualScene;

#include "colladaInstance.I"

#endif

