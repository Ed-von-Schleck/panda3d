// Filename: cgEffect.cxx
// Created by: Xidram (15Aug10)
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

#include "cgContextManager.h"
#include "cgEffect.h"
#include "pointerTo.h"
#include "renderPass.h"
#include "technique.h"
#include "virtualFileSystem.h"

#include "alphaTestAttrib.h"
#include "colorBlendAttrib.h"
#include "colorWriteAttrib.h"
#include "cullFaceAttrib.h"
#include "depthTestAttrib.h"
#include "depthWriteAttrib.h"

#include <Cg/cg.h>

TypeHandle CgEffect::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: CgEffect::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
CgEffect::
CgEffect(const string &name) :
  Effect(name)
{
}

////////////////////////////////////////////////////////////////////
//     Function: CgEffect::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CgEffect::
~CgEffect() {
}

////////////////////////////////////////////////////////////////////
//     Function: CgEffect::read
//       Access: Published, Virtual
//  Description: Reads a CgFX effect from disk.
////////////////////////////////////////////////////////////////////
bool CgEffect::
read(const Filename &fullpath, BamCacheRecord *record) {
  string body;
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vfs->read_file(fullpath, body, true)) {
    cg_cat.error() << "Could not read CgFX file: " << fullpath << "\n";
    return false;
  }

  if (record != (BamCacheRecord *)NULL) {
    record->add_dependent_file(fullpath);
  }

  CGeffect cg_effect =
    CgContextManager::create_effect(body.c_str(), NULL, record);

  if (cg_effect == NULL || !cgIsEffect(cg_effect)) {
    return false;
  }

  CGtechnique cg_technique = cgGetFirstTechnique(cg_effect);
  while (cg_technique) {
    PT(Technique) technique = new Technique();
    set_technique(InternalName::make(cgGetTechniqueName(cg_technique)), technique);

    CGpass cg_pass = cgGetFirstPass(cg_technique);
    while (cg_pass) {
      PT(RenderPass) pass = new RenderPass(cgGetPassName(cg_pass));
      technique->add_pass(pass);

      CGstateassignment cg_sa;
      CPT(RenderState) state = RenderState::make_empty();
      int count;

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "AlphaTestEnable")) {
        const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
        if (value) {
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "AlphaFunc")) {
            float alpha_ref = 1.0f;
            const float* floats = cgGetFloatStateAssignmentValues(cg_sa, &count);
            RenderAttrib::PandaCompareFunc func =
              (RenderAttrib::PandaCompareFunc) (int) floats[0];
            if (count >= 2) {
              alpha_ref = floats[1];
            }
            state = state->add_attrib(AlphaTestAttrib::make(func, alpha_ref));
          } else {
            state = state->add_attrib(AlphaTestAttrib::make_default());
          }
        } else {
          state = state->add_attrib(AlphaTestAttrib::make(RenderAttrib::M_none, 1.0f));
        }
      }

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "BlendEnable")) {
        const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
        if (value) {
          ColorBlendAttrib::Mode eq = ColorBlendAttrib::M_none;
          ColorBlendAttrib::Operand opA = ColorBlendAttrib::O_one;
          ColorBlendAttrib::Operand opB = ColorBlendAttrib::O_zero;
          Colorf color = Colorf::zero();
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "BlendEquation")) {
            eq = (ColorBlendAttrib::Mode) *cgGetIntStateAssignmentValues(cg_sa, &count);
          }
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "BlendFunc")) {
            const int* funcs = cgGetIntStateAssignmentValues(cg_sa, &count);
            opA = (ColorBlendAttrib::Operand) funcs[0];
            opB = (ColorBlendAttrib::Operand) funcs[1];
          }
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "BlendColor")) {
            const float* col = cgGetFloatStateAssignmentValues(cg_sa, &count);
            memcpy(color._v.data, col, sizeof(float) * count);
          }
          state = state->add_attrib(ColorBlendAttrib::make(eq, opA, opB, color));
        } else {
          state = state->add_attrib(ColorBlendAttrib::make_off());
        }
      }

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "CullFaceEnable")) {
        const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
        if (value) {
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "CullFace")) {
            CullFaceAttrib::Mode mode = (CullFaceAttrib::Mode)
              *cgGetIntStateAssignmentValues(cg_sa, &count);
            state = state->add_attrib(CullFaceAttrib::make(mode));
          } else {
            state = state->add_attrib(CullFaceAttrib::make_default());
          }
        } else {
          state = state->add_attrib(CullFaceAttrib::make(CullFaceAttrib::M_cull_none));
        }
      }

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "ColorMask")) {
        const CGbool* values = cgGetBoolStateAssignmentValues(cg_sa, &count);
        int mask = 0;
        if (values[0]) mask |= ColorWriteAttrib::C_red;
        if (values[1]) mask |= ColorWriteAttrib::C_green;
        if (values[2]) mask |= ColorWriteAttrib::C_blue;
        if (values[3]) mask |= ColorWriteAttrib::C_alpha;
        state = state->add_attrib(ColorWriteAttrib::make(mask));
      }

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "DepthMask")) {
        const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
        if (value) {
          state = state->add_attrib(DepthWriteAttrib::make(DepthWriteAttrib::M_on));
        } else {
          state = state->add_attrib(DepthWriteAttrib::make(DepthWriteAttrib::M_off));
        }
      }

      if (cg_sa = cgGetNamedStateAssignment(cg_pass, "DepthTestEnable")) {
        const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
        if (value) {
          if (cg_sa = cgGetNamedStateAssignment(cg_pass, "DepthFunc")) {
            RenderAttrib::PandaCompareFunc func = (RenderAttrib::PandaCompareFunc)
              *cgGetIntStateAssignmentValues(cg_sa, &count);
            state = state->add_attrib(DepthTestAttrib::make(func));
          } else {
            state = state->add_attrib(DepthTestAttrib::make_default());
          }
        } else {
          state = state->add_attrib(DepthTestAttrib::make(RenderAttrib::M_none));
        }
      }

      pass->set_state(state);

      cg_pass = cgGetNextPass(cg_pass);
    }

    cg_technique = cgGetNextTechnique(cg_technique);
  }

  cgDestroyEffect(cg_effect);

  return true;
}

