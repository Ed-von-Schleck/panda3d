// Filename: guiButton.cxx
// Created by:  cary (30Oct00)
// 
////////////////////////////////////////////////////////////////////

#include "guiButton.h"
#include "config_gui.h"

#include <throw_event.h>

#include <map>

typedef map<string, GuiButton*> ButtonMap;
static ButtonMap buttons;

inline void GetExtents(GuiLabel* v, GuiLabel* w, GuiLabel* x, GuiLabel* y,
		       GuiLabel* z, float& l, float& r, float& b, float& t) {
  float l1, l2, r1, r2, b1, b2, t1, t2;
  v->get_extents(l1, r1, b1, t1);
  w->get_extents(l2, r2, b2, t2);
  l1 = (l1<l2)?l1:l2;
  r1 = (r1<r2)?r2:r1;
  b1 = (b1<b2)?b1:b2;
  t1 = (t1<t2)?t2:t1;
  if (x != (GuiLabel*)0L) {
    x->get_extents(l2, r2, b2, t2);
    l1 = (l1<l2)?l1:l2;
    r1 = (r1<r2)?r2:r1;
    b1 = (b1<b2)?b1:b2;
    t1 = (t1<t2)?t2:t1;
  }
  if (y != (GuiLabel*)0L) {
    y->get_extents(l2, r2, b2, t2);
    l1 = (l1<l2)?l1:l2;
    r1 = (r1<r2)?r2:r1;
    b1 = (b1<b2)?b1:b2;
    t1 = (t1<t2)?t2:t1;
  }
  if (z != (GuiLabel*)0L) {
    z->get_extents(l2, r2, b2, t2);
    l = (l1<l2)?l1:l2;
    r = (r1<r2)?r2:r1;
    b = (b1<b2)?b1:b2;
    t = (t1<t2)?t2:t1;
  }
}

static void enter_button(CPT_Event e) {
  GuiButton* val = buttons[e->get_name()];
  val->enter();
}

static void exit_button(CPT_Event e) {
  GuiButton* val = buttons[e->get_name()];
  val->exit();
}

static void click_button(CPT_Event e) {
  GuiButton* val = buttons[e->get_name()];
  val->click();
}

void GuiButton::switch_state(GuiButton::States nstate) {
  // cleanup old state
  switch (_state) {
  case NONE:
    break;
  case UP:
    _mgr->remove_label(_up);
    break;
  case UP_ROLLOVER:
    _mgr->remove_label(_up_rollover);
    break;
  case DOWN:
    _mgr->remove_label(_down);
    break;
  case DOWN_ROLLOVER:
    _mgr->remove_label(_down_rollover);
    break;
  case INACTIVE:
    if (_inactive != (GuiLabel*)0L)
      _mgr->remove_label(_inactive);
    break;
  case INACTIVE_ROLLOVER:
    if (_inactive != (GuiLabel*)0L)
      _mgr->remove_label(_inactive);
    break;
  default:
    gui_cat->warning() << "switching away from invalid state (" << _state
		       << ")" << endl;
  }
  _state = nstate;
  // deal with new state
  switch (_state) {
  case NONE:
    _rgn->trap_clicks(false);
    break;
  case UP:
    _mgr->add_label(_up);
    if (!_up_event.empty())
      throw_event(_up_event);
    _rgn->trap_clicks(true);
    break;
  case UP_ROLLOVER:
    if (_up_rollover != (GuiLabel*)0L) {
      _mgr->add_label(_up_rollover);
      if (!_up_rollover_event.empty())
	throw_event(_up_rollover_event);
    } else {
      _mgr->add_label(_up);
      if (!_up_event.empty())
	throw_event(_up_event);
      _state = UP;
    }
    _rgn->trap_clicks(true);
    break;
  case DOWN:
    _mgr->add_label(_down);
    if (!_down_event.empty())
      throw_event(_down_event);
    _rgn->trap_clicks(true);
    break;
  case DOWN_ROLLOVER:
    if (_down_rollover != (GuiLabel*)0L) {
      _mgr->add_label(_down_rollover);
      if (!_down_rollover_event.empty())
	throw_event(_down_rollover_event);
    } else {
      _mgr->add_label(_down);
      if (!_down_event.empty())
	throw_event(_down_event);
      _state = DOWN;
    }
    _rgn->trap_clicks(true);
    break;
  case INACTIVE:
    if (_inactive != (GuiLabel*)0L) {
      _mgr->add_label(_inactive);
      if (!_inactive_event.empty())
	throw_event(_inactive_event);
    }
    _rgn->trap_clicks(false);
    break;
  case INACTIVE_ROLLOVER:
    if (_inactive != (GuiLabel*)0L) {
      _mgr->add_label(_inactive);
      if (!_inactive_event.empty())
	throw_event(_inactive_event);
    }
    _rgn->trap_clicks(false);
    break;
  default:
    gui_cat->warning() << "switched to invalid state (" << _state << ")"
		       << endl;
  }
}

GuiButton::GuiButton(const string& name, GuiLabel* up, GuiLabel* up_roll,
		     GuiLabel* down, GuiLabel* down_roll, GuiLabel* inactive)
  : Namable(name), _up(up), _up_rollover(up_roll), _down(down),
    _down_rollover(down_roll), _inactive(inactive), _state(GuiButton::NONE),
    _added_hooks(false), _mgr((GuiManager*)0L), _up_event(name + "-up"),
    _up_rollover_event(name + "-up-rollover"), _down_event(name +"-down"),
    _down_rollover_event(name + "-down-rollover"),
    _inactive_event(name + "-inactive") {
  float left, right, bottom, top;

  GetExtents(up, down, up_roll, down_roll, inactive, left, right, bottom, top);
  _rgn = new GuiRegion("button-" + name, left, right, bottom, top, true);
  buttons["gui-in-button-" + name] = this;
  buttons["gui-out-button-" + name] = this;
  buttons["gui-button-" + name + "-mouse1"] = this;
  buttons["gui-button-" + name + "-mouse2"] = this;
  buttons["gui-button-" + name + "-mouse3"] = this;
}

GuiButton::~GuiButton(void) {
}

void GuiButton::manage(GuiManager* mgr, EventHandler& eh) {
  if (!_added_hooks) {
    eh.add_hook("gui-in-button-" + get_name(), enter_button);
    eh.add_hook("gui-out-button-" + get_name(), exit_button);
    eh.add_hook("gui-button-" + get_name() + "-mouse1", click_button);
    eh.add_hook("gui-button-" + get_name() + "-mouse2", click_button);
    eh.add_hook("gui-button-" + get_name() + "-mouse3", click_button);
    _added_hooks = true;
  }
  if (_mgr == (GuiManager*)0L) {
    mgr->add_region(_rgn);
    _mgr = mgr;
    switch_state(UP);
  } else
    gui_cat->warning() << "tried to manage button (0x" << (void*)this
		       << ") that is already managed" << endl;
}

void GuiButton::unmanage(void) {
  _mgr->remove_region(_rgn);
  switch_state(NONE);
  _mgr = (GuiManager*)0L;
}
