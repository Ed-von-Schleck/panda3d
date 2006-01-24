#
# Normal mapping is a way of making polygonal surfaces look
# less flat.  Another word for normal mapping is "bump mapping."
#
# This is a tutorial to show how to do normal mapping
# in panda3d using Cg.  This is probably the simplest possible
# version of normal mapping: just one point light, one ambient
# light, and no shadows.

import direct.directbase.DirectStart
from direct.task import Task
from direct.actor import Actor
from direct.showbase import DirectObject
from direct.gui.DirectGui import *
from pandac.PandaModules import *
import sys,os
from direct.showbase.DirectObject import DirectObject
import pdb


# Figure out what directory this program is in.
MYDIR=os.path.abspath(sys.path[0])
MYDIR=Filename.fromOsSpecific(MYDIR).getFullpath()

# Function to put instructions on the screen.
def addInstructions(pos, msg):
    return OnscreenText(text=msg, style=1, fg=(1,1,1,1), pos=(-1.3, pos), align=TextNode.ALeft, scale = .05)

# Function to put title on the screen.
def addTitle(text):
    return OnscreenText(text=text, style=1, fg=(1,1,1,1), pos=(1.3,-0.95), align=TextNode.ARight, scale = .07)

class NormalMapDemo(DirectObject):

    def __init__(self):

        # Post the instructions
        self.title = addTitle("Panda3D: Tutorial - Normal Mapping (aka Bump Mapping)")
        self.inst1 = addInstructions(0.95, "Press ESC to exit")
        self.inst2 = addInstructions(0.90, "Move mouse to rotate camera")
        self.inst3 = addInstructions(0.85, "Left mouse button: Move forwards")
        self.inst4 = addInstructions(0.80, "Right mouse button: Move backwards")
        self.inst5 = addInstructions(0.75, "Enter: Turn normal maps Off")

        # Load the 'abstract room' model.  This is a model of an
        # empty room containing a pillar, a pyramid, and a bunch
        # of exaggeratedly bumpy textures.

        self.room = loader.loadModel(MYDIR+"/models/abstractroom")
        self.room.reparentTo(render)

        # Make the mouse invisible, turn off normal mouse controls
        base.disableMouse()
        props = WindowProperties()
        props.setCursorHidden(True)
        base.win.requestProperties(props)

        # Set the current viewing target
        self.focus = Vec3(48,0,25)
        self.heading = 180
        self.pitch = 0
        self.mousex = 0
        self.mousey = 0
        self.last = 0
        self.mousebtn = [0,0,0]

        # Start the camera control task:
        taskMgr.add(self.controlCamera, "camera-task")
        self.accept("escape", sys.exit, [0])
        self.accept("mouse1", self.setMouseBtn, [0, 1])
        self.accept("mouse1-up", self.setMouseBtn, [0, 0])
        self.accept("mouse2", self.setMouseBtn, [1, 1])
        self.accept("mouse2-up", self.setMouseBtn, [1, 0])
        self.accept("mouse3", self.setMouseBtn, [2, 1])
        self.accept("mouse3-up", self.setMouseBtn, [2, 0])
        self.accept("enter", self.toggleShader)
        self.accept("j", self.rotateLight, [-1])
        self.accept("k", self.rotateLight, [1])
        self.accept("arrow_left", self.rotateCam, [-1])
        self.accept("arrow_right", self.rotateCam, [1])

        # Add a light to the scene.
        self.lightpivot = render.attachNewNode("lightpivot")
        self.lightpivot.setPos(0,0,25)
        self.lightpivot.hprInterval(10,Point3(360,0,0)).loop()
        plight = PointLight('plight')
        plight.setColor(VBase4(1, 1, 1, 1))
        plight.setAttenuation(Vec3(0,0.05,0))
        plnp = self.lightpivot.attachNewNode(plight.upcastToPandaNode())
        plnp.setPos(45, 0, 0)
        self.room.setLight(plnp)
        self.room.setShaderInput("light", plnp)
        
        # Add an ambient light
        alight = AmbientLight('alight')
        alight.setColor(VBase4(0.2, 0.2, 0.2, 1))
        alnp = render.attachNewNode(alight.upcastToPandaNode())
        self.room.setLight(alnp)

        # create a sphere to denote the light
        sphere = loader.loadModel("misc/sphere")
        sphere.reparentTo(plnp)

        # load and apply the shader.
        self.shader = Shader.load(MYDIR+"/bumpMapper.sha")
        self.room.setShader(self.shader)
        self.shaderenable = 1

#       pdb.set_trace()
        
    def setMouseBtn(self, btn, value):
        self.mousebtn[btn] = value

    def rotateLight(self, offset):
        self.lightpivot.setH(self.lightpivot.getH()+offset*20)

    def rotateCam(self, offset):
        self.heading = self.heading - offset*10

    def toggleShader(self):
        self.inst5.destroy()
        if (self.shaderenable):
            self.inst5 = addInstructions(0.75, "Enter: Turn normal maps On")
            self.shaderenable = 0
            self.room.setShaderOff()
        else:
            self.inst5 = addInstructions(0.75, "Enter: Turn normal maps Off")
            self.shaderenable = 1
            self.room.setShader(self.shader)

    def controlCamera(self, task):
        # figure out how much the mouse has moved (in pixels)
        md = base.win.getPointer(0)
        x = md.getX()
        y = md.getY()
        if base.win.movePointer(0, 100, 100):
            self.heading = self.heading - (x - 100)*0.2
            self.pitch = self.pitch - (y - 100)*0.2
        if (self.pitch < -45): self.pitch = -45
        if (self.pitch >  45): self.pitch =  45
        base.camera.setHpr(self.heading,self.pitch,0)
        dir = base.camera.getMat().getRow3(1)
        elapsed = task.time - self.last
        if (self.last == 0): elapsed = 0
        if (self.mousebtn[0]):
            self.focus = self.focus + dir * elapsed*30
        if (self.mousebtn[1]) or (self.mousebtn[2]):
            self.focus = self.focus - dir * elapsed*30
        base.camera.setPos(self.focus - (dir*5))
        if (base.camera.getX() < -49.0): base.camera.setX(-49)
        if (base.camera.getX() >  49.0): base.camera.setX( 49)
        if (base.camera.getY() < -49.0): base.camera.setY(-49)
        if (base.camera.getY() >  49.0): base.camera.setY( 49)
        if (base.camera.getZ() <   1.0): base.camera.setZ(  1)
        if (base.camera.getZ() >  49.0): base.camera.setZ( 49)
        self.focus = base.camera.getPos() + (dir*5)
        self.last = task.time
        return Task.cont


if (base.win.getGsg().getSupportsBasicShaders()):
#    pdb.set_trace()
    t=NormalMapDemo()
else:
    t=addTitle("Normal Map Demo: Video driver reports that shaders are not supported.")

run()
