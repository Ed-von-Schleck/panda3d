
from ShowBaseGlobal import *
from intervalGlobal import *
import DirectObject
import math




class ViewPort:
           
    
    def __init__(self,X1,X2,Y1,Y2,layer,cam,background=Vec4(0.3,0.3,0.3,1),projection="perspective",type="top",scene=render):
        self.VPType=type
        self.VP_X1=X1
	self.VP_Y1=Y1
	self.VP_X2=X2
	self.VP_Y2=Y2
        self.VP_width=self.VP_X2 - self.VP_X1
	self.VP_height=self.VP_Y2 - self.VP_Y1
        
	self.the_viewport=layer.makeDisplayRegion(self.VP_X1, self.VP_X2,self.VP_Y1, self.VP_Y2)
        self.the_viewport.setCamera(cam)
        self.the_viewport.setClearDepthActive(1)
        self.the_viewport.setClearColorActive(1)
        self.the_viewport.setClearColor(background)
     	self.cam=cam   
     

        # Set up the cameras to look in the right place.
	if(type=="top"):
            self.cam.setP(-90)
            self.cam.setZ(-40)
	elif(type=="left"):
            self.cam.setH(-90)
            self.cam.setX(10)
        elif(type=="front"):
            self.cam.setY(-10) 
        elif(type=="perspective"):
	    cam.setY(-100)
	    #cam.setX(10)
	    #cam.setZ(-10)
	    #cam.setH(45)
            #cam.setP(-45)   
	    print "aa"



        if(projection=="ortho"):
	    self.lens=OrthographicLens()
    	    self.lens.setAspectRatio((self.VP_X2-self.VP_X1)/(self.VP_Y2-self.VP_Y1))
            self.lens.setFilmSize(self.VP_width*200,self.VP_height*200)
	    #lens.setFilmOffset((self.VP_X2 + self.VP_X1) * 0.5, (self.VP_Y2 + self.VP_Y1) * 0.5)
	    self.lens.setNearFar(-1000, 1000)
            self.cam.node().setLens(self.lens)
            self.cam.node().setScene(scene)
        elif(projection=="perspective"):
	    self.lens=base.cam.node().getLens()
    	    self.lens.setAspectRatio((self.VP_X2-self.VP_X1)/(self.VP_Y2-self.VP_Y1))
	    self.cam.node().setLens(self.lens)
	    self.cam.node().setScene(scene)

        self.the_viewport.setCamera(self.cam)

    def resizeX(self,width_increment):
        if(self.VPType=="top" or self.VPType=="left"):
            self.the_viewport.setDimensions(self.VP_X1,self.VP_X2+width_increment,self.VP_Y1,self.VP_Y2)
        elif(self.VPType=="perspective" or self.VPType=="front"):
            self.the_viewport.setDimensions(self.VP_X1+width_increment,self.VP_X2,self.VP_Y1,self.VP_Y2)

    def resizeY(self,height_increment,direction):
	if(self.VPType=="left" or self.type=="perspective"):
            self.the_viewport.setDimensions(self.VP_X1,self.VP_X2,self.VP_Y1,self.VP_Y2+height_increment)
	else:
	    self.the_viewport.setDimensions(self.VP_X1,self.VP_X2,self.VP_Y1+height_increment,self.VP_Y2)

    def AdjustAspect(self,x,y):
	if (y==0):
	    y=1
	self.lens.setAspectRatio(x/y)
	self.cam.node().setLens(self.lens)
   
    def resize(self,x,y):
	
	if(self.VPType=="left"):
	    self.the_viewport.setDimensions(0,x,0,y)
	    w=abs(x-self.VP_X1)
	    h=abs(y-self.VP_Y1)
	    if(h==0):
	        h=1
	    self.lens.setAspectRatio(w/h)
	    self.cam.node().setLens(self.lens)
	if(self.VPType=="top"):
	    self.the_viewport.setDimensions(0,x,y,1)
	    w=abs(x-self.VP_X1)
	    h=abs(self.VP_Y2-y)
	    if(h==0):
	        h=1
	    self.lens.setAspectRatio(w/h)
	    self.cam.node().setLens(self.lens)
	if(self.VPType=="front"):
	    self.the_viewport.setDimensions(x,1,y,1)
	    w=abs(self.VP_X2-x)
	    h=abs(self.VP_Y2-y)
	    if(h==0):
	        h=1
	    self.lens.setAspectRatio(w/h)
	    self.cam.node().setLens(self.lens)
	if(self.VPType=="perspective"):
	    self.the_viewport.setDimensions(x,1,0,y)
	    w=abs(self.VP_X2-x)
	    h=abs(y-self.VP_Y1)
	    if(h==0):
	        h=1
	    self.lens.setAspectRatio(w/h)
	    self.cam.node().setLens(self.lens)

