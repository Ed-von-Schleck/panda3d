// Filename: ceguiInputHandler.cxx
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

#include "mouseAndKeyboard.h"
#include "windowProperties.h"

#include "config_cegui.h"
#include "ceguiInputHandler.h"
#include <CEGUISystem.h>

TypeHandle CeguiInputHandler::_type_handle;

CeguiInputHandler::
CeguiInputHandler(const string &name) :
  DataNode(name) {
  // Same inputs and outputs as MouseAndKeyboard.
  // This is to pass unconsumed events further down the graph.
  // Take all inputs from MouseAndKeyboard, and output all of them, filtering
  // those consumed by CEGUI.
  // HACK ALERT: EventStoreVec2::get_class_type() doesn't work.
  _pixel_xy_input = define_input("pixel_xy",
                                 TypeRegistry::ptr()->
                                 find_type("EventStoreVec2"));
  _pixel_size_input = define_input("pixel_size",
                                   TypeRegistry::ptr()->
                                   find_type("EventStoreVec2"));
  _xy_input = define_input("xy", TypeRegistry::ptr()->
                                 find_type("EventStoreVec2"));
  _button_events_input = define_input("button_events",
                                      TypeRegistry::ptr()->
                                      find_type("ButtonEventList"));
  _pointer_events_input = define_input("pointer_events",
                                       TypeRegistry::ptr()->
                                       find_type("PointerEventList"));

  _pixel_xy_output = define_output("pixel_xy",
                                   TypeRegistry::ptr()->
                                   find_type("EventStoreVec2"));
  _pixel_size_output = define_output("pixel_size",
                                   TypeRegistry::ptr()->
                                   find_type("EventStoreVec2"));
  _xy_output = define_output("xy",TypeRegistry::ptr()->
                             find_type("EventStoreVec2"));
  _button_events_output = define_output("button_events",
                                        TypeRegistry::ptr()->
                                        find_type("ButtonEventList"));
  _pointer_events_output = define_output("pointer_events",
                                         TypeRegistry::ptr()->
                                         find_type("PointerEventList"));

  init_keymap();
  _clock.reset();
}

void CeguiInputHandler::
init_keymap() {
  _keymap[KeyboardButton::left()] = CEGUI::Key::ArrowLeft;
  _keymap[KeyboardButton::right()] = CEGUI::Key::ArrowRight;
  _keymap[KeyboardButton::up()] = CEGUI::Key::ArrowUp;
  _keymap[KeyboardButton::down()] = CEGUI::Key::ArrowDown;
  _keymap[KeyboardButton::shift()] = CEGUI::Key::LeftShift;
  _keymap[KeyboardButton::control()] = CEGUI::Key::LeftControl;
  _keymap[KeyboardButton::alt()] = CEGUI::Key::LeftAlt;
  _keymap[KeyboardButton::enter()] = CEGUI::Key::Return;
  _keymap[KeyboardButton::backspace()] = CEGUI::Key::Backspace;
  _keymap[KeyboardButton::del()] = CEGUI::Key::Delete;
  _keymap[KeyboardButton::home()] = CEGUI::Key::Home;
  _keymap[KeyboardButton::end()] = CEGUI::Key::End;
  _keymap[KeyboardButton::page_up()] = CEGUI::Key::PageUp;
  _keymap[KeyboardButton::page_down()] = CEGUI::Key::PageDown;
}

void CeguiInputHandler::
do_transmit_data(DataGraphTraverser *, const DataNodeTransmit &input,
                 DataNodeTransmit &output) {
  process_mouse_pos(input, output); // _pixel_xy_input
  process_buttons(input, output);   // _button_events_input
  
  // Pass the rest of the events without filtering.
  output.set_data(_pixel_size_output, input.get_data(_pixel_size_input));
  output.set_data(_pointer_events_output,
                  input.get_data(_pointer_events_input));

  inject_time_pulse();
}

void CeguiInputHandler::
process_mouse_pos(const DataNodeTransmit &input,
                  DataNodeTransmit &output) {
  if (input.has_data(_pixel_xy_input)) {
    const EventStoreVec2* pixel_xy;
    DCAST_INTO_V(pixel_xy, input.get_data(_pixel_xy_input).get_ptr());
    const LVecBase2f &mouse_xy = pixel_xy->get_value();
    bool consumed = CEGUI::System::getSingleton().
      injectMousePosition(mouse_xy.get_x(), mouse_xy.get_y());
    if (!consumed) {
      output.set_data(_pixel_xy_output, input.get_data(_pixel_xy_input));
      output.set_data(_xy_output, input.get_data(_xy_input));
    }
  }
}

