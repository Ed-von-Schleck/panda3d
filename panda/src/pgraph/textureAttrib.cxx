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
  return make_on(tex_mgr->get_default_stage(), texture);
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
//     Function: TextureAttrib::make_set
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that replaces
//               any previous texture stages with the indicated stage
//               only.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_set(TextureStage *stage, Texture *tex) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_off_all_stages = true;
  attrib->_on_stages.push_back(stage);
  attrib->_on_textures[stage] = tex;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_on
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns on
//               the indicated stage, but does not affect any other
//               stages.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_on(TextureStage *stage, Texture *tex) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_on_stages.push_back(stage);
  attrib->_on_textures[stage] = tex;

  attrib->_sort_seq = UpdateSeq::old();
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_off
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns off
//               the indicated stage, but does not affect any other
//               stages.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_off(TextureStage *stage) {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_off_stages.insert(stage);

  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::make_all_off
//       Access: Published, Static
//  Description: Constructs a new TextureAttrib object that turns off
//               all stages (and hence disables texturing).
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
make_all_off() {
  TextureAttrib *attrib = new TextureAttrib;
  attrib->_off_all_stages = true;
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::add_on_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage added to the list of stages
//               turned on by this attrib.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
add_on_stage(TextureStage *stage, Texture *tex) const {
  TextureAttrib *attrib = new TextureAttrib(*this);
  pair<OnTextures::iterator, bool> insert_result = 
    attrib->_on_textures.insert(OnTextures::value_type(stage, tex));
  if (insert_result.second) {
    // If the insert was successful--we have added a new stage that
    // wasn't present before--then add the stage to the linear list
    // also.
    attrib->_on_stages.push_back(stage);
    attrib->_sort_seq = UpdateSeq::old();

    // Also ensure it is removed from the off_stages list.
    attrib->_off_stages.erase(stage);

  } else {
    // If the insert was unsuccessful, it means there was already a
    // definition for that stage.  Replace it.
    (*insert_result.first).second = tex;
  }
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::remove_on_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage removed from the list of
//               stages turned on by this attrib.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
remove_on_stage(TextureStage *stage) const {
  TextureAttrib *attrib = new TextureAttrib(*this);

  OnTextures::iterator ti = attrib->_on_textures.find(stage);
  if (ti != attrib->_on_textures.end()) {
    attrib->_on_textures.erase(ti);
    OnStages::iterator si = 
      find(attrib->_on_stages.begin(), attrib->_on_stages.end(), 
           PT(TextureStage)(stage));
    if (si != attrib->_on_stages.end()) {
      attrib->_on_stages.erase(si);
    }
  }

  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::add_off_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage added to the list of stages
//               turned off by this attrib.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
add_off_stage(TextureStage *stage) const {
  TextureAttrib *attrib = new TextureAttrib(*this);
  if (!_off_all_stages) {
    attrib->_off_stages.insert(stage);

    // Also ensure it is removed from the on_stages list.
    OnTextures::iterator ti = attrib->_on_textures.find(stage);
    if (ti != attrib->_on_textures.end()) {
      attrib->_on_textures.erase(ti);
      OnStages::iterator si = 
        find(attrib->_on_stages.begin(), attrib->_on_stages.end(), 
             PT(TextureStage)(stage));
      if (si != attrib->_on_stages.end()) {
        attrib->_on_stages.erase(si);
      }
    }
  }
  return return_new(attrib);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureAttrib::remove_off_stage
//       Access: Published
//  Description: Returns a new TextureAttrib, just like this one, but
//               with the indicated stage removed from the list of
//               stages turned off by this attrib.
////////////////////////////////////////////////////////////////////
CPT(RenderAttrib) TextureAttrib::
remove_off_stage(TextureStage *stage) const {
  TextureAttrib *attrib = new TextureAttrib(*this);
  attrib->_off_stages.erase(stage);
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
  check_sorted();

  out << get_type() << ":";
  if (_off_stages.empty()) {
    if (_on_stages.empty()) {
      if (_off_all_stages) {
        out << "all off";
      } else {
        out << "identity";
      }
    } else {
      if (_off_all_stages) {
        out << "set";
      } else {
        out << "on";
      }
    }

  } else {
    out << "off";
    OffStages::const_iterator fi;
    for (fi = _off_stages.begin(); fi != _off_stages.end(); ++fi) {
      TextureStage *stage = (*fi);
      out << " " << stage->get_name();
    }

    if (!_on_stages.empty()) {
      out << " on";
    }
  }
    
  OnStages::const_iterator li;
  for (li = _on_stages.begin(); li != _on_stages.end(); ++li) {
    TextureStage *stage = (*li);
    OnTextures::const_iterator ti = _on_textures.find(stage);
    if (ti != _on_textures.end()) {
      Texture *tex = (*ti).second;
      out << " " << stage->get_name() << ":" << tex->get_name();
    } else {
      out << " " << stage->get_name();
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

  if (_off_all_stages != ta->_off_all_stages) {
    return (int)_off_all_stages - (int)ta->_off_all_stages;
  }

  OnTextures::const_iterator li = _on_textures.begin();
  OnTextures::const_iterator oli = ta->_on_textures.begin();

  while (li != _on_textures.end() && oli != ta->_on_textures.end()) {
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

  if (li != _on_textures.end()) {
    return 1;
  }
  if (oli != ta->_on_textures.end()) {
    return -1;
  }

  OffStages::const_iterator fi = _off_stages.begin();
  OffStages::const_iterator ofi = ta->_off_stages.begin();

  while (fi != _off_stages.end() && ofi != ta->_off_stages.end()) {
    TextureStage *stage = (*fi);
    TextureStage *other_stage = (*ofi);

    if (stage != other_stage) {
      return stage < other_stage ? -1 : 1;
    }

    ++fi;
    ++ofi;
  }

  if (fi != _off_stages.end()) {
    return 1;
  }
  if (ofi != ta->_off_stages.end()) {
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

  if (ta->_off_all_stages) {
    // If the other type turns off all stages, it doesn't matter what
    // we are.
    return ta;
  }

  // This is a three-way merge between ai, bi, and ci, except that bi
  // and ci should have no intersection and therefore needn't be
  // compared to each other.
  OnTextures::const_iterator ai = _on_textures.begin();
  OnTextures::const_iterator bi = ta->_on_textures.begin();
  OffStages::const_iterator ci = ta->_off_stages.begin();

  // Create a new TextureAttrib that will hold the result.
  TextureAttrib *new_attrib = new TextureAttrib;
  insert_iterator<OnTextures> result = 
    inserter(new_attrib->_on_textures, new_attrib->_on_textures.end());

  while (ai != _on_textures.end() && 
         bi != ta->_on_textures.end() && 
         ci != ta->_off_stages.end()) {
    if ((*ai).first < (*bi).first) {
      if ((*ai).first < (*ci)) {
        // Here is a stage that we have in the original, which is not
        // present in the secondary.
        *result = *ai;
        new_attrib->_on_stages.push_back((*ai).first);
        ++ai;
        ++result;

      } else if ((*ci) < (*ai).first) {
        // Here is a stage that is turned off in the secondary, but
        // was not present in the original.
        ++ci;

      } else { // (*ci) == (*ai).first
        // Here is a stage that is turned off in the secondary, and
        // was present in the original.
        ++ai;
        ++ci;
      }

    } else if ((*bi).first < (*ai).first) {
      // Here is a new stage we have in the secondary, that was not
      // present in the original.
      *result = *bi;
      new_attrib->_on_stages.push_back((*bi).first);
      ++bi;
      ++result;

    } else {  // (*bi).first == (*ai).first
      // Here is a stage we have in both.
      *result = *ai;
      new_attrib->_on_stages.push_back((*ai).first);
      ++ai;
      ++bi;
      ++result;
    }
  }

  while (ai != _on_textures.end() && bi != ta->_on_textures.end()) {
    if ((*ai).first < (*bi).first) {
      // Here is a stage that we have in the original, which is not
      // present in the secondary.
      *result = *ai;
      new_attrib->_on_stages.push_back((*ai).first);
      ++ai;
      ++result;

    } else if ((*bi).first < (*ai).first) {
      // Here is a new stage we have in the secondary, that was not
      // present in the original.
      *result = *bi;
      new_attrib->_on_stages.push_back((*bi).first);
      ++bi;
      ++result;

    } else {
      // Here is a stage we have in both.
      *result = *ai;
      new_attrib->_on_stages.push_back((*ai).first);
      ++ai;
      ++bi;
      ++result;
    }
  }

  while (ai != _on_textures.end() && ci != ta->_off_stages.end()) {
    if ((*ai).first < (*ci)) {
      // Here is a stage that we have in the original, which is not
      // present in the secondary.
      *result = *ai;
      new_attrib->_on_stages.push_back((*ai).first);
      ++ai;
      ++result;
      
    } else if ((*ci) < (*ai).first) {
      // Here is a stage that is turned off in the secondary, but
      // was not present in the original.
      ++ci;
      
    } else { // (*ci) == (*ai).first
      // Here is a stage that is turned off in the secondary, and
      // was present in the original.
      ++ai;
      ++ci;
    }
  }

  while (ai != _on_textures.end()) {
    *result = *ai;
    new_attrib->_on_stages.push_back((*ai).first);
    ++ai;
    ++result;
  }

  while (bi != ta->_on_textures.end()) {
    *result = *bi;
    new_attrib->_on_stages.push_back((*bi).first);
    ++bi;
    ++result;
  }

  return return_new(new_attrib);
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
    _on_stages.push_back(tex_mgr->get_default_stage());
    _on_textures[tex_mgr->get_default_stage()] = DCAST(Texture, texture);
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
//     Function: TextureAttrib::sort_on_stages
//       Access: Private
//  Description: Sorts the list of stages so that they are listed in
//               render order.
////////////////////////////////////////////////////////////////////
void TextureAttrib::
sort_on_stages() {
  sort(_on_stages.begin(), _on_stages.end(), IndirectLess<TextureStage>());

  TextureStageManager *tex_mgr = TextureStageManager::get_global_ptr();
  _sort_seq = tex_mgr->get_sort_seq();
}
