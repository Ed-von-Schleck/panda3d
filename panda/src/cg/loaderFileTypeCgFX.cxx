// Filename: loaderFileTypeCgFX.cxx
// Created by:  rdb (27Aug10)
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

#include "loaderFileTypeCgFX.h"
#include "config_cg.h"
#include "bamFile.h"
#include "bamCacheRecord.h"
#include "loaderOptions.h"
#include "dcast.h"

#include "alphaTestAttrib.h"
#include "colorWriteAttrib.h"
#include "depthTestAttrib.h"
#include "depthWriteAttrib.h"
#include "materialAttrib.h"

TypeHandle LoaderFileTypeCgFX::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
LoaderFileTypeCgFX::
LoaderFileTypeCgFX() {
}

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::get_name
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
string LoaderFileTypeCgFX::
get_name() const {
  return "CgFX";
}

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::get_extension
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
string LoaderFileTypeCgFX::
get_extension() const {
  return "cgfx";
}

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::supports_compressed
//       Access: Published, Virtual
//  Description: Returns true if this file type can transparently load
//               compressed files (with a .pz extension), false
//               otherwise.
////////////////////////////////////////////////////////////////////
bool LoaderFileTypeCgFX::
supports_compressed() const {
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::load_file
//       Access: Public, Virtual
//  Description: Loads a CgFX effect and returns an Effect object
//               that most closely represents the CgFX effect.
//               Returns NULL on failure.
////////////////////////////////////////////////////////////////////
PT(PandaNode) LoaderFileTypeCgFX::
load_file(const Filename &path, const LoaderOptions &options,
          BamCacheRecord *record) const {
  if (record != (BamCacheRecord *)NULL) {
    record->add_dependent_file(path);
  }

  bool report_errors = (options.get_flags() & LoaderOptions::LF_report_errors) != 0;

  string body;
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  if (!vfs->read_file(path, body, true)) {
    cg_cat.error() << "Could not read CgFX file: " << path << "\n";
    return NULL;
  }

  CGeffect cg_effect =
    CgContextManager::create_effect(body.c_str(), NULL, record);

  if (cg_effect == NULL || !cgIsEffect(cg_effect)) {
    return NULL;
  }

  PT(Effect) effect = new Effect(path.get_basename());

  CGtechnique cg_technique = cgGetFirstTechnique(cg_effect);
  while (cg_technique) {
    PT(Technique) technique = new Technique();
    effect->set_technique(InternalName::make(cgGetTechniqueName(cg_technique)), technique);

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

      PT(Material) mat = load_material(cg_pass);
      if (mat) {
        state = state->add_attrib(MaterialAttrib::make(mat));
      }

      pass->set_state(state);

      cg_pass = cgGetNextPass(cg_pass);
    }

    cg_technique = cgGetNextTechnique(cg_technique);
  }

  cgDestroyEffect(cg_effect);

  return DCAST(Effect, effect);
}

////////////////////////////////////////////////////////////////////
//     Function: LoaderFileTypeCgFX::load_material
//       Access: Private
//  Description: Loads material settings from a CGpass object.
//               Returns a new Material object, or NULL if no
//               material settings were specified on the pass.
//
//               This method tries hard to do the exact
//               inverse of glGSG::do_issue_material.
////////////////////////////////////////////////////////////////////
PT(Material) LoaderFileTypeCgFX::
load_material(const CGpass cg_pass) const {
  CGstateassignment cg_sa;
  int count;

  PT(Material) mat = new Material;
  bool mat_modified = false;

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "MaterialAmbient")) {
    const float* values = cgGetFloatStateAssignmentValues(cg_sa, &count);
    Colorf color;
    memcpy(color._v.data, values, sizeof(float) * count);
    mat->set_ambient(color);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "MaterialDiffuse")) {
    const float* values = cgGetFloatStateAssignmentValues(cg_sa, &count);
    Colorf color;
    memcpy(color._v.data, values, sizeof(float) * count);
    mat->set_diffuse(color);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "MaterialEmission")) {
    const float* values = cgGetFloatStateAssignmentValues(cg_sa, &count);
    Colorf color;
    memcpy(color._v.data, values, sizeof(float) * count);
    mat->set_emission(color);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "MaterialShininess")) {
    const float value = *cgGetFloatStateAssignmentValues(cg_sa, &count);
    mat->set_shininess(value);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "MaterialSpecular")) {
    const float* values = cgGetFloatStateAssignmentValues(cg_sa, &count);
    Colorf color;
    memcpy(color._v.data, values, sizeof(float) * count);
    mat->set_specular(color);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "LightModelLocalViewerEnable")) {
    const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
    mat->set_local(value);
    mat_modified = true;
  }

  if (cg_sa = cgGetNamedStateAssignment(cg_pass, "LightModelTwoSideEnable")) {
    const CGbool value = *cgGetBoolStateAssignmentValues(cg_sa, &count);
    mat->set_twoside(value);
    mat_modified = true;
  }

  // Make sure that we don't return a material if no material
  // properties were actually specified in the CG pass.
  return mat_modified ? mat : NULL;
}