class QuadView(DirectObject.DirectObject):


    def __init__(self):
	self.ControlPressed=0
	self.AltPressed=0
        self.PanConstantX=50
	self.PanConstantY=50
	self.ZoomConstant=1	
	self.FrontWidth=100
	self.FrontHeight=100
	self.TopWidth=100
	self.TopHeight=100
	self.LeftWidth=100
	self.LeftHeight=100

	self.MouseButton=0
	self.CurrentQuad=4
	self.HorizontalAxis=0.5
	self.VerticalAxis=0.5
        base.disableMouse()
	self.MouseDragging=0
	self.dataUnused = NodePath('dataUnused')
	self.mouse2cam = self.dataUnused.attachNewNode(Transform2SG('mouse2cam'))
        self.currX= 0
	self.oldX=self.currX
        self.currY= 0
	self.oldY=self.currY

	self.FrontTexture=1
	self.LeftTexture=1
	self.PerspectiveTexture=1
	self.TopTexture=1

	self.FrontWire=0
	self.LeftWire=0
	self.PerspectiveWire=0
	self.TopWire=0


	# Keep track of the currently selected window... values are 1-4 for four quadrants of a standard 
	# Cartesian coordinate system

	
        # These are the orthographic cameras
        # They will be restricted to panning and zooming i.e. no rotation
        # Top could be flipped to back, left to right and front to back
        self.topCam= render.attachNewNode(Camera('topCam'))
        self.frontCam = render.attachNewNode(Camera('frontCam'))
        self.leftCam= render.attachNewNode(Camera('leftCam'))

	# This camera will have a trackball control since its perspective
        self.perspectiveCam = render.attachNewNode(Camera('perspectiveCam'))
	
	#self.toplens=OrthographicLens()
	#self.leftLens=OrthographicLens()
	#self.frontLens=OrthographicLens()
	#self.perspectiveLens=base.cam.node().getLens()


        # For now all lenses are same as that of base.cam
        #self.topCamLens=OrthographicLens()
        #self.frontCamLens= base.cam.node().getLens()
        #self.leftCamLens= base.cam.node().getLens()
        #self.perspectiveCamLens= base.cam.node().getLens()

	# Manipulate lenses here if need be	
	#self.topCamLens.setFilmSize(250)

	# Set the Lenses
	#self.topCam.node().setLens(self.topCamLens)
	#self.frontCam.node().setLens(self.frontCamLens)
	#self.leftCam.node().setLens(self.leftCamLens)
	#self.perspectiveCam.node().setLens(self.perspectiveCamLens)

        # Add any model to view
        #self.smiley = loader.loadModel('Atmosphere')
        #self.smiley.reparentTo(render)
        #self.badwiz = loader.loadModel('badwizard1')
	#self.badwiz.setScale(0.01,0.01,0.01)
	#self.badwiz.reparentTo(render)
	self.sun = loader.loadModel('badwizard1')
        self.sun.reparentTo(render)
	#self.sun.setRenderModeWireframe(100)
        base.useTrackball()
	# Create four separate display regions for the quad view.
        # These will overlap the main display region
        # To stack these overlapping DisplayRegions, we need a new layer.  If
        # they didn't overlap, we could put them in the same layer.

        self.newLayer = base.win.getChannel(0).makeLayer()

	self.PerspectiveScene=NodePath('PerspectiveScene')
	self.FrontScene=NodePath('FrontScene')
	self.TopScene=NodePath('TopScene')
	self.LeftScene=NodePath('LeftScene')
	self.SceneParent=NodePath('SceneParent')

	self.PerspectiveScene=render.copyTo(render)
	self.FrontScene=render.copyTo(render)
	self.TopScene=render.copyTo(render)
	self.LeftScene=render.copyTo(render)
	
	self.PerspectiveScene.reparentTo(self.SceneParent)
	self.FrontScene.reparentTo(self.SceneParent)
	self.TopScene.reparentTo(self.SceneParent)
	self.LeftScene.reparentTo(self.SceneParent)
        
        #self.Perspective=ViewPort(0.5,1.0,0.0,0.5,self.newLayer,self.perspectiveCam,Vec4(0.75,0.75,0.75,1),"perspective","perspective",self.PerspectiveScene)
	self.Top=ViewPort(0.0,0.5,0.5,1.0,self.newLayer,self.topCam,Vec4(0.80,0.80,0.80,1),"ortho","top",self.TopScene)
        self.Left=ViewPort(0.0,0.5,0.0,0.5,self.newLayer,self.leftCam,Vec4(0.85,0.85,0.85,1),"ortho","left",self.LeftScene)
        self.Front=ViewPort(0.5,1.0,0.5,1.0,self.newLayer,self.frontCam,Vec4(0.85,0.85,0.85,1),"ortho","front",self.FrontScene)

	#base.setMouseOnNode(self.smiley.node()) # Let Mouse Control Perspective View for now
	#base.enableSoftwareMousePointer()
        
        # Message Handlers
        self.accept("a",self.setLeft)
        self.accept("q",self.setTop)
        self.accept("w",self.setFront)
        self.accept("s",self.setPerspective)
	self.accept("o",self.ortho)
	self.accept("mouse1",self.MouseTell,[1])
	self.accept("mouse2",self.MouseTell,[2])
	self.accept("mouse3",self.MouseTell,[3])
	self.accept("mouse1-up",self.MouseTellUp,[4])
	self.accept("mouse2-up",self.MouseTellUp,[5])
	self.accept("mouse3-up",self.MouseTellUp,[6])
	self.accept("mouse2-scroll",self.resizedr)
	self.accept("r",self.resizedr)
	self.accept("g",self.gridtoggle)
	self.accept("t", self.ToggleTexture)
	self.accept("alt",self.AltHandler)
	self.accept("alt-up",self.AltUpHandler)
	self.accept("w",self.ToggleWire)
	self.accept("alt-mouse1",self.AltDown)
	self.accept("alt-mouse1-up",self.AltUp)
	self.accept("control-mouse1",self.CtlDown)
	self.accept("control-mouse1-up",self.CtlUp)
	
    # Methods
    def AltDown(self):
	self.AltPressed=1
	print "as"

    def AltUp(self):
	self.AltPressed=0
	print "as"

    def CtlDown(self):
	self.ControlPressed=1
	print "as"

    def CtlUp(self):
	self.ControlPressed=0
	print "as"

    def ToggleWire(self):
	if (self.CurrentQuad==1): # Front View
	    if(self.FrontWire): # Wireframe is On so turn it off
	        self.FrontScene.setRenderModeWireframe(100);
                self.FrontScene.setTwoSided(1);
	        #self.FrontScene.setTextureOff(100)
	        self.FrontWire=0
	    else:
	        self.FrontScene.clearRenderMode()
	        #self.FrontScene.setTwoSided(not self.backfaceCullingEnabled)
		if(self.FrontTexture):
			 self.FrontScene.clearTexture()
	        self.FrontWire=1

    def ToggleTexture(self):
	if (self.CurrentQuad==1): # Front View
	    if(self.FrontTexture): # Texture is on so turn it off
	        self.FrontScene.setTextureOff(100)
	  	self.FrontTexture=0;
	    else:
	        self.FrontScene.clearTexture()
	        self.FrontTexture=1

	
        

    def AltHandler(self):
	print "THIS" + str(base.mouseWatcherNode.getButtonDownPattern())
	self.oldX=base.mouseWatcherNode.getMouseX()
	if(self.oldX<-1 or self.oldX>1):
	    return
	self.oldY=base.mouseWatcherNode.getMouseY()
	if(self.oldY<-1 or self.oldY>1):
	    return
	taskMgr.add(self.DragAction,'DragAction')
	    
    def AltUpHandler(self):
        taskMgr.remove('DragAction')	
	
    def gridtoggle(self):
	base.win.getDisplayRegion(0).setDimensions(0.5,1,0,0.5)
        #grid=DirectGrid()
	#grid.enable()

    def resizedr(self,x,y):
	print "X: " + str(x) + " Y: " + str(y)
        #self.Perspective.resize(x,y)
  	self.Top.resize(x,y)
	self.Front.resize(x,y)
	self.Left.resize(x,y)

    def setAppropriateViewPort(self,x,y):
	print str(x) + " " + str(y)
	x=(x+1)/2.0
	y=(y+1)/2.0
        if(x<self.VerticalAxis):
            if(y<self.HorizontalAxis):
                self.setLeft()
            else:
                self.setTop()
        else:
            if(y<self.HorizontalAxis):
                self.setPerspective()
            else:
                self.setFront()
	
    def MouseTell(self,buttonCode):
	self.MouseButton=buttonCode
        self.setAppropriateViewPort(base.mouseWatcherNode.getMouseX(),base.mouseWatcherNode.getMouseY())

	self.oldX=base.mouseWatcherNode.getMouseX()
	if(self.oldX<-1 or self.oldX>1):
	    return

	self.oldY=base.mouseWatcherNode.getMouseY()
	if(self.oldY<-1 or self.oldY>1):
	    return

        self.Mouse_Dragging=1
	taskMgr.add(self.DragAction,'DragAction')
	#print "Mouse Down" + str(buttonCode)
	#print str(base.mouseWatcherNode.getMouseX())
	#print str(base.mouseWatcherNode.getMouseY())

    def MouseTellUp(self,buttoncode):
	#self.MouseButton=0
	self.PanConstantX= 50
	self.PanConstantY= 50
	self.ZoomConstant=1
	taskMgr.remove('DragAction')
 	self.Mouse_Draggin=0
	#print "Mouse Up"
	#print str(base.mouseWatcherNode.getMouseX())
	#print str(base.mouseWatcherNode.getMouseY())


    def Max_Style_Mouse_View(self,buttoncode):
	pass
		
    def setLeft(self):
	#base.disableMouse()
	print "LEFT"
	self.CurrentQuad=3
	#base.setMouseOnNode(self.leftCam.node())

    def setTop(self):
	#+base.disableMouse()
	#base.useDrive()
	print "TOP"
	self.CurrentQuad=2
        #base.setMouseOnNode(self.topCam.node())

    def setPerspective(self):
	print "PERSPECTIVE"
	self.CurrentQuad=4
	base.useTrackball()
	#base.enableMouse()
	#base.oobe()
	#base.setMouseOnNode(self.perspectiveCam.node())

    def setFront(self):
	#base.disableMouse()
	print "FRONT"
	self.CurrentQuad=1
	#base.setMouseOnNode(self.frontCam.node())

    def ortho(self):
	base.makeCamera2d(base.win,-1,1,-1,1)
    
    def DragAction(self,task):
        #if(self.MouseDragging==1):
        self.currX= base.mouseWatcherNode.getMouseX()
	#if(self.currX<-1 or self.currX>1):
	#    return
	self.currY= base.mouseWatcherNode.getMouseY()
	#if(self.currY<-1 or self.currY>1):
	#    return


	self.diffX=self.currX-self.oldX
	self.diffY=self.currY-self.oldY

	#if(self.ControlPressed): # Change Size of the ViewPorts
	if(base.getControl()):
	     self.VerticalAxis=(self.currX+1)/2.0
	     self.HorizontalAxis=(self.currY+1)/2.0
	     if(self.HorizontalAxis<0 or self.HorizontalAxis>1 or self.VerticalAxis<0 or self.VerticalAxis>1):
	         return
	     self.resizedr(self.VerticalAxis,self.HorizontalAxis)
	
	#if(self.AltPressed): # View Camera Transforms -> Maya style
	elif(1):
	    #print "ALTPRESSED"
	    if(self.PanConstantX<4096):
 	        self.PanConstantX= self.PanConstantX * 2
	        self.PanConstantY= self.PanConstantY * 2
	    self.ZoomConstant= self.ZoomConstant + 50
	    if(self.MouseButton==1): # TrackBall rotation only for Perspective View
	        if(self.CurrentQuad==4):
		    #self.RotatCam
		    print "THIS IS WHERE I WILL DO TRACKBALL ROTATION"
            elif(self.MouseButton==2): # Do Panning
	 	print "PANNING"
		if(self.CurrentQuad==1): # Y and Z values change meanings for different cameras
		    self.MoveCamera(-self.diffX*self.PanConstantX,0,-self.diffY*self.PanConstantY,self.CurrentQuad)
		elif(self.CurrentQuad==2):
		    self.MoveCamera(-self.diffX*self.PanConstantX,-self.diffY*self.PanConstantY,0,self.CurrentQuad)
		elif(self.CurrentQuad==3):
		    self.MoveCamera(0,self.diffX*self.PanConstantX,-self.diffY*self.PanConstantY,self.CurrentQuad)
		elif(self.CurrentQuad==4):
		    #self.MoveCamera(0,-self.diffX*self.PanConstantX,-self.diffY*self.PanConstantY,self.CurrentQuad)	
	            print"PERSP PAN"
            elif(self.MouseButton==3): # Do Zoom 
		if(self.CurrentQuad==1): # Y and Z values change meanings for different cameras
 		    #lens = OrthographicLens()
                    #lens.setFilmSize(l,self.VP_height*200)
	    	    #lens.setFilmOffset((self.VP_X2 + self.VP_X1) * 0.5, (self.VP_Y2 + self.VP_Y1) * 0.5)
	    	    #lens.setNearFar(-1000, 1000)
		    self.FrontWidth= self.FrontWidth + self.diffX*self.ZoomConstant
		    self.FrontHeight= self.FrontHeight + self.diffX*self.ZoomConstant
		    self.FrontWidth= self.FrontWidth + self.diffY*self.ZoomConstant
		    self.FrontHeight= self.FrontHeight + self.diffY*self.ZoomConstant

		    if(self.FrontWidth<=0):
			Frontwidth=1
		    if(self.FrontHeight<=0):
			FrontHeight=1
		    print str(self.FrontWidth)
	            self.frontCam.node().getLens().setFilmSize(self.FrontWidth,self.FrontHeight)
		    self.resizedr(self.VerticalAxis,self.HorizontalAxis)	
            	elif(self.CurrentQuad==2):
		    self.TopWidth= self.TopWidth + self.diffX*self.ZoomConstant
		    self.TopHeight= self.TopHeight + self.diffX*self.ZoomConstant
		    self.TopWidth= self.TopWidth + self.diffY*self.ZoomConstant
		    self.TopHeight= self.TopHeight + self.diffY*self.ZoomConstant
	            self.topCam.node().getLens().setFilmSize(self.TopWidth,self.TopHeight)
		    self.resizedr(self.VerticalAxis,self.HorizontalAxis)	
		elif(self.CurrentQuad==3):
		    self.LeftWidth= self.LeftWidth + self.diffX*self.ZoomConstant
		    self.LeftHeight= self.LeftHeight + self.diffX*self.ZoomConstant
		    self.LeftWidth= self.LeftWidth + self.diffY*self.ZoomConstant
		    self.LeftHeight= self.LeftHeight + self.diffY*self.ZoomConstant
		    self.leftCam.node().getLens().setFilmSize(self.LeftWidth,self.LeftHeight)
		    self.resizedr(self.VerticalAxis,self.HorizontalAxis)	
		elif(self.CurrentQuad==4):
		    print "NOOGEA"
	else:
  	     print "H: " + str(self.perspectiveCam.getH()) +" P:  " + str(self.perspectiveCam.getP()) + " R: " + str(self.perspectiveCam.getH())	     
             #print "X: " + str(self.perspectiveCam.getX()) +" Y:  " + str(self.perspectiveCam.getY()) + " Z: " + str(self.perspectiveCam.getZ())	     
	     
		
        #print "Difference: " + str(self .currX-self.oldX)
        self.oldX=self.currX
        self.oldY=self.currY
        return Task.cont

    def MoveCamera(self,X_amt,Y_amt,Z_amt,quad):
	if(quad==1):
            self.frontCam.setPos(self.frontCam.getX()+X_amt,self.frontCam.getY()+Y_amt,self.frontCam.getZ()+Z_amt)
	elif(quad==2):
            self.topCam.setPos(self.topCam.getX()+X_amt,self.topCam.getY()+Y_amt,self.topCam.getZ()+Z_amt)
	elif(quad==3):
            self.leftCam.setPos(self.leftCam.getX()+X_amt,self.leftCam.getY()+Y_amt,self.leftCam.getZ()+Z_amt)
	elif(quad==4):
            self.perspectiveCam.setPos(self.perspectiveCam.getX()+X_amt,self.perspectiveCam.getY()+Y_amt,self.perspectiveCam.getZ()+Z_amt)
	




View=QuadView()
run()