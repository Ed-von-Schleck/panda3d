#Author: Kwasi Mensah
#Date: 7/11/2005
#
# This is a tutorial to show some of the more advanced things
# you can do with Cg. Specifically, with Non Photo Realistic
# effects like Toon Shading. It also shows how to implement
# multiple buffers in Panda.


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
    return OnscreenText(text=msg, style=1, fg=(1,1,1,1),
      pos=(-1.3, pos), align=TextNode.ALeft, scale = .05)

# Function to put title on the screen.
def addTitle(text):
    return OnscreenText(text=text, style=1, fg=(1,1,1,1),
                  pos=(1.3,-0.95), align=TextNode.ARight, scale = .07)


class ToonMaker(DirectObject):
    def __init__(self):
        base.disableMouse()
        camera.setPos(0, -50, 0)

        # Post the instructions.
        self.title = addTitle("Panda3D: Tutorial - Toon Shading with Normals-Based Inking")
        self.inst1 = addInstructions(0.95,"ESC: Quit")
        self.inst2 = addInstructions(0.90,"Up/Down: Increase/Decrease Line Thickness")
        self.inst3 = addInstructions(0.85,"Left/Right: Decrease/Increase Line Darkness")

        # This shader's job is to render the model with discrete lighting
        # levels.  The lighting calculations built into the shader assume
        # a single nonattenuating point light.

        lightingGen=Shader.load(MYDIR+"/lightingGen.sha")
        lightingGenAttrib = ShaderAttrib.make().setShader(lightingGen)
        base.cam.node().setInitialState(render.getState().addAttrib(lightingGenAttrib))
        
        # This is the object that represents the single "light", as far
        # the shader is concerned.  It's not a real Panda3D LightNode, but
        # the shader doesn't care about that.

        light = render.attachNewNode("light")
        light.setPos(30,-50,0)
                
        # this call puts the light's nodepath into the render state.
        # this enables the shader to access this light by name.

        render.setShaderInput("light", light)

        # The "normals buffer" will contain a picture of the model colorized
        # so that the color of the model is a representation of the model's
        # normal at that point.

        normalsBuffer=base.win.makeTextureBuffer("normalsBuffer", 512, 512)
        normalsBuffer.setClearColor(VBase4(0.5,0.5,0.5,1))
        self.normalsBuffer=normalsBuffer
        normalsCamera=base.makeCamera(normalsBuffer, aspectRatio=base.getAspectRatio())
        normalsCamera.node().setScene(render)
        normalGen=Shader.load(MYDIR+"/normalGen.sha")
        normalGenAttrib=ShaderAttrib.make().setShader(normalGen)
        normalsCamera.node().setInitialState(render.getState().addAttrib(normalGenAttrib))
                
        #what we actually do to put edges on screen is apply them as a texture to 
        #a transparent screen-fitted card

        drawnScene=normalsBuffer.getTextureCard()
        drawnScene.setTransparency(1)
        drawnScene.setColor(1,1,1,0)
        drawnScene.reparentTo(render2d)
        self.drawnScene = drawnScene

        # this shader accepts, as input, the picture from the normals buffer.
        # it compares each adjacent pixel, looking for discontinuities.
        # wherever a discontinuity exists, it emits black ink.
                
        self.separation = 0.001
        self.cutoff = 0.3
        inkGen=Shader.load(MYDIR+"/inkGen.sha")
        drawnScene.setShader(inkGen)
        drawnScene.setShaderInput("separation", Vec4(self.separation,0,self.separation,0));
        drawnScene.setShaderInput("cutoff", Vec4(self.cutoff,self.cutoff,self.cutoff,self.cutoff));
        
        # Load a dragon model and animate it.

        self.character=Actor.Actor()
        self.character.loadModel(MYDIR+'/nik-dragon')
        self.character.reparentTo(render)
        self.character.loadAnims({'win': MYDIR+'/nik-dragon'})
        self.character.loop('win')
        self.character.hprInterval(15, Point3(360, 0,0)).loop()

        # This quad contains the contents of the normals buffer.
        # We'll position it in the lower-left corner of the screen,
        # so you can see what's happening under the covers.

        normsCard=self.normalsBuffer.getTextureCard()
        normsCard.reparentTo(render2d)
        normsCard.setScale(0.25,1,0.25)
        normsCard.setPos(-0.75,0,-0.75)

        # these allow you to change cartooning parameters in realtime

        self.accept("escape", sys.exit, [0])
        self.accept("arrow_up", self.increaseSeparation)
        self.accept("arrow_down", self.decreaseSeparation)
        self.accept("arrow_left", self.increaseCutoff)
        self.accept("arrow_right", self.decreaseCutoff)
        
    def increaseSeparation(self):
        self.separation = self.separation * 1.11111111;
        print self.separation
        self.drawnScene.setShaderInput("separation", Vec4(self.separation,0,self.separation,0));
        
    def decreaseSeparation(self):
        self.separation = self.separation * 0.90000000;
        print self.separation
        self.drawnScene.setShaderInput("separation", Vec4(self.separation,0,self.separation,0));
        
    def increaseCutoff(self):
        self.cutoff = self.cutoff * 1.11111111;
        print self.cutoff
        self.drawnScene.setShaderInput("cutoff", Vec4(self.cutoff,self.cutoff,self.cutoff,self.cutoff));
        
    def decreaseCutoff(self):
        self.cutoff = self.cutoff * 0.90000000;
        print self.cutoff
        self.drawnScene.setShaderInput("cutoff", Vec4(self.cutoff,self.cutoff,self.cutoff,self.cutoff));
        
if (base.win.getGsg().getSupportsBasicShaders()):
#    pdb.set_trace()
    t=ToonMaker()
else:
    t=addTitle("Toon Shader: Video driver reports that shaders are not supported.")

run()
