// Filename: graphicsStateGuardian.cxx
// Created by:  drose (02Feb99)
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


#include "graphicsStateGuardian.h"
#include "config_display.h"
#include "textureContext.h"
#include "renderBuffer.h"
#include "colorAttrib.h"
#include "renderState.h"
#include "depthWriteAttrib.h"
#include "colorWriteAttrib.h"

#include "clockObject.h"
#include "geomNode.h"
#include "pStatTimer.h"

#include <algorithm>

#ifndef CPPPARSER
PStatCollector GraphicsStateGuardian::_total_texusage_pcollector("Texture usage");
PStatCollector GraphicsStateGuardian::_active_texusage_pcollector("Texture usage:Active");
PStatCollector GraphicsStateGuardian::_total_geom_pcollector("Prepared Geoms");
PStatCollector GraphicsStateGuardian::_active_geom_pcollector("Prepared Geoms:Active");
PStatCollector GraphicsStateGuardian::_total_geom_node_pcollector("Prepared GeomNodes");
PStatCollector GraphicsStateGuardian::_active_geom_node_pcollector("Prepared GeomNodes:Active");
PStatCollector GraphicsStateGuardian::_total_texmem_pcollector("Texture memory");
PStatCollector GraphicsStateGuardian::_used_texmem_pcollector("Texture memory:In use");
PStatCollector GraphicsStateGuardian::_texmgrmem_total_pcollector("Texture manager");
PStatCollector GraphicsStateGuardian::_texmgrmem_resident_pcollector("Texture manager:Resident");
PStatCollector GraphicsStateGuardian::_vertices_pcollector("Vertices");
PStatCollector GraphicsStateGuardian::_vertices_tristrip_pcollector("Vertices:Triangle strips");
PStatCollector GraphicsStateGuardian::_vertices_trifan_pcollector("Vertices:Triangle fans");
PStatCollector GraphicsStateGuardian::_vertices_tri_pcollector("Vertices:Triangles");
PStatCollector GraphicsStateGuardian::_vertices_other_pcollector("Vertices:Other");
PStatCollector GraphicsStateGuardian::_state_changes_pcollector("State changes");
PStatCollector GraphicsStateGuardian::_transform_state_pcollector("State changes:Transforms");
PStatCollector GraphicsStateGuardian::_texture_state_pcollector("State changes:Textures");
PStatCollector GraphicsStateGuardian::_nodes_pcollector("Nodes");
PStatCollector GraphicsStateGuardian::_geom_nodes_pcollector("Nodes:GeomNodes");
PStatCollector GraphicsStateGuardian::_frustum_cull_volumes_pcollector("Cull volumes");
PStatCollector GraphicsStateGuardian::_frustum_cull_transforms_pcollector("Cull volumes:Transforms");

PStatCollector GraphicsStateGuardian::_set_state_pcollector("Draw:Set state");
PStatCollector GraphicsStateGuardian::_draw_primitive_pcollector("Draw:Primitive");

#endif

TypeHandle GraphicsStateGuardian::_type_handle;
TypeHandle GraphicsStateGuardian::GsgWindow::_type_handle;

GraphicsStateGuardian::GsgFactory *GraphicsStateGuardian::_factory = NULL;

GraphicsStateGuardian::GsgWindow::~GsgWindow(void) {}

TypeHandle GraphicsStateGuardian::GsgWindow::get_class_type(void) {
  return _type_handle;
}

void GraphicsStateGuardian::GsgWindow::init_type(void) {
  GsgParam::init_type();
  register_type(_type_handle, "GraphicsStateGuardian::GsgWindow",
                GsgParam::get_class_type());
}

TypeHandle GraphicsStateGuardian::GsgWindow::get_type(void) const {
  return get_class_type();
}

