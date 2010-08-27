// Filename: cgContextManager.cxx
// Created by:  rdb (20Aug10)
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

#include "colorBlendAttrib.h"
#include "cullFaceAttrib.h"
#include "texture.h"

#include <Cg/cg.h>

CgContextManager *CgContextManager::_global_ptr = NULL;

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::Constructor
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
CgContextManager::
CgContextManager() :
  _context(cgCreateContext()),
  _record(NULL),
  _lock("CgContextManager") {

  // Needed to support loading include files through the VFS,
  // and optionally to register includes with a BamCacheRecord.
  cgSetCompilerIncludeCallback(_context, &cg_include_callback);

  // Register all states that we support.
  register_states();
  register_sampler_states();
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::Destructor
//       Access: Public, Virtual
//  Description:
////////////////////////////////////////////////////////////////////
CgContextManager::
~CgContextManager() {
  cgDestroyContext(_context);
  if (_global_ptr == this) {
    _global_ptr = NULL;
  }
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::register_states
//       Access: Private
//  Description: Registers the supported states with Cg.
////////////////////////////////////////////////////////////////////
void CgContextManager::
register_states() {
  CGstate cg_state;

  cg_state = cgCreateState(_context, "AlphaTestEnable", CG_BOOL);
  cg_state = cgCreateState(_context, "AlphaFunc", CG_FLOAT2); // sic
  cgAddStateEnumerant(cg_state, "Never", RenderAttrib::M_never);
  cgAddStateEnumerant(cg_state, "Less", RenderAttrib::M_less);
  cgAddStateEnumerant(cg_state, "Equal", RenderAttrib::M_equal);
  cgAddStateEnumerant(cg_state, "LEqual", RenderAttrib::M_less_equal);
  cgAddStateEnumerant(cg_state, "Greater", RenderAttrib::M_greater);
  cgAddStateEnumerant(cg_state, "NotEqual", RenderAttrib::M_not_equal);
  cgAddStateEnumerant(cg_state, "GEqual", RenderAttrib::M_greater_equal);
  cgAddStateEnumerant(cg_state, "Always", RenderAttrib::M_always);

  cg_state = cgCreateState(_context, "BlendEnable", CG_BOOL);
  cg_state = cgCreateState(_context, "BlendColor", CG_FLOAT4);
  cg_state = cgCreateState(_context, "BlendFunc", CG_INT2);
  cgAddStateEnumerant(cg_state, "Zero", ColorBlendAttrib::O_zero);
  cgAddStateEnumerant(cg_state, "One", ColorBlendAttrib::O_one);
  cgAddStateEnumerant(cg_state, "SrcColor", ColorBlendAttrib::O_incoming_color);
  cgAddStateEnumerant(cg_state, "OneMinusSrcColor", ColorBlendAttrib::O_one_minus_incoming_color);
  cgAddStateEnumerant(cg_state, "DstColor", ColorBlendAttrib::O_fbuffer_color);
  cgAddStateEnumerant(cg_state, "OneMinusDstColor", ColorBlendAttrib::O_one_minus_fbuffer_color);
  cgAddStateEnumerant(cg_state, "SrcAlpha", ColorBlendAttrib::O_incoming_alpha);
  cgAddStateEnumerant(cg_state, "OneMinusSrcAlpha", ColorBlendAttrib::O_one_minus_incoming_alpha);
  cgAddStateEnumerant(cg_state, "DstAlpha", ColorBlendAttrib::O_fbuffer_alpha);
  cgAddStateEnumerant(cg_state, "OneMinusDstAlpha", ColorBlendAttrib::O_one_minus_fbuffer_alpha);
  cgAddStateEnumerant(cg_state, "ConstantColor", ColorBlendAttrib::O_constant_color);
  cgAddStateEnumerant(cg_state, "OneMinusConstantColor", ColorBlendAttrib::O_one_minus_constant_color);
  cgAddStateEnumerant(cg_state, "ConstantAlpha", ColorBlendAttrib::O_constant_alpha);
  cgAddStateEnumerant(cg_state, "OneMinusConstantAlpha", ColorBlendAttrib::O_one_minus_constant_alpha);
  cgAddStateEnumerant(cg_state, "SrcAlphaSaturate", ColorBlendAttrib::O_incoming_color_saturate);
  cg_state = cgCreateState(_context, "BlendEquation", CG_INT);
  cgAddStateEnumerant(cg_state, "FuncAdd", ColorBlendAttrib::M_add);
  cgAddStateEnumerant(cg_state, "FuncSubtract", ColorBlendAttrib::M_subtract);
  cgAddStateEnumerant(cg_state, "FuncReverseSubtract", ColorBlendAttrib::M_inv_subtract);
  cgAddStateEnumerant(cg_state, "Min", ColorBlendAttrib::M_min);
  cgAddStateEnumerant(cg_state, "Max", ColorBlendAttrib::M_max);

  cg_state = cgCreateState(_context, "CullFaceEnable", CG_BOOL);
  cg_state = cgCreateState(_context, "CullFace", CG_INT);
  cgAddStateEnumerant(cg_state, "Front", CullFaceAttrib::M_cull_counter_clockwise);
  cgAddStateEnumerant(cg_state, "Back", CullFaceAttrib::M_cull_clockwise);

  cg_state = cgCreateState(_context, "ColorMask", CG_BOOL4);
  cg_state = cgCreateState(_context, "DepthMask", CG_BOOL);

  cg_state = cgCreateState(_context, "DepthTestEnable", CG_BOOL);
  cg_state = cgCreateState(_context, "DepthFunc", CG_INT);
  cgAddStateEnumerant(cg_state, "Never", RenderAttrib::M_never);
  cgAddStateEnumerant(cg_state, "Less", RenderAttrib::M_less);
  cgAddStateEnumerant(cg_state, "Equal", RenderAttrib::M_equal);
  cgAddStateEnumerant(cg_state, "LEqual", RenderAttrib::M_less_equal);
  cgAddStateEnumerant(cg_state, "Greater", RenderAttrib::M_greater);
  cgAddStateEnumerant(cg_state, "NotEqual", RenderAttrib::M_not_equal);
  cgAddStateEnumerant(cg_state, "GEqual", RenderAttrib::M_greater_equal);
  cgAddStateEnumerant(cg_state, "Always", RenderAttrib::M_always);

  cg_state = cgCreateState(_context, "MaterialAmbient", CG_FLOAT4);
  cg_state = cgCreateState(_context, "MaterialDiffuse", CG_FLOAT4);
  cg_state = cgCreateState(_context, "MaterialEmission", CG_FLOAT4);
  cg_state = cgCreateState(_context, "MaterialShininess", CG_FLOAT);
  cg_state = cgCreateState(_context, "MaterialSpecular", CG_FLOAT4);
  cg_state = cgCreateState(_context, "LightModelLocalViewerEnable", CG_BOOL);
  cg_state = cgCreateState(_context, "LightModelTwoSideEnable", CG_BOOL);

  cg_state = cgCreateState(_context, "VertexShader", CG_PROGRAM_TYPE);
  cg_state = cgCreateState(_context, "PixelShader", CG_PROGRAM_TYPE);

  cg_state = cgCreateState(_context, "VertexProgram", CG_PROGRAM_TYPE);
  cg_state = cgCreateState(_context, "GeometryProgram", CG_PROGRAM_TYPE);
  cg_state = cgCreateState(_context, "FragmentProgram", CG_PROGRAM_TYPE);
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::register_sampler_states
//       Access: Private
//  Description: Registers the supported texture properties with Cg.
////////////////////////////////////////////////////////////////////
void CgContextManager::
register_sampler_states() {
  CGstate cg_state;

  cg_state = cgCreateSamplerState(_context, "Texture", CG_TEXTURE);
  cg_state = cgCreateSamplerState(_context, "BorderColor", CG_FLOAT4);
  cg_state = cgCreateSamplerState(_context, "MinFilter", CG_INT);
  cgAddStateEnumerant(cg_state, "Nearest", Texture::FT_nearest);
  cgAddStateEnumerant(cg_state, "Linear", Texture::FT_linear);
  cgAddStateEnumerant(cg_state, "LinearMipMapNearest", Texture::FT_linear_mipmap_nearest);
  cgAddStateEnumerant(cg_state, "NearestMipMapNearest", Texture::FT_nearest_mipmap_nearest);
  cgAddStateEnumerant(cg_state, "NearestMipMapLinear", Texture::FT_nearest_mipmap_linear);
  cgAddStateEnumerant(cg_state, "LinearMipMapLinear", Texture::FT_linear_mipmap_linear);
  cg_state = cgCreateSamplerState(_context, "MagFilter", CG_INT);
  cgAddStateEnumerant(cg_state, "Nearest", Texture::FT_nearest);
  cgAddStateEnumerant(cg_state, "Linear", Texture::FT_linear);

  cg_state = cgCreateSamplerState(_context, "MaxAnisotropy", CG_FLOAT);

  cg_state = cgCreateSamplerState(_context, "WrapS", CG_INT);
  register_sampler_wrap_enumerants(cg_state);
  cg_state = cgCreateSamplerState(_context, "WrapT", CG_INT);
  register_sampler_wrap_enumerants(cg_state);
  cg_state = cgCreateSamplerState(_context, "WrapR", CG_INT);
  register_sampler_wrap_enumerants(cg_state);
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::register_sampler_states
//       Access: Private
//  Description: Registers the supported texture wrap enums
//               with the given Cg state object. This is called
//               internally by register_sampler_states().
////////////////////////////////////////////////////////////////////
void CgContextManager::
register_sampler_wrap_enumerants(CGstate cg_state) {
  cgAddStateEnumerant(cg_state, "Repeat", Texture::WM_repeat);
  cgAddStateEnumerant(cg_state, "Clamp", Texture::WM_clamp);
  cgAddStateEnumerant(cg_state, "ClampToEdge", Texture::WM_clamp);
  cgAddStateEnumerant(cg_state, "ClampToBorder", Texture::WM_border_color);
  cgAddStateEnumerant(cg_state, "MirroredRepeat", Texture::WM_mirror);
  cgAddStateEnumerant(cg_state, "MirrorClamp", Texture::WM_mirror);
  cgAddStateEnumerant(cg_state, "MirrorClampToEdge", Texture::WM_mirror);
  cgAddStateEnumerant(cg_state, "MirrorClampToBorder", Texture::WM_mirror_once);
  // Some CgFX shaders seem to specify 'Wrap' instead of 'Repeat'.
  cgAddStateEnumerant(cg_state, "Wrap", Texture::WM_repeat);
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::create_effect
//       Access: Public, Static
//  Description:
////////////////////////////////////////////////////////////////////
CGeffect CgContextManager::
create_effect(const char *source, const char **args, BamCacheRecord *record) {
  CgContextManager* p = get_global_ptr();
  LightMutexHolder(p->_lock);
  p->_record = record;
  CGeffect effect = cgCreateEffect(p->_context, source, args);
  p->_record = NULL;

  // Ugh, there doesn't seem to be a better way to check if
  // any errors occurred - it doesn't seem to fire the
  // error handler or set the error flag if it failed.
  const char* listing = cgGetLastListing(p->_context);
  if (listing != NULL && listing[0] != 0) {
    cg_cat.error(false) << listing << "\n";
    return 0;
  }
  return effect;
}

////////////////////////////////////////////////////////////////////
//     Function: CgContextManager::get_global_ptr
//       Access: Private, Static
//  Description:
////////////////////////////////////////////////////////////////////
CgContextManager *CgContextManager::
get_global_ptr() {
  if (_global_ptr == NULL) {
    _global_ptr = new CgContextManager;
  }
  return _global_ptr;
}

////////////////////////////////////////////////////////////////////
//     Function: cg_include_callback
//       Access: Private, Static
//  Description: Will be called by Cg whenever an include
//               is encountered in a Cg or CgFX file.
////////////////////////////////////////////////////////////////////
void CgContextManager::
cg_include_callback(CGcontext context, const char* filename) {
  cg_cat.debug() << "Processing include " << filename << "\n";

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  Filename fullpath = Filename::from_os_specific(filename);

  // A bit of a nuisance - Cg may give a filename that
  // starts with a slash, even if it's really a local path.
  if (filename[0] == '/') {
    if (!vfs->exists(fullpath)) {
      fullpath = Filename::from_os_specific(filename + 1);
    }
  }

  if (fullpath.is_local()) {
    fullpath = get_model_path().find_file(fullpath);
    if (fullpath.empty()) {
      // Nothing? Hmm... pass through, maybe it's
      // defined in the virtual file system of Cg.
      cg_cat.warning() << "Failed to find included file: " << filename << "\n";
      return;
    }
  }

  string body;
  if (!vfs->read_file(fullpath, body, true)) {
    cg_cat.error() << "Could not read included file: " << filename << "\n";
    return;
  }

  // Pass the source back to the Cg runtime.
  cgSetCompilerIncludeString(context, filename, body.c_str());

  BamCacheRecord *record = get_global_ptr()->_record;
  if (record != (BamCacheRecord *)NULL) {
    record->add_dependent_file(fullpath);
  }
}
