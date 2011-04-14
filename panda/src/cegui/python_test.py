# This is a minimal CEGUI Python demo.

# Panda3d imports
from pandac.PandaModules import *
import direct.directbase.DirectStart

import PyCEGUI

# Initialize CEGUI
CeguiSupport.init(base.win,
                  base.mouseWatcher.getParent(),
                  render2d,
                  '/home/nik/Code/panda3d/project/datafiles')

# Create the GUI
winMgr = PyCEGUI.WindowManager.getSingleton()
root = winMgr.createWindow("DefaultWindow", "Root")
sys = PyCEGUI.System.getSingleton()
sys.setGUISheet(root)

wnd = winMgr.loadWindowLayout('LoremIpsum.layout')
root.addChildWindow(wnd)

while True:
  # Update frame title with time since the program start.
  time = ClockObject.getGlobalClock().getRealTime()
  root.getChildAtIdx(0).getChildAtIdx(0).Text = str(time)

  taskMgr.step()