TypeHandle GraphicsStateGuardian::GsgWindow::force_init_type(void) {
  init_type();
  return get_class_type();
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
GraphicsStateGuardian::
GraphicsStateGuardian(GraphicsWindow *win) {
  _win = win;
  _coordinate_system = default_coordinate_system;
  _current_display_region = (DisplayRegion*)0L;
  _current_lens = (Lens *)NULL;
  reset();
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
GraphicsStateGuardian::
~GraphicsStateGuardian() {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::reset
//       Access: Public, Virtual
//  Description: Resets all internal state as if the gsg were newly
//               created.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
reset() {
  _display_region_stack_level = 0;
  _frame_buffer_stack_level = 0;
  _lens_stack_level = 0;

  _state.clear();
  _qpstate = RenderState::make_empty();

  _buffer_mask = 0;
  _color_clear_value.set(gsg_clear_r, gsg_clear_g, gsg_clear_b, 0.0);
  _depth_clear_value = 1.0;
  _stencil_clear_value = 0.0;
  _accum_clear_value.set(0.0, 0.0, 0.0, 0.0);
  _clear_buffer_type = RenderBuffer::T_back | RenderBuffer::T_depth;
  _normals_enabled = false;

  //Color and alpha transform variables
  _color_transform_enabled = false;
  _alpha_transform_enabled = false;
  _current_color_mat = LMatrix4f::ident_mat();
  _current_alpha_offset = 0;
  _current_alpha_scale = 1;

  _has_scene_graph_color = false;
  _issued_color_stale = false;
  _vertex_colors_enabled = true;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::modify_state
//       Access: Public
//  Description: Applies the transitions indicated in the state set to
//               the current state, and issues the changes to the
//               graphics hardware.
//
//               Any transitions not mentioned in new_state are left
//               unchanged.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
modify_state(const NodeTransitions &new_state) {
  PStatTimer timer(_set_state_pcollector);

  if (gsg_cat.is_debug()) {
    gsg_cat.debug() << "\n";
    gsg_cat.debug()
      << "Frame " << ClockObject::get_global_clock()->get_frame_count()
      << ", setting via NodeTransitions\n";
    new_state.write(gsg_cat.debug(false), 10);
  }

  NodeTransitions::const_iterator new_i;
  State::iterator current_i;

  new_i = new_state.begin();
  current_i = _state.begin();

  while (new_i != new_state.end() && current_i != _state.end()) {
    TypeHandle new_type = (*new_i).first;
    NodeTransition *new_trans = (*new_i).second;
    TypeHandle current_type = (*current_i)._type;
    PT(NodeTransition) &current_trans = (*current_i)._trans;

    if (new_type < current_type) {
      // The user requested setting an attribute that we've never set
      // before.  Issue the command.

      if (new_trans != (NodeTransition *)NULL) {
        if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Issuing new attrib " << *new_trans << "\n";
        }
        record_state_change(new_type);
        new_trans->issue(this);

        // And store the new value.  This is a terribly slow way to
        // insert an element into a vector, but it happens only very
        // rarely.
        current_i = _state.insert(current_i, *new_i);
        ++current_i;
      }

      ++new_i;

    } else if (current_type < new_type) {
      // Here's an attribute that we've set previously, but the user
      // didn't specify this time.
      ++current_i;

    } else {  // current_type == new_type

      if (new_trans == (NodeTransition *)NULL) {
        // Here's an attribute that we've set previously, which
        // appears in the new list, but is NULL indicating it should
        // be removed.
        ++current_i;
        ++new_i;

      } else if (current_trans == (NodeTransition *)NULL) {
        // Here's a new attribute which we had previously set NULL,
        // indicating the initial attribute.
        if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Issuing previously NULL attrib " << *new_trans << "\n";
        }
        record_state_change(new_type);
        new_trans->issue(this);
        
        // And store the new value.
        current_trans = new_trans;

      } else {
        // Here's an attribute that we've set previously, and the user
        // asked us to set it again.  Issue the command only if the new
        // attribute is different from that which we'd set before.
        if (new_trans != current_trans) {
          if (!compare_state_by_pointer &&
              new_trans->compare_to_ignore_priority(*current_trans) == 0) {
            // Oops, different pointers, same value.

            // Get an assignable reference to the source.  We modify
            // the source in this way in a trivial manner--we replace
            // the transition in the source with an identical one--in
            // the hopes that this will reduce the need for future
            // comparisons.
            PT(NodeTransition) &new_trans_assign = 
              (PT(NodeTransition) &)(*new_i).second;
            new_trans_assign = current_trans;

            if (gsg_cat.is_debug()) {
              gsg_cat.debug()
                << "Updating pointer for attrib " << *new_trans << "\n";
            }

          } else {
            if (gsg_cat.is_debug()) {
              gsg_cat.debug()
                << "Reissuing attrib " << *new_trans << "\n";
              gsg_cat.debug()
                << "Previous was " << *current_trans << "\n";
            }
            record_state_change(new_type);
            new_trans->issue(this);
            
            // And store the new value.
            current_trans = new_trans;
          }

        } else if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Not reissuing unchanged attrib " << *new_trans << "\n";
        }

        ++current_i;
        ++new_i;
      }
    }
  }

  while (new_i != new_state.end()) {
    // The user requested setting an attribute that we've never set
    // before.  Issue the command.
    TypeHandle new_type = (*new_i).first;
    NodeTransition *new_trans = (*new_i).second;

    if (new_trans != (NodeTransition *)NULL) {
      if (gsg_cat.is_debug()) {
        gsg_cat.debug()
          << "Issuing new attrib " << *new_trans << "\n";
      }
      record_state_change(new_type);

      new_trans->issue(this);

      // And store the new value.
      _state.push_back(*new_i);
    }
    ++new_i;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_state
//       Access: Public
//  Description: Applies the transitions indicated in the state set to
//               the current state, and issues the changes to the
//               graphics hardware.
//
//               The state is taken to be a complete description of
//               what the graphics state should be; any transitions
//               not mentioned in new_state are implicitly reset to
//               their initial values.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_state(const NodeTransitionCache &new_state) {
  PStatTimer timer(_set_state_pcollector);

  if (gsg_cat.is_debug()) {
    gsg_cat.debug() << "\n";
    gsg_cat.debug()
      << "Frame " << ClockObject::get_global_clock()->get_frame_count()
      << ", setting to\n";
    new_state.write(gsg_cat.debug(false), 10);
  }

  NodeTransitionCache::const_iterator new_i;
  State::iterator current_i;

  new_i = new_state.begin();
  current_i = _state.begin();

  while (new_i != new_state.end() && current_i != _state.end()) {
    TypeHandle new_type = (*new_i).first;
    NodeTransition *new_trans = (*new_i).second.get_trans();
    TypeHandle current_type = (*current_i)._type;
    PT(NodeTransition) &current_trans = (*current_i)._trans;

    if (new_type < current_type) {
      // The user requested setting an attribute that we've never set
      // before.  Issue the command.

      if (new_trans != (NodeTransition *)NULL) {
        if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Issuing new attrib " << *new_trans << "\n";
        }
        record_state_change(new_type);
        new_trans->issue(this);

        // And store the new value.  This is a terribly slow way to
        // insert an element into a vector, but it happens only very
        // rarely.
        current_i = _state.insert(current_i, *new_i);
        ++current_i;
      }

      ++new_i;

    } else if (current_type < new_type) {
      // Here's an attribute that we've set previously, but the user
      // didn't specify this time.

      if (current_trans != (NodeTransition *)NULL) {
        // This attribute should now get the default initial value.

        if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Unissuing attrib " << *current_trans
            << " (previously set, not now)\n";
        }
        record_state_change(current_type);

        PT(NodeTransition) initial = current_trans->make_initial();
        initial->issue(this);

        current_trans = NULL;
      }

      ++current_i;

    } else {  // current_type == new_type

      if (new_trans == (NodeTransition *)NULL) {
        // Here's an attribute that we've set previously, which
        // appears in the new list, but is NULL indicating it should
        // be removed.
        if (current_trans != (NodeTransition *)NULL) {
          if (gsg_cat.is_debug()) {
            gsg_cat.debug()
              << "Unissuing attrib " << *current_trans
              << " (previously set, now NULL)\n";
          }
          record_state_change(current_type);

          // Issue the initial attribute before clearing the state.
          PT(NodeTransition) initial = current_trans->make_initial();
          initial->issue(this);

          current_trans = NULL;
        }

      } else if (current_trans == (NodeTransition *)NULL) {
        // Here's a new attribute which we had previously set NULL,
        // indicating the initial attribute.
        if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Issuing previously NULL attrib " << *new_trans << "\n";
        }
        record_state_change(new_type);
        new_trans->issue(this);
        
        // And store the new value.
        current_trans = new_trans;
        
      } else {
        // Here's an attribute that we've set previously, and the user
        // asked us to set it again.  Issue the command only if the new
        // attribute is different from that which we'd set before.
        if (new_trans != current_trans) {
          if (!compare_state_by_pointer &&
              new_trans->compare_to_ignore_priority(*current_trans) == 0) {
            // Oops, different pointers, same value.

            // Get an assignable reference to the source.  We modify
            // the source in this way in a trivial manner--we replace
            // the transition in the source with an identical one--in
            // the hopes that this will reduce the need for future
            // comparisons.
            NodeTransitionCacheEntry &new_trans_assign = 
              (NodeTransitionCacheEntry &)(*new_i).second;
            new_trans_assign.set_trans(current_trans);

          } else {
            if (gsg_cat.is_debug()) {
              gsg_cat.debug()
                << "Reissuing attrib " << *new_trans << "\n";
              gsg_cat.debug()
                << "Previous was " << *current_trans << "\n";
            }
            record_state_change(new_type);
            new_trans->issue(this);
            
            // And store the new value.
            current_trans = new_trans;
          }

        } else if (gsg_cat.is_debug()) {
          gsg_cat.debug()
            << "Not reissuing unchanged attrib " << *new_trans << "\n";
        }
      }

      ++current_i;
      ++new_i;
    }
  }

  while (current_i != _state.end()) {
    // Here's an attribute that we've set previously, but the user
    // didn't specify this time.
    TypeHandle current_type = (*current_i)._type;
    PT(NodeTransition) &current_trans = (*current_i)._trans;
    
    if (current_trans != (NodeTransition *)NULL) {
      if (gsg_cat.is_debug()) {
        gsg_cat.debug()
          << "Unissuing attrib " << *current_trans
          << " (previously set, end of list)\n";
      }
      record_state_change(current_type);
      
      // This attribute should now get the default initial value.
      PT(NodeTransition) initial = current_trans->make_initial();
      initial->issue(this);

      current_trans = NULL;
    }
    
    ++current_i;
  }

  while (new_i != new_state.end()) {
    // The user requested setting an attribute that we've never set
    // before.  Issue the command.
    TypeHandle new_type = (*new_i).first;
    NodeTransition *new_trans = (*new_i).second.get_trans();

    if (new_trans != (NodeTransition *)NULL) {
      if (gsg_cat.is_debug()) {
        gsg_cat.debug()
          << "Issuing new attrib " << *new_trans << "\n";
      }
      record_state_change(new_type);

      new_trans->issue(this);

      // And store the new value.
      _state.push_back(*new_i);
    }
    ++new_i;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::get_render_buffer
//       Access: Public
//  Description: Returns a RenderBuffer object suitable for operating
//               on the requested set of buffers.  buffer_type is the
//               union of all the desired RenderBuffer::Type values.
////////////////////////////////////////////////////////////////////
RenderBuffer GraphicsStateGuardian::
get_render_buffer(int buffer_type) {
  return RenderBuffer(this, buffer_type & _buffer_mask);
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_color_clear_value
//       Access: Public
//  Description: Sets the color that the next clear() command will set
//               the color buffer to
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_color_clear_value(const Colorf& value) {
  _color_clear_value = value;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_depth_clear_value
//       Access: Public
//  Description: Sets the depth that the next clear() command will set
//               the depth buffer to
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_depth_clear_value(const float value) {
  _depth_clear_value = value;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_stencil_clear_value
//       Access: Public
//  Description: Sets the value that the next clear() command will set
//               the stencil buffer to
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_stencil_clear_value(const bool value) {
  _stencil_clear_value = value;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_accum_clear_value
//       Access: Public
//  Description: Sets the color that the next clear() command will set
//               the accumulation buffer to
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_accum_clear_value(const Colorf& value) {
  _accum_clear_value = value;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::enable_frame_clear
//       Access: Public
//  Description: Activates or deactivates the automatic clearing of
//               the frame buffer and/or depth buffer at the beginning
//               of each frame.
//
//               If clear_color is true, the color buffer will be
//               cleared; if clear_depth is true, the depth buffer
//               will be cleared.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
enable_frame_clear(bool clear_color, bool clear_depth) {
  _clear_buffer_type = 0;
  if (clear_color) {
    _clear_buffer_type |= RenderBuffer::T_back;
  }
  if (clear_depth) {
    _clear_buffer_type |= RenderBuffer::T_depth;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::release_all_textures
//       Access: Public
//  Description: Frees the resources for all textures associated with
//               this GSG.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
release_all_textures() {
  // We must get a copy of the _prepared_textures list first, because
  // each call to release_texture() will remove that texture from the
  // list, and we don't want to traverse a list while we're modifying
  // it!

  Textures temp = _prepared_textures;
  for (Textures::const_iterator ti = temp.begin();
       ti != temp.end();
       ++ti) {
    release_texture(*ti);
  }

  // Now that we've released all of the textures, the
  // _prepared_textures list should have completely emptied itself.
  nassertv(_prepared_textures.empty());
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::release_all_geoms
//       Access: Public
//  Description: Frees the resources for all Geoms and GeomNodes
//               associated with this GSG.  Warning!  This may make
//               the Geoms unrenderable, if the Panda-level
//               information has been deleted.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
release_all_geoms() {
  // As above, for both Geoms and GeomNodes.

  Geoms temp_geoms = _prepared_geoms;
  for (Geoms::const_iterator gi = temp_geoms.begin();
       gi != temp_geoms.end();
       ++gi) {
    release_geom(*gi);
  }

  GeomNodes temp_geom_nodes = _prepared_geom_nodes;
  for (GeomNodes::const_iterator gni = temp_geom_nodes.begin();
       gni != temp_geom_nodes.end();
       ++gni) {
    release_geom_node(*gni);
  }

  nassertv(_prepared_geoms.empty());
  nassertv(_prepared_geom_nodes.empty());
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::clear_attribute
//       Access: Public
//  Description: Explicitly clear the indicated attribute, specified
//               by the TypeHandle of its associated transition.  If
//               the attribute is not set already, this does nothing;
//               if it is set, it resets it to its default value.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
clear_attribute(TypeHandle type) {
  // Look for the transition in our state vector.  We'll use STL's
  // binary search functions to do this.

  State::iterator si =
    lower_bound(_state.begin(), _state.end(), StateInfo(type));

  if (si != _state.end() && (*si)._type == type) {
    // We found it.
    PT(NodeTransition) &current_trans = (*si)._trans;

    if (current_trans != (NodeTransition *)NULL) {
      // The state was already set; get the initial value and reset it.
      PT(NodeTransition) initial = current_trans->make_initial();
      initial->issue(this);
      current_trans = (NodeTransition *)NULL;
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::get_attribute
//       Access: Public
//  Description: Returns the NodeTransition from the current state
//               associated with the indicated type, or NULL if there
//               is no such transition set.
////////////////////////////////////////////////////////////////////
NodeTransition *GraphicsStateGuardian::
get_attribute(TypeHandle type) const {
  State::const_iterator si =
    lower_bound(_state.begin(), _state.end(), StateInfo(type));

  if (si != _state.end() && (*si)._type == type) {
    return (*si)._trans;
  }

  return (NodeTransition *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::prepare_texture
//       Access: Public, Virtual
//  Description: Prepares the indicated texture for retained-mode
//               rendering.  In the future, this texture may be
//               applied simply by calling apply_texture() with the
//               value returned by this function.
////////////////////////////////////////////////////////////////////
TextureContext *GraphicsStateGuardian::
prepare_texture(Texture *) {
  return (TextureContext *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::apply_texture
//       Access: Public, Virtual
//  Description: Applies the texture previously indicated via a call
//               to prepare_texture() to the graphics state, so that
//               geometry rendered in the future will be rendered with
//               the given texture.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
apply_texture(TextureContext *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::release_texture
//       Access: Public, Virtual
//  Description: Frees the resources previously allocated via a call
//               to prepare_texture(), including deleting the
//               TextureContext itself, if necessary.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
release_texture(TextureContext *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::prepare_geom_node
//       Access: Public, Virtual
//  Description: Prepares the indicated GeomNode for retained-mode
//               rendering.  If this function returns non-NULL, the
//               value returned will be passed back to a future call
//               to draw_geom_node(), which is expected to draw the
//               contents of the node.
////////////////////////////////////////////////////////////////////
GeomNodeContext *GraphicsStateGuardian::
prepare_geom_node(GeomNode *) {
  return (GeomNodeContext *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::draw_geom_node
//       Access: Public, Virtual
//  Description: Draws a GeomNode previously indicated by a call to
//               prepare_geom_node().
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
draw_geom_node(GeomNode *node, GeomNodeContext *) {
  int num_geoms = node->get_num_geoms();
  for (int i = 0; i < num_geoms; i++) {
    node->get_geom(i)->draw(this);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::release_geom_node
//       Access: Public, Virtual
//  Description: Frees the resources previously allocated via a call
//               to prepare_geom_node(), including deleting the
//               GeomNodeContext itself, if necessary.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
release_geom_node(GeomNodeContext *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::prepare_geom
//       Access: Public, Virtual
//  Description: Prepares the indicated Geom for retained-mode
//               rendering.  The value returned by this function will
//               be passed back into future calls to draw_tristrip(),
//               etc., along with the Geom pointer.
////////////////////////////////////////////////////////////////////
GeomContext *GraphicsStateGuardian::
prepare_geom(Geom *) {
  return (GeomContext *)NULL;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::release_geom
//       Access: Public, Virtual
//  Description: Frees the resources previously allocated via a call
//               to prepare_geom(), including deleting the GeomContext
//               itself, if necessary.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
release_geom(GeomContext *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::set_state_and_transform
//       Access: Public, Virtual
//  Description: Simultaneously resets the render state and the
//               transform state.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
set_state_and_transform(const RenderState *state,
                        const TransformState *transform) {
  set_state(state);
  set_transform(transform);
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::clear_framebuffer
//       Access: Public, Virtual
//  Description: Erases the contents of the framebuffer, according to
//               _clear_buffer_type, which is set by
//               enable_frame_clear().
//
//               This is used to prepare the framebuffer for drawing
//               a new frame.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
clear_framebuffer() {
  if (_clear_buffer_type != 0) {
    PT(DisplayRegion) win_dr =
      _win->make_scratch_display_region(_win->get_width(), _win->get_height());
    nassertv(win_dr != (DisplayRegion*)NULL);
    DisplayRegionStack old_dr = push_display_region(win_dr);
    prepare_display_region();
    clear(get_render_buffer(_clear_buffer_type));
    pop_display_region(old_dr);
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::prepare_lens
//       Access: Public, Virtual
//  Description: Makes the current lens (whichever lens was most
//               recently specified with push_lens()) active, so that
//               it will transform future rendered geometry.  Normally
//               this is only called from the draw process, and
//               usually it is called immediately after a call to
//               push_lens().
//
//               The return value is true if the lens is acceptable,
//               false if it is not.
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
prepare_lens() {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::wants_normals
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
wants_normals() const {
  return _normals_enabled;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::wants_texcoords
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
wants_texcoords() const {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::wants_colors
//       Access: Public, Virtual
//  Description: Returns true if the GSG should issue geometry color
//               commands, false otherwise.
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
wants_colors() const {
  return _vertex_colors_enabled;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::begin_decal
//       Access: Public, Virtual
//  Description: This will be called to initiate decaling mode.  It is
//               passed the pointer to the GeomNode that will be the
//               destination of the decals, which it is expected that
//               the GSG will render normally; subsequent geometry
//               rendered up until the next call of end_decal() should
//               be rendered as decals of the base_geom.
//
//               The attributes wrapper is the current state as of the
//               base geometry node.  It may or may not be modified by
//               the GSG to reflect whatever rendering state is
//               necessary to render the decals properly.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
begin_decal(GeomNode *base_geom, AllTransitionsWrapper &attrib) {
  base_geom->draw(this);
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::end_decal
//       Access: Public, Virtual
//  Description: This will be called to terminate decaling mode.  It
//               is passed the same base_geom that was passed to
//               begin_decal().
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
end_decal(GeomNode *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::polygon_offset_decals
//       Access: Public, Virtual
//  Description: Returns true if this GSG can implement decals using a
//               PolygonOffsetAttrib, or false if that is unreliable
//               and the three-step rendering process should be used
//               instead.
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
polygon_offset_decals() {
  return false;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::begin_decal_base_first
//       Access: Public, Virtual
//  Description: Called during draw to begin a three-step rendering
//               phase to draw decals.  The first step,
//               begin_decal_base_first(), is called prior to drawing the
//               base geometry.  It should set up whatever internal
//               state is appropriate, as well as returning a
//               RenderState object that should be applied to the base
//               geometry for rendering.
////////////////////////////////////////////////////////////////////
CPT(RenderState) GraphicsStateGuardian::
begin_decal_base_first() {
  // Turn off writing the depth buffer to render the base geometry.
  static CPT(RenderState) decal_base_first;
  if (decal_base_first == (const RenderState *)NULL) {
    decal_base_first = RenderState::make
      (DepthWriteAttrib::make(DepthWriteAttrib::M_off));
  }
  return decal_base_first;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::begin_decal_nested
//       Access: Public, Virtual
//  Description: Called during draw to begin a three-step rendering
//               phase to draw decals.  The second step,
//               begin_decal_nested(), is called after drawing the
//               base geometry and prior to drawing any of the nested
//               decal geometry that is to be applied to the base
//               geometry.
////////////////////////////////////////////////////////////////////
CPT(RenderState) GraphicsStateGuardian::
begin_decal_nested() {
  // We should keep the depth buffer off during this operation, so
  // that decals on decals will render properly.
  static CPT(RenderState) decal_nested;
  if (decal_nested == (const RenderState *)NULL) {
    decal_nested = RenderState::make
      (DepthWriteAttrib::make(DepthWriteAttrib::M_off));
  }
  return decal_nested;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::begin_decal_base_second
//       Access: Public, Virtual
//  Description: Called during draw to begin a three-step rendering
//               phase to draw decals.  The third step,
//               begin_decal_base_second(), is called after drawing the
//               base geometry and the nested decal geometry, and
//               prior to drawing the base geometry one more time (if
//               needed).
//
//               It should return a RenderState object appropriate for
//               rendering the base geometry the second time, or NULL
//               if it is not necessary to re-render the base
//               geometry.
////////////////////////////////////////////////////////////////////
CPT(RenderState) GraphicsStateGuardian::
begin_decal_base_second() {
  // Now let the depth buffer go back on, but turn off writing the
  // color buffer to render the base geometry after the second pass.
  static CPT(RenderState) decal_base_second;
  if (decal_base_second == (const RenderState *)NULL) {
    decal_base_second = RenderState::make
      (ColorWriteAttrib::make(ColorWriteAttrib::M_off));
  }
  return decal_base_second;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::finish_decal
//       Access: Public, Virtual
//  Description: Called during draw to clean up after decals are
//               finished.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
finish_decal() {
  // No need to do anything special here.
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::get_internal_coordinate_system
//       Access: Public, Virtual
//  Description: Should be overridden by derived classes to return the
//               coordinate system used internally by the GSG, if any
//               one particular coordinate system is used.  The
//               default, CS_default, indicates that the GSG can use
//               any coordinate system.
//
//               If this returns other than CS_default, the
//               GraphicsEngine will automatically convert all
//               transforms into the indicated coordinate system.
////////////////////////////////////////////////////////////////////
CoordinateSystem GraphicsStateGuardian::
get_internal_coordinate_system() const {
  return CS_default;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::issue_transform
//       Access: Public, Virtual
//  Description: Sends the indicated transform matrix to the graphics
//               API to be applied to future vertices.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
issue_transform(const TransformState *) {
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::issue_color
//       Access: Public, Virtual
//  Description: This method is defined in the base class because it
//               is likely that this functionality will be used for
//               all (or at least most) kinds of
//               GraphicsStateGuardians--it's not specific to any one
//               rendering backend.
//
//               The ColorAttribute just changes the interpretation of
//               the color on the vertices, and fiddles with
//               _vertex_colors_enabled, etc.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
issue_color(const ColorAttrib *attrib) {
  switch (attrib->get_color_type()) {
  case ColorAttrib::T_flat:
    // Color attribute flat: it specifies a scene graph color that
    // overrides the vertex color.
    _scene_graph_color = attrib->get_color();
    _has_scene_graph_color = true;
    _vertex_colors_enabled = false;
    _issued_color_stale = true;
    break;

  case ColorAttrib::T_off:
    // Color attribute off: it specifies that no scene graph color is
    // in effect, and vertex color is not important either.
    _has_scene_graph_color = false;
    _issued_color_stale = false;
    _vertex_colors_enabled = false;
    break;

  case ColorAttrib::T_vertex:
    // Color attribute vertex: it specifies that vertex color should
    // be revealed.
    _has_scene_graph_color = false;
    _issued_color_stale = false;
    _vertex_colors_enabled = true;
    break;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::mark_prepared_texture
//       Access: Protected
//  Description: This is intended to be called from within
//               prepare_texture().  It adds the indicated
//               TextureContext pointer to the _prepared_textures set,
//               and returns true if it was successfully added
//               (i.e. it was not already in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
mark_prepared_texture(TextureContext *tc) {
  bool prepared = _prepared_textures.insert(tc).second;
#ifdef DO_PSTATS
  if (prepared) {
    _total_texusage_pcollector.add_level(tc->estimate_texture_memory());
  }
#endif
  return prepared;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::unmark_prepared_texture
//       Access: Protected
//  Description: This is intended to be called from within
//               release_texture().  It removes the indicated
//               TextureContext pointer from the _prepared_textures
//               set, and returns true if it was successfully removed
//               (i.e. it had been in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
unmark_prepared_texture(TextureContext *tc) {
  bool removed = (_prepared_textures.erase(tc) != 0);
#ifdef DO_PSTATS
  if (removed) {
    _total_texusage_pcollector.sub_level(tc->estimate_texture_memory());
  }
#endif
  return removed;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::mark_prepared_geom
//       Access: Protected
//  Description: This is intended to be called from within
//               prepare_geom().  It adds the indicated GeomContext
//               pointer to the _prepared_geoms set, and returns true
//               if it was successfully added (i.e. it was not already
//               in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
mark_prepared_geom(GeomContext *gc) {
  bool prepared = _prepared_geoms.insert(gc).second;
#ifdef DO_PSTATS
  if (prepared) {
    _total_geom_pcollector.add_level(1);
  }
#endif
  return prepared;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::unmark_prepared_geom
//       Access: Protected
//  Description: This is intended to be called from within
//               release_geom().  It removes the indicated GeomContext
//               pointer from the _prepared_geoms set, and returns
//               true if it was successfully removed (i.e. it had been
//               in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
unmark_prepared_geom(GeomContext *gc) {
  bool removed = (_prepared_geoms.erase(gc) != 0);
#ifdef DO_PSTATS
  if (removed) {
    _total_geom_pcollector.sub_level(1);
  }
#endif
  return removed;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::mark_prepared_geom_node
//       Access: Protected
//  Description: This is intended to be called from within
//               prepare_geom_node().  It adds the indicated
//               GeomNodeContext pointer to the _prepared_geom_nodes
//               set, and returns true if it was successfully added
//               (i.e. it was not already in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
mark_prepared_geom_node(GeomNodeContext *gnc) {
  bool prepared = _prepared_geom_nodes.insert(gnc).second;
#ifdef DO_PSTATS
  if (prepared) {
    _total_geom_node_pcollector.add_level(1);
  }
#endif
  return prepared;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::unmark_prepared_geom_node
//       Access: Protected
//  Description: This is intended to be called from within
//               release_geom_node().  It removes the indicated
//               GeomNodeContext pointer from the _prepared_geom_nodes
//               set, and returns true if it was successfully removed
//               (i.e. it had been in the set).
////////////////////////////////////////////////////////////////////
bool GraphicsStateGuardian::
unmark_prepared_geom_node(GeomNodeContext *gnc) {
  bool removed = (_prepared_geom_nodes.erase(gnc) != 0);
#ifdef DO_PSTATS
  if (removed) {
    _total_geom_node_pcollector.sub_level(1);
  }
#endif
  return removed;
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::close_gsg
//       Access: Protected, Virtual
//  Description: This is called by the associated GraphicsWindow when
//               close_window() is called.  It should null out the
//               _win pointer and possibly free any open resources
//               associated with the GSG.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
close_gsg() {
  _win = (GraphicsWindow *)NULL;
}

#ifdef DO_PSTATS

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::init_frame_pstats
//       Access: Protected
//  Description: Initializes the relevant PStats data at the beginning
//               of the frame.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
init_frame_pstats() {
  _current_textures.clear();
  _current_geoms.clear();
  _current_geom_nodes.clear();
  _active_texusage_pcollector.clear_level();
  _active_geom_pcollector.clear_level();
  _active_geom_node_pcollector.clear_level();

  // Also clear out our other counters while we're here.
  _vertices_tristrip_pcollector.clear_level();
  _vertices_trifan_pcollector.clear_level();
  _vertices_tri_pcollector.clear_level();
  _vertices_other_pcollector.clear_level();

  _state_changes_pcollector.clear_level();
  _transform_state_pcollector.clear_level();
  _texture_state_pcollector.clear_level();

  _nodes_pcollector.clear_level();
  _geom_nodes_pcollector.clear_level();

  // Not to mention the view-frustum-cull counters.
  _frustum_cull_volumes_pcollector.clear_level();
  _frustum_cull_transforms_pcollector.clear_level();
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::add_to_texture_record
//       Access: Protected
//  Description: Records that the indicated texture has been applied
//               this frame, and thus must be present in current
//               texture memory.  This function is only used to update
//               the PStats current_texmem collector; it gets compiled
//               out if we aren't using PStats.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
add_to_texture_record(TextureContext *tc) {
  if (PStatClient::is_connected()) {
    if (_current_textures.insert(tc).second) {
      _active_texusage_pcollector.add_level(tc->estimate_texture_memory());
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::add_to_geom_record
//       Access: Protected
//  Description: Records that the indicated Geom has been drawn this
//               frame.  This function is only used to update the
//               PStats current_texmem collector; it gets compiled out
//               if we aren't using PStats.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
add_to_geom_record(GeomContext *gc) {
  if (PStatClient::is_connected()) {
    if (gc != (GeomContext *)NULL && _current_geoms.insert(gc).second) {
      _active_geom_pcollector.add_level(1);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::add_to_geom_node_record
//       Access: Protected
//  Description: Records that the indicated GeomNode has been drawn
//               this frame.  This function is only used to update the
//               PStats current_texmem collector; it gets compiled out
//               if we aren't using PStats.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
add_to_geom_node_record(GeomNodeContext *gnc) {
  if (PStatClient::is_connected()) {
    if (gnc != (GeomNodeContext *)NULL && 
        _current_geom_nodes.insert(gnc).second) {
      _active_geom_node_pcollector.add_level(1);
    }
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::record_state_change
//       Access: Protected
//  Description: Indicates a state change request for a property of
//               the given type.
////////////////////////////////////////////////////////////////////
void GraphicsStateGuardian::
record_state_change(TypeHandle type) {
  _state_changes_pcollector.add_level(1);

  // We can't use the get_class_type() methods since we don't have
  // those header files available yet.
  string name = type.get_name();
  if (name == "TransformTransition") {
    _transform_state_pcollector.add_level(1);
  } else if (name == "TextureTransition") {
    _texture_state_pcollector.add_level(1);
  }
}
#endif  // DO_PSTATS


void GraphicsStateGuardian::
traverse_prepared_textures(bool (*pertex_callbackfn)(TextureContext *,void *),void *callback_arg) {
  for (Textures::const_iterator ti = _prepared_textures.begin(); ti != _prepared_textures.end();
       ++ti) {
    bool bResult=(*pertex_callbackfn)(*ti,callback_arg);
    if(!bResult)
      return;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: GraphicsStateGuardian::get_factory
//       Access: Public, Static
//  Description: Returns the factory object that can be used to
//               register new kinds of GraphicsStateGuardian objects that may
//               be created.
////////////////////////////////////////////////////////////////////
GraphicsStateGuardian::GsgFactory &GraphicsStateGuardian::
get_factory() {
  if (_factory == (GsgFactory *)NULL) {
    _factory = new GsgFactory;
  }
  return (*_factory);
}

void GraphicsStateGuardian::
read_priorities(void) {
  GsgFactory &factory = get_factory();
  if (factory.get_num_preferred() == 0) {
    Config::ConfigTable::Symbol::iterator i;
    for (i = preferred_gsg_begin(); i != preferred_gsg_end(); ++i) {
      ConfigString type_name = (*i).Val();
      TypeHandle type = TypeRegistry::ptr()->find_type(type_name);
      if (type == TypeHandle::none()) {
        display_cat.warning()
          << "Unknown type requested for GSG preference: " << type_name
          << "\n";
      } else {
        display_cat.debug()
          << "Specifying type " << type << " for GSG preference.\n";
        factory.add_preferred(type);
      }
    }
  }
}
