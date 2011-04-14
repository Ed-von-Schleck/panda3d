// Filename: ceguiInputHandler.h
// Created by:  Nik (Oct10)
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

#ifndef CEGUIINPUTHANDLER_H
#define CEGUIINPUTHANDLER_H

#include "dataNodeTransmit.h"
#include "keyboardButton.h"
#include "mouseButton.h"
#include "dataNode.h"
#include "dataGraphTraverser.h"

#include "CEGUIInputEvent.h"

// Avoid collision with Panda's MouseButton
typedef ::MouseButton PMouseButton;

typedef enum CEGUI::MouseButton CMouseButton;

typedef struct Key CKey;
typedef CEGUI::Key::Scan CScan;

class CeguiInputHandler : public DataNode {
  // A DataNode that takes all inputs and translates them into Cegui events.
  // Should be parented somewhere under MouseAndKeyboard, probably.
public:
  CeguiInputHandler(const string &name);
protected:
  virtual void do_transmit_data(DataGraphTraverser *,
                                const DataNodeTransmit &input,
                                DataNodeTransmit &output);

private:
  void init_keymap();
  void process_mouse_pos(const DataNodeTransmit &input,
                         DataNodeTransmit &output);
  void process_buttons(const DataNodeTransmit &input,
                       DataNodeTransmit &output);
  typedef unsigned long int uint32;
  bool pass_button_down(const CMouseButton& m_button,
                        const CScan& key,
                        uint32 keycode);
  bool pass_button_up(const CMouseButton& m_button,
                      const CScan& key,
                      uint32 keycode);
  CScan get_cegui_keyboard_button(const ButtonHandle& handle);
  CMouseButton get_cegui_mouse_button(const ButtonHandle& handle);
  bool check_mouse_scroll(const ButtonHandle& handle);
  void inject_time_pulse(void);

  typedef pmap<ButtonHandle, CScan> KeyMap;
  KeyMap _keymap;

  // Inputs and outputs from MouseAndKeyboard
  int _pixel_xy_input;
  int _button_events_input;
  int _pixel_size_input;
  int _xy_input;
  int _pointer_events_input;

  int _pixel_xy_output;
  int _button_events_output;
  int _pixel_size_output;
  int _xy_output;
  int _pointer_events_output;

  ClockObject _clock;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    DataNode::init_type();
    register_type(_type_handle, "CeguiInputHandler",
                  DataNode::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}

private:
  static TypeHandle _type_handle;
};

#endif
