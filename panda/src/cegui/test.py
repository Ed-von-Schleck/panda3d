#
#
# coding=utf8
#
# This is a demo of CEGUI in Python.
#
# This is a liberal translation of Sample_FontDemo.cpp to Python
# using CEGUIPython.

import sys, pdb

from pandac.PandaModules import *

import direct.directbase.DirectStart

# CEGUIPython imports

import ogre.gui.CEGUI as cegui
from ogre.gui.CEGUI import *
#from cegui import *

class Handler(object):
    def handleFontSelection(self, eArgs):
        winMgr = WindowManager.getSingleton()
        lbox = eArgs.window
        item = lbox.getFirstSelectedItem()
        if item:
            fontName = item.Text
            font = FontManager.getSingleton().getFont(fontName)
            if font.isPropertyPresent("AutoScaled"):
                cb = winMgr.getWindow("FontDemo/AutoScaled")
                cb.Selected = PropertyHelper.stringToBool(
                    font.getProperty("AutoScaled"))

            if font.isPropertyPresent("Antialiased"):
                cb = winMgr.getWindow("FontDemo/Antialiased")
                cb.Selected = PropertyHelper.stringToBool(
                    font.getProperty("Antialiased"))

            winMgr.getWindow("FontDemo/FontSample").setFont(font)


# Global handler to make sure it doesn't get destroyed.
handler = Handler()


def main():
    init()
    while True:
        taskMgr.step()
        #render2d.getChild(2).writeBamFile('r2d.bam')

class MyListItem(ListboxTextItem):
    items = []                          # Dummy list to prevent item desctruction.
    def __init__(self, text):
        ListboxTextItem.__init__(self, text)
        self.setSelectionBrushImage("TaharezLook", "MultiListSelectionBrush")
        MyListItem.items.append(self)

languages = {'English' : 'English text here',
             'German' : 'German text here',
             'Your favourite language' : 'Text in your favourite language.'
             }

def init():
    CeguiSupport.init(base.win,
                      base.mouseWatcher.getParent(),
                      render2d,
                      "/home/nik/Code/src/CEGUI-0.7.1/datafiles/")
#     cegui.Logger.getSingleton().loggingLevel = cegui.Insane

    winMgr = WindowManager.getSingleton()
    ImagesetManager.getSingleton().createFromImageFile("BackgroundImage",
                                                       "GPN-2000-001437.tga")

    background = winMgr.createWindow ("TaharezLook/StaticImage")
    # disable frame and standard background
    background.setProperty ("FrameEnabled", "false")
    background.setProperty ("BackgroundEnabled", "false")
    # set the background image
    background.setProperty ("Image", "set:BackgroundImage image:full_image")
    # install this as the root GUI sheet
    System.getSingleton().setGUISheet(background)

    # Create GUI
    background.addChildWindow(winMgr.loadWindowLayout('FontDemo.layout'))
    return
    # Fonts selection
    lbox = winMgr.getWindow ("FontDemo/FontList")
    fontMgr = FontManager.getSingleton()
    fonts = ["DejaVuSans",
             # "Commonwealth-10",
             # "Iconified-12",
             # "fkp-16",
             # "FairChar-30"
             ]

    for font in fonts:
        if fontMgr.isFontPresent(font):
            item = MyListItem(font)
            lbox.addItem(item)

    # Event subscription: pass in an object and a method name.
    lbox.subscribeEvent(cegui.Listbox.EventSelectionChanged,
                        handler, 'handleFontSelection')


    # Language selection - can't use Unicode for some reason.
    # Is my CEGUIPython messed up?
    lbox = winMgr.getWindow("FontDemo/LangList");
    global languages
    for lang in languages.keys():
        lbox.addItem(MyListItem(lang))

    lbox.subscribeEvent(Listbox.EventSelectionChanged,
                        handleLangSelection, '')

    # Alternative event subscription: pass in a function.
    winMgr.getWindow("FontDemo/AutoScaled").subscribeEvent(
        Checkbox.EventCheckStateChanged, handleAutoScaled, '')
    winMgr.getWindow("FontDemo/Antialiased").subscribeEvent(
        Checkbox.EventCheckStateChanged, handleAntialiased, '')
    winMgr.getWindow("FontDemo/PointSize").subscribeEvent(
        Scrollbar.EventScrollPositionChanged, handlePointSize, '')

    
def handleLangSelection(eArgs):
    lbox = eArgs.window
    item = lbox.getFirstSelectedItem()
    if item:
        lang = str(item.Text)
        winMgr = WindowManager.getSingleton()
        global languages
        winMgr.getWindow("FontDemo/FontSample").Text = languages[lang]

def handlePointSize(eArgs):
    MIN_POINT_SIZE = 6.0
    scrollbar = eArgs.window
    winMgr = WindowManager.getSingleton()
    font = winMgr.getWindow("FontDemo/FontSample").getFont()
    font.setProperty("PointSize", PropertyHelper.intToString(
        int(MIN_POINT_SIZE + scrollbar.ScrollPosition)))

def handleAutoScaled(eArgs):
    winMgr = WindowManager.getSingleton()
    checkbox = eArgs.window
    textBox = winMgr.getWindow("FontDemo/FontSample")
    font = textBox.getFont()
    font.setProperty("AutoScaled",
                     PropertyHelper.boolToString(checkbox.Selected))

def handleAntialiased(eArgs):
    winMgr = WindowManager.getSingleton()
    checkbox = eArgs.window
    textBox = winMgr.getWindow("FontDemo/FontSample")
    font = textBox.getFont()
    font.setProperty("Antialiased",
                     PropertyHelper.boolToString(checkbox.Selected))

if __name__ == '__main__':
  # Pass 'd' in command line to get the debugger: python main.py d
  if 'd' in sys.argv:
      pdb.run('main()')
  else:
      main()
