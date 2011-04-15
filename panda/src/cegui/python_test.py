from panda3d.core import *
loadPrcFileData("", """
sync-video 0
win-origin -2 -2
win-size 1280 800
yield-timeslice 0
client-sleep 0
multi-sleep 0
basic-shaders-only #t

audio-library-name null""")

from direct.directbase import DirectStart
from direct.showbase.DirectObject import DirectObject
import gc, sys

render.setShaderAuto()
#base.toggleWireframe()
base.buttonThrowers[0].node().setModifierButtons(ModifierButtons())

"""
Good practice for the time of development
"""
gc.enable()
gc.set_debug(gc.DEBUG_LEAK)

import panda3d.cegui as cegui
#import PyCEGUI

base.setBackgroundColor(0, 0, 0)
base.disableMouse()

base.accept("escape", sys.exit)

# begin test
cegui.CeguiSupport.init(base.win,
									base.mouseWatcher.getParent(),
									render2d,
									'C:/SVN/ceGUI/cegui_mk2/v0-7/datafiles')

# Create the GUI
winMgr = cegui.WindowManager.getSingleton()
root = winMgr.createWindow("DefaultWindow", "Root")
sys = cegui.System.getSingleton()
sys.setGUISheet(root)

wnd = winMgr.loadWindowLayout('TabControlDemo.layout')
root.addChildWindow(wnd)

while True:
	# Update frame title with time since the program start.
	time = ClockObject.getGlobalClock().getRealTime()
	root.getChildAtIdx(0).getChildAtIdx(0).Text = str(time)

	taskMgr.step()
# end test

run()
