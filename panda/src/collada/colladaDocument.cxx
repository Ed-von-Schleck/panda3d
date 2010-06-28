// Filename: colladaDocument.cxx
// Created by:  rdb (13Apr10)
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

#include "colladaDocument.h"
#include "config_collada.h"
#include "colladaCamera.h"
#include "colladaEffect.h"
#include "colladaInstance.h"
#include "colladaLight.h"
#include "colladaMaterial.h"
#include "colladaRoot.h"
#include "colladaVisualScene.h"
#include "config_util.h"
#include "config_express.h"
#include "string_utils.h"
#include "modelRoot.h"
#include "dSearchPath.h"
#include "virtualFileSystem.h"
#include "zStream.h"

TypeHandle ColladaDocument::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::Constructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaDocument::
ColladaDocument() {
  _root = new ColladaRoot(this);
  _root->ref();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::Destructor
//       Access: Public
//  Description:
////////////////////////////////////////////////////////////////////
ColladaDocument::
~ColladaDocument() {
  _root->unref();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::resolve_dae_filename
//       Access: Public, Static
//  Description: Looks for the indicated filename, first along the
//               indicated searchpath, and then along the model_path.
//               If found, updates the filename to the full path and
//               returns true; otherwise, returns false.
////////////////////////////////////////////////////////////////////
bool ColladaDocument::
resolve_dae_filename(Filename &dae_filename, const DSearchPath &searchpath) {
  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  if (dae_filename.is_fully_qualified() && vfs->exists(dae_filename)) {
    return true;
  }

  vfs->resolve_filename(dae_filename, searchpath, "dae") ||
    vfs->resolve_filename(dae_filename, get_model_path(), "dae");

  return vfs->exists(dae_filename);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::read
//       Access: Public
//  Description: Opens the indicated filename and reads the dae data
//               contents from it.  Returns true if the file was
//               successfully opened and read, false if there were
//               some errors, in which case the data may be partially
//               read.
////////////////////////////////////////////////////////////////////
bool ColladaDocument::
read(Filename filename, string display_name) {
  filename.set_text();
  set_filename(filename);

  if (display_name.empty()) {
    display_name = filename;
  }

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();

  istream *file = vfs->open_read_file(filename, true);
  if (file == (istream *)NULL) {
    collada_cat.error() << "Unable to open " << display_name << "\n";
    return false;
  }

  collada_cat.info()
    << "Reading " << display_name << "\n";

  bool read_ok = read(*file);
  vfs->close_read_file(file);
  return read_ok;
}


////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::read
//       Access: Public
//  Description: Parses the dae syntax contained in the indicated
//               input stream.  Returns true if the stream was a
//               completely valid dae file, false if there were some
//               errors, in which case the data may be partially read.
//
//               Before you call this routine, you should probably
//               call set_filename() to set the name of the dae
//               file we're processing, if at all possible.  If there
//               is no such filename, you may set it to the empty
//               string.
////////////////////////////////////////////////////////////////////
bool ColladaDocument::
read(istream &in) {
  TiXmlDocument *doc = new TiXmlDocument;
  // We stream it to a string first,
  // because TinyXML doesn't support row/column
  // tracking if we use the >> operator.
  string contents;
  while (in.good()) {
    char c = in.get();
    if (in.good()) {
      contents += c;
    }
  }
  if (!in.eof()) {
    delete doc;
    return false;
  }
  doc->Parse(contents.c_str());

  if (doc->Error()) {
    if (doc->ErrorRow()) {
      collada_cat.error()
        << "Error in " << _filename << " at line " << doc->ErrorRow()
        << ", column " << doc->ErrorCol() << ": "
        << doc->ErrorDesc() << "\n";
    } else {
      collada_cat.error()
        << "Error in " << _filename << ": "
        << doc->ErrorDesc() << "\n";
    }
    delete doc;
    return false;
  }

  TiXmlElement *elem = doc->RootElement();
  bool okflag;

  if (elem == NULL) {
    collada_cat.error() << "Empty COLLADA document!\n";
    okflag = false;
  } else {
    okflag = _root->load_xml (elem);
  }

  delete doc;
  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::write_dae
//       Access: Public
//  Description: The main interface for writing complete dae files.
////////////////////////////////////////////////////////////////////
bool ColladaDocument::
write_dae(Filename filename) const {
  filename.unlink();
  filename.set_text();

#ifdef HAVE_ZLIB
  bool pz_file = false;
  if (filename.get_extension() == "pz") {
    // The filename ends in .pz, which means to automatically compress
    // the dae file that we write.
    pz_file = true;
    filename.set_binary();
  }
#endif  // HAVE_ZLIB

  pofstream file;
  if (!filename.open_write(file)) {
    collada_cat.error() << "Unable to open " << filename << " for writing.\n";
    return false;
  }

#ifdef HAVE_ZLIB
  if (pz_file) {
    OCompressStream compressor(&file, false);
    return write_dae(compressor);
  }
#endif  // HAVE_ZLIB

  return write_dae(file);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::write_dae
//       Access: Public
//  Description: The main interface for writing complete dae files.
////////////////////////////////////////////////////////////////////
bool ColladaDocument::
write_dae(ostream &out) const {
  TiXmlElement * xelement = _root->make_xml();
  if (xelement == NULL) {
    return false;
  }
  TiXmlPrinter printer;
  printer.SetIndent("  ");
  xelement->Accept(&printer);
  out << printer.CStr();
  delete xelement;
  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::make_node
//       Access: Public
//  Description: Simply calls get_root()->make_node().
////////////////////////////////////////////////////////////////////
PT(PandaNode) ColladaDocument::
make_node() const {
  return _root->make_node();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::resolve_url
//       Access: Public
//  Description: Resolves the given URL and returns the right
//               element, or NULL if it wasn't found.
////////////////////////////////////////////////////////////////////
ColladaElement *ColladaDocument::
resolve_url(const string &url) const {
  if (url.empty()) {
    return NULL;
  }

  if (url[0] != '#') {
    collada_cat.warning() << "URL '" << url << "' is not local, cannot resolve\n";
    return NULL;
  }

  string id = url.substr(1);
  return _root->get_element_by_id(id);
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaDocument::resolve_instance
//       Access: Public
//  Description: Resolves the URL in the given instance, returns
//               the right element, or NULL if it wasn't found.
////////////////////////////////////////////////////////////////////
ColladaElement *ColladaDocument::
resolve_instance(const ColladaInstanceBase *inst) const {
  if (inst->_url.empty()) {
    return NULL;
  }

  const TypeHandle &handle = inst->get_target_type();
  PT(ColladaElement) result = resolve_url(inst->_url);
  if (!result->is_of_type(handle)) {
    collada_cat.warning()
      << "Instance URL '" << inst->_url << "' points to element of the wrong type!\n";
  }

  return result;
}

