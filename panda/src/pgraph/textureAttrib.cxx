// Filename: textureAttrib.cxx
// Created by:  drose (21Feb02)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#include "textureAttrib.h"
#include "graphicsStateGuardianBase.h"
#include "bamReader.h"
#include "bamWriter.h"
#include "datagram.h"
#include "datagramIterator.h"

TypeHandle TextureAttrib::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object suitable for
//               rendering the indicated texture onto geometry.
//
//               This method is deprecated, and is provided for
//               backward compatibility; you should use the
//               multitexture form of this instead.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make(Texture *texture) {
  TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
  return make(O_set, tex_mgr->get_default_stage(), texture);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_off
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object suitable for
//               rendering untextured geometry.
//
//               This method is deprecated, and is provided for
//               backward compatibility; you should use the
//               multitexture form of this instead.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_off() {
  return make_all_off();
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_all_off
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns off
//               all stages (and hence disables stageing).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_all_off() {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_operation = O_set;
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns on (or
//               off, according to op) the indicate stage(s).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make(TextureAttrib::Operation op, TextureStage *stage, Texture *tex) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_operation = op;
  attrib->_stages.push_back(stage);
  attrib->_textures[stage] = tex;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns on (or
//               off, according to op) the indicate stage(s).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make(TextureAttrib::Operation op, 
     TextureStage *stageA, Texture *texA,
     TextureStage *stageB, Texture *texB) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_operation = op;
  attrib->_stages.push_back(stageA);
  attrib->_textures[stageA] = texA;
  attrib->_stages.push_back(stageB);
  attrib->_textures[stageB] = texB;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns on (or
//               off, according to op) the indicate stage(s).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make(TextureAttrib::Operation op,
     TextureStage *stageA, Texture *texA,
     TextureStage *stageB, Texture *texB,
     TextureStage *stageC, Texture *texC) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_operation = op;
  attrib->_stages.push_back(stageA);
  attrib->_textures[stageA] = texA;
  attrib->_stages.push_back(stageB);
  attrib->_textures[stageB] = texB;
  attrib->_stages.push_back(stageC);
  attrib->_textures[stageC] = texC;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns on (or
//               off, according to op) the indicate stage(s).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make(TextureAttrib::Operation op,
     TextureStage *stageA, Texture *texA,
     TextureStage *stageB, Texture *texB,
     TextureStage *stageC, Texture *texC,
     TextureStage *stageD, Texture *texD) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_operation = op;
  attrib->_stages.push_back(stageA);
  attrib->_textures[stageA] = texA;
  attrib->_stages.push_back(stageB);
  attrib->_textures[stageB] = texB;
  attrib->_stages.push_back(stageC);
  attrib->_textures[stageC] = texC;
  attrib->_stages.push_back(stageD);
  attrib->_textures[stageD] = texD;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::issue
//       Access: Public, Virtual
//  Description: Calls the appropriate method on the indicated GSG
//               to issue the graphics commands appropriate to the
//               given attribute.  This is normally called
//               (indirectly) only from
//               GraphicsStateGuardian::set_state() or modify_state().
////////////////////////////////////////////////////////////////////
void TextureAttrib::
issue(GraphicsStateGuardianBase *gsg) const {
  gsg->issue_texture(this);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::output
//       Access: Public, Virtual
//  Description: 
////////////////////////////////////////////////////////////////////
void TextureAttrib::
output(ostream &out) const {
  out << get_type() << ":";
  if (_operation == O_set && _stages.empty()) {
    out << "all off";
  } else {
    switch (_operation) {
    case O_set:
      out << "set";
      break;
    case O_add:
      out << "add";
      break;
    case O_remove:
      out << "remove";
      break;
    }

    Stages::const_iterator li;
    for (li = _stages.begin(); li != _stages.end(); ++li) {
      TextureStage *stage = (*li);
      Textures::const_iterator ti = _textures.find(stage);
      if (ti != _textures.end()) {
        Texture *tex = (*ti).second;
        out << " " << stage->get_name() << ":" << tex->get_name();
      } else {
        out << " " << stage->get_name();
      }
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::compare_to_impl
//       Access: Protected, Virtual
//  Description: Intended to be overridden by derived TextureAttrib
//               types to return a unique number indicating whether
//               this TextureAttrib is equivalent to the other one.
//
//               This should return 0 if the two TextureAttrib objects
//               are equivalent, a number less than zero if this one
//               should be sorted before the other one, and a number
//               greater than zero otherwise.
//
//               This will only be called with two TextureAttrib
//               objects whose get_type() functions return the same.
////////////////////////////////////////////////////////////////////
int TextureAttrib::
compare_to_impl(const RenderAttrib *other) const {
  const TextureAttrib *ta;
  DCAST_INTO_R(ta, other, 0);

  if (_operation != ta->_operation) {
    return (int)_operation - (int)ta->_operation;
  }

  Textures::const_iterator li = _textures.begin();
  Textures::const_iterator oli = ta->_textures.begin();

  while (li != _textures.end() && oli != ta->_textures.end()) {
    TextureStage *stage = (*li).first;
    TextureStage *other_stage = (*oli).first;

    if (stage != other_stage) {
      return stage < other_stage ? -1 : 1;
    }

    Texture *tex = (*li).second;
    Texture *other_tex = (*oli).second;
    if (tex != other_tex) {
      return tex < other_tex ? -1 : 1;
    }

    ++li;
    ++oli;
  }

  if (li != _textures.end()) {
    return 1;
  }
  if (oli != ta->_textures.end()) {
    return -1;
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::compose_impl
//       Access: Protected, Virtual
//  Description: Intended to be overridden by derived RenderAttrib
//               types to specify how two consecutive RenderAttrib
//               objects of the same type interact.
//
//               This should return the result of applying the other
//               RenderAttrib to a node in the scene graph below this
//               RenderAttrib, which was already applied.  In most
//               cases, the result is the same as the other
//               RenderAttrib (that is, a subsequent RenderAttrib
//               completely replaces the preceding one).  On the other
//               hand, some kinds of RenderAttrib (for instance,
//               ColorTransformAttrib) might combine in meaningful
//               ways.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
compose_impl(const RenderAttrib *other) const {
  const TextureAttrib *ta;
  DCAST_INTO_R(ta, other, 0);

  if (ta->_operation == O_set) {
    // If the other type is O_set, it doesn't matter what we are.
    return ta;
  }

  if (_operation == ta->_operation) {
    // If the operation types match, the composition is simply the
    // union.
    return do_add(ta, _operation);

  } else if (ta->_operation == O_remove) {
    // If the other operation type is remove, and our type is add or
    // set, then remove.
    return do_remove(ta, _operation);

  } else if (_operation == O_remove) {
    // If our type is remove, then the other one wins.
    return ta;

  } else {
    // Otherwise, the result is the union.
    return do_add(ta, _operation);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::invert_compose_impl
//       Access: Protected, Virtual
//  Description: Intended to be overridden by derived RenderAttrib
//               types to specify how two consecutive RenderAttrib
//               objects of the same type interact.
//
//               See invert_compose() and compose_impl().
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
invert_compose_impl(const RenderAttrib *other) const {
  // I think in this case the other attrib always wins.  Maybe this
  // needs a bit more thought.  It's hard to imagine that it's even
  // important to compute this properly.
  return other;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_default_impl
//       Access: Protected, Virtual
//  Description: Intended to be overridden by derived TextureAttrib
//               types to specify what the default property for a
//               TextureAttrib of this type should be.
//
//               This should return a newly-allocated TextureAttrib of
//               the same type that corresponds to whatever the
//               standard default for this kind of TextureAttrib is.
////////////////////////////////////////////////////////////////////
RenderAttrib *TextureAttrib::
make_default_impl() const {
  return new TextureAttrib;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::do_add
//       Access: Private
//  Description: Returns a new TextureAttrib that represents all the
//               stages of this attrib, with those of the other one
//               added in.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
do_add(const TextureAttrib *other, TextureAttrib::Operation op) const {
  Textures::const_iterator ai = _textures.begin();
  Textures::const_iterator bi = other->_textures.begin();

  // Create a new TextureAttrib that will hold the result.
  TextureAttrib *new_attrib = new TextureAttrib;
  new_attrib->_operation = op;
  insert_iterator<Textures> result = 
    inserter(new_attrib->_textures, new_attrib->_textures.end());

  while (ai != _textures.end() && bi != other->_textures.end()) {
    if ((*ai).first < (*bi).first) {
      // Here is a stage that we have in the original, which is not
      // present in the secondary.
      *result = *ai;
      new_attrib->_stages.push_back((*ai).first);
      ++ai;
      ++result;
    } else if ((*bi).first < (*ai).first) {
      // Here is a new stage we have in the secondary, that was not
      // present in the original.
      *result = *bi;
      new_attrib->_stages.push_back((*bi).first);
      ++bi;
      ++result;
    } else {
      // Here is a stage we have in both.
      *result = *ai;
      new_attrib->_stages.push_back((*ai).first);
      ++ai;
      ++bi;
      ++result;
    }
  }

  while (ai != _textures.end()) {
    *result = *ai;
    new_attrib->_stages.push_back((*ai).first);
    ++ai;
    ++result;
  }

  while (bi != other->_textures.end()) {
    *result = *bi;
    new_attrib->_stages.push_back((*bi).first);
    ++bi;
    ++result;
  }

  return return_new(new_attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::do_remove
//       Access: Private
//  Description: Returns a new TextureAttrib that represents all the
//               stages of this attrib, with those of the other one
//               removed.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
do_remove(const TextureAttrib *other, TextureAttrib::Operation op) const {
  Textures::const_iterator ai = _textures.begin();
  Textures::const_iterator bi = other->_textures.begin();

  // Create a new TextureAttrib that will hold the result.
  TextureAttrib *new_attrib = new TextureAttrib;
  new_attrib->_operation = op;
  insert_iterator<Textures> result = 
    inserter(new_attrib->_textures, new_attrib->_textures.end());

  while (ai != _textures.end() && bi != other->_textures.end()) {
    if ((*ai).first < (*bi).first) {
      // Here is a stage that we have in the original, which is
      // not present in the secondary.  Keep it.
      *result = *ai;
      new_attrib->_stages.push_back((*ai).first);
      ++ai;
      ++result;
    } else if ((*bi).first < (*ai).first) {
      // Here is a new stage we have in the secondary, that was
      // not present in the original.  Ignore it.
      ++bi;
    } else {
      // Here is a stage we have in both.  Drop it.
      ++ai;
      ++bi;
    }
  }

  while (ai != _textures.end()) {
    *result = *ai;
    new_attrib->_stages.push_back((*ai).first);
    ++ai;
    ++result;
  }

  return return_new(new_attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::register_with_read_factory
//       Access: Public, Static
//  Description: Tells the BamReader how to create objects of type
//               TextureAttrib.
////////////////////////////////////////////////////////////////////
void TextureAttrib::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::write_datagram
//       Access: Public, Virtual
//  Description: Writes the contents of this object to the datagram
//               for shipping out to a Bam file.
////////////////////////////////////////////////////////////////////
void TextureAttrib::
write_datagram(BamWriter *manager, Datagram &dg) {
  RenderAttrib::write_datagram(manager, dg);

  manager->write_pointer(dg, get_texture());
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::complete_pointers
//       Access: Public, Virtual
//  Description: Receives an array of pointers, one for each time
//               manager->read_pointer() was called in fillin().
//               Returns the number of pointers processed.
////////////////////////////////////////////////////////////////////
int TextureAttrib::
complete_pointers(TypedWritable **p_list, BamReader *manager) {
  int pi = RenderAttrib::complete_pointers(p_list, manager);

  TypedWritable *texture = p_list[pi++];
  if (texture != (TypedWritable *)NULL) {
    TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
    _operation = O_set;
    _stages.push_back(tex_mgr->get_default_stage());
    _textures[tex_mgr->get_default_stage()] = DCAST(Texture, texture);
    _sort_seq = tex_mgr->get_sort_seq();
  }

  return pi;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_from_bam
//       Access: Protected, Static
//  Description: This function is called by the BamReader's factory
//               when a new object of type TextureAttrib is encountered
//               in the Bam file.  It should create the TextureAttrib
//               and extract its information from the file.
////////////////////////////////////////////////////////////////////
TypedWritable *TextureAttrib::
make_from_bam(const FactoryParams &params) {
  TextureAttrib *attrib = new TextureAttrib;
  DatagramIterator scan;
  BamReader *manager;

  parse_params(params, scan, manager);
  attrib->fillin(scan, manager);

  return attrib;
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::fillin
//       Access: Protected
//  Description: This internal function is called by make_from_bam to
//               read in all of the relevant data from the BamFile for
//               the new TextureAttrib.
////////////////////////////////////////////////////////////////////
void TextureAttrib::
fillin(DatagramIterator &scan, BamReader *manager) {
  RenderAttrib::fillin(scan, manager);

  // Read the _texture pointer.
  manager->read_pointer(scan);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::sort_stages
//       Access: Private
//  Description: Sorts the list of stages so that they are listed in
//               render order.
////////////////////////////////////////////////////////////////////
void TextureAttrib::
sort_stages() {
  sort(_stages.begin(), _stages.end(), IndirectLess<TextureStage>());

  TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
  _sort_seq = tex_mgr->get_sort_seq();
}
