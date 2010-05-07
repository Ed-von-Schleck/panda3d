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
class ColladaMaterial;
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
    { return TypeHandle::none(); };

protected:
  string _url;
  friend class ColladaDocument;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    ColladaElement::init_type();
    register_type(_type_handle, "ColladaInstanceBase",
                  ColladaElement::get_class_type());
  }

private:
  static TypeHandle _type_handle;
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

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type(const string &type_name = "UndefinedColladaInstance") {
    ColladaInstanceBase::init_type();
    _type_handle = register_dynamic_type
      (type_name, ColladaInstanceBase::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    // In this case, we can't do anything, since we don't have the
    // class' type_name.
    return get_class_type();
  }

private:
  static TypeHandle _type_handle;
};

// Note: when adding new types, be sure to add them to config_collada.cxx also

EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaCamera>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaEffect>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaGeometry>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaLight>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaMaterial>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaNode>);
EXPORT_TEMPLATE_CLASS(EXPCL_COLLADA, EXPTP_COLLADA, ColladaInstance<ColladaVisualScene>);

typedef ColladaInstance<ColladaCamera> ColladaInstanceCamera;
typedef ColladaInstance<ColladaEffect> ColladaInstanceEffect;
typedef ColladaInstance<ColladaGeometry> ColladaInstanceGeometry;
typedef ColladaInstance<ColladaLight> ColladaInstanceLight;
typedef ColladaInstance<ColladaMaterial> ColladaInstanceMaterial;
typedef ColladaInstance<ColladaNode> ColladaInstanceNode;
typedef ColladaInstance<ColladaVisualScene> ColladaInstanceVisualScene;

#include "colladaInstance.I"

// Tell GCC that we'll take care of the instantiation explicitly here.
#ifdef __GNUC__
#pragma interface
#endif

#endif

