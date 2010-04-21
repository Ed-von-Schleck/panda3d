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
  clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::load_xml
//       Access: Public
//  Description: Loads <node> data from a TiXmlElement.
////////////////////////////////////////////////////////////////////
bool ColladaNode::
load_xml(const TiXmlElement *xelement, const CoordinateSystem cs) {
  if (!ColladaAssetElement::load_xml(xelement)) {
    return false;
  }
  nassertr(xelement->ValueStr() == "node", false);

  CoordinateSystem newcs (cs);
  bool okflag = true;

  if (_asset != NULL) {
    if (_asset->get_coordinate_system() != CS_default) {
      newcs = _asset->get_coordinate_system();
    }
  }

  const TiXmlElement* xchild;

  // Read out the node type
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

  // Read out the child nodes
  xchild = xelement->FirstChildElement("node");
  while (xchild != NULL) {
    PT(ColladaNode) node = new ColladaNode;
    node->_parent = this;
    node->load_xml(xchild, newcs);
    _nodes.push_back(node);
    xchild = xchild->NextSiblingElement("node");
  }

  // Read out any instances to nodes
  xchild = xelement->FirstChildElement("instance_node");
  while (xchild != NULL) {
    PT(ColladaInstanceNode) inst = new ColladaInstanceNode;
    inst->_parent = this;
    //TODO: figure out what to do with newcs here
    inst->load_xml(xchild);
    _instance_nodes.push_back(inst);
    xchild = xchild->NextSiblingElement("instance_node");
  }

  //TODO: handle layers (binning?)

  // Iterate over the elements to apply the transforms one by one
  //TODO: coordinate system conversion
  xchild = xelement->FirstChildElement();
  while (xchild != NULL) {
    const string ttype (xchild->Value());
    if (ttype == "lookat") {
      collada_cat.error() << "<lookat> elements are not supported yet!\n";
      okflag = false;
    } else if (ttype == "matrix") {
      vector_string m;
      tokenize(trim(xchild->GetText()), m, " ", true);
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
      tokenize(trim(xchild->GetText()), r, " ", true);
      if (r.size() == 4) {
        _transform *= LMatrix4d::rotate_mat(patof(r[3].c_str()), LVecBase3d(
          patof(r[0].c_str()), patof(r[1].c_str()), patof(r[2].c_str())));
      } else {
        collada_cat.error() << "<rotate> element should have 4 values!\n";
        okflag = false;
      }
    } else if (ttype == "scale") {
      vector_string v;
      tokenize(trim(xchild->GetText()), v, " ", true);
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
      tokenize(trim(xchild->GetText()), v, " ", true);
      if (v.size() == 3) {
        _transform *= LMatrix4d::translate_mat(
          patof(v[0].c_str()), patof(v[1].c_str()), patof(v[2].c_str()));
      } else {
        collada_cat.error() << "<translate> element should have 3 values!\n";
        okflag = false;
      }
    }
    xchild = xchild->NextSiblingElement();
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
  TiXmlElement * xelement = ColladaAssetElement::make_xml();
  xelement->SetValue("node");

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

  for (int i = 0; i < _instance_nodes.size(); ++i) {
    PT(ColladaNode) target = _instance_nodes.at(i)->_target;
    nassertd(target != NULL) continue;
    // Make sure that we re-use the same node, for the sake of instancing
    PT(PandaNode) targetnode = target->_cached_node;
    if (targetnode == NULL) {
      targetnode = target->make_node();
    }
    pnode->add_child(targetnode);
  }

  ((ColladaNode*)this)->_cached_node = pnode;
  return pnode;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaNode::get_element_by_id
//       Access: Public
//  Description: Returns the element in the node hierarchy that has
//               the given ID, or NULL if none found.
//               Searches recursively for subnodes as well.
////////////////////////////////////////////////////////////////////
INLINE PT(ColladaNode) ColladaNode::
get_element_by_id(const string &id) const {
  PT(ColladaNode) res;
  for (int i = 0; i < _nodes.size(); ++i) {
    res = _nodes.at(i);
    if (res->get_id() == id) {
      return res;
    } else {
      res = res->get_element_by_id(id);
      if (res != NULL) {
        return res;
      }
    }
  }
  return NULL;
}
