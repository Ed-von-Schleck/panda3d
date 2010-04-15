// Filename: colladaData.cxx
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

#include "colladaData.h"
#include "config_collada.h"
#include "config_util.h"
#include "config_express.h"
#include "string_utils.h"
#include "dSearchPath.h"
#include "virtualFileSystem.h"
#include "zStream.h"

TypeHandle ColladaData::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::resolve_dae_filename
//       Access: Public, Static
//  Description: Looks for the indicated filename, first along the
//               indicated searchpath, and then along the model_path.
//               If found, updates the filename to the full path and
//               returns true; otherwise, returns false.
////////////////////////////////////////////////////////////////////
bool ColladaData::
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
//     Function: ColladaData::clear
//       Access: Public
//  Description: Resets the stored data of this ColladaData,
//               and makes it as if it were a new instance.
////////////////////////////////////////////////////////////////////
void ColladaData::
clear() {
  _filename = "";
  _asset = NULL;
  _instance_visual_scene.clear();
  _library_nodes.clear();
  _library_visual_scenes.clear();
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::read
//       Access: Public
//  Description: Opens the indicated filename and reads the dae data
//               contents from it.  Returns true if the file was
//               successfully opened and read, false if there were
//               some errors, in which case the data may be partially
//               read.
//
//               error is the output stream to which to write error
//               messages.
////////////////////////////////////////////////////////////////////
bool ColladaData::
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
//     Function: ColladaData::read
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
bool ColladaData::
read(istream &in) {
  TiXmlDocument *doc = new TiXmlDocument;
  in >> *doc;
  if (in.fail() && !in.eof()) {
    delete doc;
    return false;
  }

  TiXmlElement *elem = doc->RootElement();
  bool okflag;

  if (elem == NULL) {
    collada_cat.error() << "Empty COLLADA document!\n";
    okflag = false;
  } else {
    okflag = load_xml (elem);
  }

  delete doc;
  return okflag;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::load_xml
//       Access: Public
//  Description: Parses the dae syntax contained in the indicated
//               TiXmlElement.  Returns true if the stream was a
//               completely valid dae file, false if there were some
//               errors, in which case the data may be partially read.
//
//               Before you call this routine, you should probably
//               call set_filename() to set the name of the dae
//               file we're processing, if at all possible.  If there
//               is no such filename, you may set it to the empty
//               string.
////////////////////////////////////////////////////////////////////
bool ColladaData::
load_xml(const TiXmlElement *xelement) {
  // First, dispense with any data we had previously.  We will
  // replace them with the new data.
  clear();

  nassertr (xelement != NULL, false);

  if (xelement->ValueStr() != "COLLADA") {
    collada_cat.error() << "Root element must be <COLLADA>, not <" << xelement->Value() << ">\n";
    return false;
  }

  const TiXmlElement *xchild;

  xchild = xelement->FirstChildElement("asset");
  if (xchild != NULL) {
    _asset = new ColladaAsset();
    _asset->load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_nodes");
  if (xchild != NULL) {
    _library_nodes.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("library_visual_scenes");
  if (xchild != NULL) {
    _library_visual_scenes.load_xml(xchild);
  }

  xchild = xelement->FirstChildElement("scene");
  if (xchild != NULL) {
    const TiXmlElement *xinst;
    xinst = xchild->FirstChildElement("instance_visual_scene");
    if (xinst != NULL) {
      _instance_visual_scene.load_xml(xinst);
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::make_xml
//       Access: Public
//  Description: Returns the root <COLLADA> element of the COLLADA
//               document as new TiXmlElement. This contains the
//               entire structure of the COLLADA document.
////////////////////////////////////////////////////////////////////
TiXmlElement * ColladaData::
make_xml() const {
  // Currently, we write valid 1.5.0 collada
  TiXmlElement * xelement = new TiXmlElement("COLLADA");
  xelement->SetAttribute("version", "1.5.0");
  xelement->SetAttribute("xmlns", "http://www.collada.org/2008/03/COLLADASchema");

  if (_asset) {
    xelement->LinkEndChild(_asset->make_xml());
    //FIXME: what to do when there is no asset?
  }

  if (_library_nodes.size() > 0) {
    xelement->LinkEndChild(_library_nodes.make_xml());
  }
  if (_library_visual_scenes.size() > 0) {
    xelement->LinkEndChild(_library_visual_scenes.make_xml());
  }

  TiXmlElement * xscene = new TiXmlElement("scene");
  if (!_instance_visual_scene.is_empty()) {
    xscene->LinkEndChild(_instance_visual_scene.make_xml());
  }
  xelement->LinkEndChild(xscene);

  return xelement;
}

////////////////////////////////////////////////////////////////////
//     Function: ColladaData::write_dae
//       Access: Public
//  Description: The main interface for writing complete dae files.
////////////////////////////////////////////////////////////////////
bool ColladaData::
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
//     Function: ColladaData::write_dae
//       Access: Public
//  Description: The main interface for writing complete dae files.
////////////////////////////////////////////////////////////////////
bool ColladaData::
write_dae(ostream &out) const {
  TiXmlElement * xelement = make_xml();
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