void CeguiInputHandler::
process_buttons(const DataNodeTransmit &input,
                DataNodeTransmit &output) {
  if (input.has_data(_button_events_input)) {
    const ButtonEventList* button_events_in;
    ButtonEventList* button_events_out = new ButtonEventList();

    DCAST_INTO_V(button_events_in,
                 input.get_data(_button_events_input).get_ptr());

    CEGUI::System *cegui = &CEGUI::System::getSingleton();

    int num_events = button_events_in->get_num_events();
    for (int i = 0; i < num_events; i++) {
      const ButtonEvent &be = button_events_in->get_event(i);
      ButtonHandle handle = be._button;
      // Only one of these may be filled in (?).
      CEGUI::MouseButton m_button = get_cegui_mouse_button(handle);
      CEGUI::Key::Scan key = get_cegui_keyboard_button(handle);
      bool consumed = false;
      if (be._type == ButtonEvent::T_down ||
          be._type == ButtonEvent::T_repeat ||
          be._type == ButtonEvent::T_keystroke) {
        consumed = consumed ||
          pass_button_down(m_button, key, be._keycode);
      } else if (be._type == ButtonEvent::T_up) {
        consumed = consumed ||
          pass_button_up(m_button, key, be._keycode);
      }
      consumed = consumed ||
        check_mouse_scroll(handle);
      if (!consumed) {
        button_events_out->add_event(be);
      }
    }
    
    if (button_events_out->get_num_events() != 0) {
      output.set_data(_button_events_output, EventParameter(button_events_out));
    }
  }
}

// Returns special (non-letter) keys: arrows, control keys, etc.
CEGUI::Key::Scan CeguiInputHandler::
get_cegui_keyboard_button(const ButtonHandle& handle) {
  KeyMap::iterator cegui_key = _keymap.find(handle);
  if (cegui_key != _keymap.end()) {
    return (*cegui_key).second;
  }
  return (CEGUI::Key::Scan) 0;
}

CEGUI::MouseButton CeguiInputHandler::
get_cegui_mouse_button(const ButtonHandle& handle) {
  if (handle == PMouseButton::one()) {
    return CEGUI::LeftButton;
  } else if (handle == PMouseButton::two()) {
    return CEGUI::MiddleButton;
  } else if (handle == PMouseButton::three()) {
    return CEGUI::RightButton;
  }
  return CEGUI::NoButton;
}

bool CeguiInputHandler::
pass_button_down(const CMouseButton& m_button,
                 const CEGUI::Key::Scan& key,
                 uint32 keycode) {
  bool consumed = false;
  if (m_button != CEGUI::NoButton) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectMouseButtonDown(m_button);
  }
  if (key != (CEGUI::Key::Scan) 0) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectKeyDown(key);
  }
  if (keycode != 0) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectChar(keycode);
  }
  return consumed;
}

bool CeguiInputHandler::
pass_button_up(const CMouseButton& m_button,
               const CEGUI::Key::Scan& key,
               uint32 keycode) {
  bool consumed = false;
  if (m_button != CEGUI::NoButton) {
    // Pass it through, in case mouse-up happens over the GUI.
    consumed = false;
    CEGUI::System::getSingleton().injectMouseButtonUp(m_button);
  }
  if (key != (CEGUI::Key::Scan) 0) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectKeyUp(key);
  }
  return consumed;
}

bool CeguiInputHandler::
check_mouse_scroll(const ButtonHandle& handle) {
  bool consumed = false;
  if (handle == PMouseButton::wheel_up()) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectMouseWheelChange(1.0);
  } else if (handle == PMouseButton::wheel_down()) {
    consumed = consumed ||
      CEGUI::System::getSingleton().injectMouseWheelChange(-1.0);
  }
  return consumed;
}

void CeguiInputHandler::
inject_time_pulse(void) {
  // CEGUI takes a float, get_real_time provides a double.
  float sec = _clock.get_real_time();
  _clock.reset();
  CEGUI::System::getSingleton().injectTimePulse(sec);
}

