// Filename: colladaNode.cxx
// Created by: rdb (13Apr10)
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

#include "colladaNode.h"

#include "pstrtod.h"

TypeHandle ColladaNode::_type_handle;
const string ColladaNode::_element_name ("node");
const string ColladaNode::_library_name ("library_nodes");

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaNode::
ColladaNode() {
  _asset = NULL;
  _node_type = NT_default;
  _transform = LMatrix4d::ident_mat();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::load_xml
//       Access: Public
//  Description: Loads <node> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaNode::
load_xml(const TiXmlElement *xelement) {
  nassertr(xelement != NULL, false);
  nassertr(xelement->ValueStr() == "node", false);

  _nodes.clear();
  clear_name();
  _node_type = NT_default;
  _transform = LMatrix4d::ident_mat();
  bool okflag = true;

  const char* name = xelement->Attribute("name");
  if (name) {
    set_name(name);
  }

  const TiXmlElement* xnode = xelement->FirstChildElement("node");
  while (xnode != NULL) {
    PT(ColladaNode) node = new ColladaNode;
    node->load_xml(xnode);
    _nodes.push_back(node);
    xnode = xnode->NextSiblingElement("node");
  }

  const TiXmlElement* xasset = xelement->FirstChildElement("asset");
  if (xasset != NULL) {
    _asset = new ColladaAsset();
    okflag &= _asset->load_xml(xasset);
  }

  const char* type = xelement->Attribute("type");
  if (type) {
    if (cmp_nocase(type, "NODE") == 0) {
      _node_type = NT_node;
    } else if (cmp_nocase(type, "JOINT") == 0) {
      _node_type = NT_joint;
    } else {
      collada_cat.error()
        << "Node type should be either 'NODE' or 'JOINT', not '" << type << "'!\n";
      okflag = false;
    }
  }

  // Iterate over the elements to apply the transforms one by one
  //TODO: coordinate system conversion
  const TiXmlElement* xtransform = xelement->FirstChildElement();
  while (xtransform != NULL) {
    const string ttype (xtransform->Value());
    if (ttype == "lookat") {
      collada_cat.error() << "<lookat> elements are not supported yet!\n";
      okflag = false;
    } else if (ttype == "matrix") {
      vector_string m;
      tokenize(trim(xtransform->GetText()), m, " ", true);
      if (m.size() == 16) {
        _transform *= LMatrix4d(
          patof(m[0].c_str()), patof(m[4].c_str()), patof(m[8].c_str()), patof(m[12].c_str()),
          patof(m[1].c_str()), patof(m[5].c_str()), patof(m[9].c_str()), patof(m[13].c_str()),
          patof(m[2].c_str()), patof(m[6].c_str()), patof(m[10].c_str()), patof(m[14].c_str()),
          patof(m[3].c_str()), patof(m[7].c_str()), patof(m[11].c_str()), patof(m[15].c_str()));
      } else {
        collada_cat.error() << "<matrix> element should have 16 values!\n";
        okflag = false;
      }
    } else if (ttype == "rotate") {
      vector_string r;
      tokenize(trim(xtransform->GetText()), r, " ", true);
      if (r.size() == 4) {
        _transform *= LMatrix4d::rotate_mat(patof(r[3].c_str()), LVecBase3d(
          patof(r[0].c_str()), patof(r[1].c_str()), patof(r[2].c_str())));
      } else {
        collada_cat.error() << "<rotate> element should have 4 values!\n";
        okflag = false;
      }
    } else if (ttype == "scale") {
      vector_string v;
      tokenize(trim(xtransform->GetText()), v, " ", true);
      if (v.size() == 3) {
        _transform *= LMatrix4d::scale_mat(
          patof(v[0].c_str()), patof(v[1].c_str()), patof(v[2].c_str()));
      } else {
        collada_cat.error() << "<scale> element should have 3 values!\n";
        okflag = false;
      }
    } else if (ttype == "skew") {
      collada_cat.error() << "<skew> elements are not supported yet!\n";
      okflag = false;
    } else if (ttype == "translate") {
      vector_string v;
      tokenize(trim(xtransform->GetText()), v, " ", true);
      if (v.size() == 3) {
        _transform *= LMatrix4d::translate_mat(
          patof(v[0].c_str()), patof(v[1].c_str()), patof(v[2].c_str()));
      } else {
        collada_cat.error() << "<translate> element should have 3 values!\n";
        okflag = false;
      }
    }
    xtransform = xtransform->NextSiblingElement();
  }

  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::make_xml
//       Access: Public
//  Description: Returns a new TiXmlElement representing
//               the visual scene.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaNode::
make_xml() const {
  TiXmlElement * xelement = new TiXmlElement("node");

  if (has_name()) {
    xelement->SetAttribute("name", get_name());
  }

  switch (_node_type) {
    case NT_node:
      xelement->SetAttribute("type", "NODE");
      break;
    case NT_joint:
      xelement->SetAttribute("type", "JOINT");
      break;
    default:
      break;
  }

  if (_asset) {
    xelement->LinkEndChild(_asset->make_xml());
  }

  //TODO: coordinate system conversion
  if (_transform != LMatrix4d::ident_mat()) {
    TiXmlElement *matrix = new TiXmlElement("matrix");
    ostringstream matstr;
    matstr << MAYBE_ZERO(_transform._m.m._00) << " "
           << MAYBE_ZERO(_transform._m.m._10) << " "
           << MAYBE_ZERO(_transform._m.m._20) << " "
           << MAYBE_ZERO(_transform._m.m._30) << " "
           << MAYBE_ZERO(_transform._m.m._01) << " "
           << MAYBE_ZERO(_transform._m.m._11) << " "
           << MAYBE_ZERO(_transform._m.m._21) << " "
           << MAYBE_ZERO(_transform._m.m._31) << " "
           << MAYBE_ZERO(_transform._m.m._02) << " "
           << MAYBE_ZERO(_transform._m.m._12) << " "
           << MAYBE_ZERO(_transform._m.m._22) << " "
           << MAYBE_ZERO(_transform._m.m._32) << " "
           << MAYBE_ZERO(_transform._m.m._03) << " "
           << MAYBE_ZERO(_transform._m.m._13) << " "
           << MAYBE_ZERO(_transform._m.m._23) << " "
           << MAYBE_ZERO(_transform._m.m._33);
    TiXmlText *text = new TiXmlText(matstr.str());
    matrix->LinkEndChild(text);
    xelement->LinkEndChild(matrix);
  }

  for (int i = 0; i < _nodes.size(); ++i) {
    xelement->LinkEndChild(_nodes.at(i)->make_xml());
  }

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::make_node
//       Access: Public
//  Description: Returns a new PandaNode representing this
//               COLLADA node and the children below it.
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaNode::
make_node() const {
  PT(PandaNode) pnode = new PandaNode(get_name());

  //TODO: coordinate system conversion
  pnode->set_transform(TransformState::make_mat(LCAST(float, _transform)));

  for (int i = 0; i < _nodes.size(); ++i) {
    pnode->add_child(_nodes.at(i)->make_node());
  }

  return pnode;
}
