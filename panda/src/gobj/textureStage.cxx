// Filename: textureStage.cxx
// Created by:  MAsaduzz (16Jul04)
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

TypeHandle TextureStage::_type_handle;

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::Constructor
//       Access: Private
//  Description:
////////////////////////////////////////////////////////////////////
TextureStage::
TextureStage(const string &name) {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
TextureStage::
~TextureStage() {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void TextureStage::
write(ostream &out) const {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::Destructor
//       Access: Published
//  Description:
////////////////////////////////////////////////////////////////////
void TextureStage::
output(ostream &out) const {
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::register_with_read_factory
//       Access: Public, Static
//  Description: Factory method to generate a TextureStage object
////////////////////////////////////////////////////////////////////
void TextureStage::
register_with_read_factory() {
  BamReader::get_factory()->register_factory(get_class_type(), make_TextureStage);
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::make_TextureStage
//       Access: Protected
//  Description: Factory method to generate a TextureStage object
////////////////////////////////////////////////////////////////////
TypedWritable* TextureStage::
make_TextureStage(const FactoryParams &params) {
  return NULL;
#if 0
  //The process of making a texture is slightly
  //different than making other Writable objects.
  //That is because all creation of TextureStages should
  //be done through calls to TexturePool, which ensures
  //that any loads of the same Texture, refer to the
  //same memory
  DatagramIterator scan;
  BamReader *manager;
  bool has_rawdata = false;

  parse_params(params, scan, manager);

  // Get the properties written by ImageBuffer::write_datagram().
  string name = scan.get_string();
  Filename filename = scan.get_string();
  Filename alpha_filename = scan.get_string();

  int primary_file_num_channels = 0;  
  int alpha_file_channel = 0;  

  if (manager->get_file_minor_ver() == 2) {
    // We temporarily had a version that stored the number of channels
    // here.
    primary_file_num_channels = scan.get_uint8();

  } else if (manager->get_file_minor_ver() >= 3) {
    primary_file_num_channels = scan.get_uint8();
    alpha_file_channel = scan.get_uint8();
  }

  // from minor version 5, read the rawdata mode, else carry on
  if (manager->get_file_minor_ver() >= 5)
    has_rawdata = scan.get_bool();

  Texture *me = NULL;
  if (has_rawdata) {
    // then create a Texture and don't load from the file
    me = new Texture;

  } else {
    if (filename.empty()) {
      // This texture has no filename; since we don't have an image to
      // load, we can't actually create the texture.
      gobj_cat.info()
        << "Cannot create texture '" << name << "' with no filename.\n";

    } else {
      // This texture does have a filename, so try to load it from disk.
      if (alpha_filename.empty()) {
        me = TexturePool::load_texture(filename, primary_file_num_channels);
      } else {
        me = TexturePool::load_texture(filename, alpha_filename, 
                                       primary_file_num_channels, alpha_file_channel);
      }
    }
  }

  if (me == (Texture *)NULL) {
    // Oops, we couldn't load the texture; we'll just return NULL.
    // But we do need a dummy texture to read in and ignore all of the
    // attributes.
    PT(Texture) dummy = new Texture;
    dummy->fillin(scan, manager, has_rawdata);

  } else {
    me->set_name(name);
    me->fillin(scan, manager, has_rawdata);

    /*
    cerr << "_xsize = " << me->_pbuffer->get_xsize() << "\n";
    cerr << "_ysize = " << me->_pbuffer->get_ysize() << "\n";
    cerr << "_xorg = " << me->_pbuffer->get_xorg() << "\n";
    cerr << "_yorg = " << me->_pbuffer->get_xorg() << "\n";
    cerr << "_components = " << me->_pbuffer->get_num_components() << "\n";
    */
  }
  return me;
#endif
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::fillin
//       Access: Protected
//  Description: Function that reads out of the datagram (or asks
//               manager to read) all of the data that is needed to
//               re-create this object and stores it in the appropiate
//               place
////////////////////////////////////////////////////////////////////
void TextureStage::
fillin(DatagramIterator &scan, BamReader *manager, bool has_rawdata) {
#if 0
  //We don't want to call ImageBuffer::fillin, like we
  //would normally, since due to needing to know the name
  //of the TextureStage before creating it, we have already read
  //that name in.  This is something of a problem as it forces
  //Texture to know how the parent write_datagram works.  And
  //makes the assumption that the only data being written is
  //the name

  _wrapu = (enum WrapMode) scan.get_uint8();
  _wrapv = (enum WrapMode) scan.get_uint8();
  _minfilter = (enum FilterType) scan.get_uint8();
  _magfilter = (enum FilterType) scan.get_uint8();
  _anisotropic_degree = scan.get_int16();

  bool has_pbuffer = scan.get_bool();
  if (has_pbuffer) {
    PixelBuffer::Format format = (PixelBuffer::Format)scan.get_uint8();
    int num_channels = -1;
    num_channels = scan.get_uint8();

    if (_pbuffer != (PixelBuffer *)NULL) {
      if (num_channels == _pbuffer->get_num_components()) {
        // Only reset the format if the number of components hasn't
        // changed, since if the number of components has changed our
        // texture no longer matches what it was when the bam was
        // written.
        _pbuffer->set_format(format);
      }

      if (has_rawdata) {
        // In the rawdata case, we must always set the format.
        _pbuffer->set_format(format);
        _pbuffer->set_xsize(scan.get_int32());
        _pbuffer->set_ysize(scan.get_int32());
        _pbuffer->set_xorg(scan.get_int32());
        _pbuffer->set_yorg(scan.get_int32());
        _pbuffer->set_border(scan.get_uint8());
        _pbuffer->set_image_type((PixelBuffer::Type)scan.get_uint8());
        _pbuffer->set_num_components(scan.get_uint8());
        _pbuffer->set_component_width(scan.get_uint8());

        _pbuffer->set_loaded();
        PN_uint32 u_size = scan.get_uint32();

        // fill the _image buffer with image data
        string temp_buff = scan.extract_bytes(u_size);
        _pbuffer->_image = PTA_uchar::empty_array((int) u_size);
        for (PN_uint32 u_idx=0; u_idx < u_size; ++u_idx) {
          _pbuffer->_image[(int)u_idx] = (uchar) temp_buff[u_idx];
        }
      }
    }
  }
#endif
}

////////////////////////////////////////////////////////////////////
//     Function: TextureStage::write_datagram
//       Access: Public
//  Description: Function to write the important information in
//               the particular object to a Datagram
////////////////////////////////////////////////////////////////////
void TextureStage::
write_datagram(BamWriter *manager, Datagram &me) {
#if 0
  // We also need to write out the pixel buffer's format, even though
  // that's not stored as part of the texture structure.
  bool has_pbuffer = (_pbuffer != (PixelBuffer *)NULL);
  bool has_rawdata = (bam_texture_mode == BTM_rawdata);

  // These properties are read in again by make_TextureStage(), above.
  ImageBuffer::write_datagram(manager, me);

  // from minor version 5, you add this byte to support rawdata mode
  me.add_bool(has_rawdata);

  // These properties are read in again by fillin(), above.
  me.add_uint8(_wrapu);
  me.add_uint8(_wrapv);
  me.add_uint8(_minfilter);
  me.add_uint8(_magfilter);
  me.add_int16(_anisotropic_degree);

  me.add_bool(has_pbuffer);
  if (has_pbuffer) {
    me.add_uint8(_pbuffer->get_format());
    me.add_uint8(_pbuffer->get_num_components());
  }

  // if it has rawdata, then stuff them here along with the header information
  if (has_rawdata) {
    me.add_int32(_pbuffer->get_xsize());
    me.add_int32(_pbuffer->get_ysize());
    me.add_int32(_pbuffer->get_xorg());
    me.add_int32(_pbuffer->get_yorg());
    me.add_uint8(_pbuffer->get_border());
    me.add_uint8(_pbuffer->get_image_type());
    me.add_uint8(_pbuffer->get_num_components());
    me.add_uint8(_pbuffer->get_component_width());

    me.add_uint32(_pbuffer->_image.size());
    me.append_data(_pbuffer->_image, _pbuffer->_image.size());
  }
#endif
}

