from ShowBaseGlobal import *


class QuadView:

	def __init__(self):
      	smiley = loader.loadModel('Atmosphere')
		smiley.reparentTo(render)

		# A new camera for a different view.
		newCamera = render.attachNewNode(Camera('newCamera'))
		newCamera.setPos(smiley, Point3(0, -10, 0))
		newCamera.lookAt(smiley)

		# It uses the same lens as the default camera's lens.  It could have a
		# different lens if we wanted.
		newCamera.node().setLens(base.camLens)

		# This camera views render, but it could view any scene graph we
		# specify.
		newCamera.node().setScene(render)


		# Now create a DisplayRegion in the lower right corner to render the
		# output of this camera.  We'll layer this DisplayRegion on top of the
		# existing DisplayRegion.

		# To stack these overlapping DisplayRegions, we need a new layer.  If
		# they didn't overlap, we could put them in the same layer.
		newLayer = base.win.getChannel(0).makeLayer()
		displayRegion = newLayer.makeDisplayRegion(0.5, 1.0, 0.0, 0.5)
	
		displayRegion1 = newLayer.makeDisplayRegion(0.0, 0.5, -0.5, 0.0)
		displayRegion2 = newLayer.makeDisplayRegion(0.0, 0.5, 0.5, 0.0)
		displayRegion3 = newLayer.makeDisplayRegion(-0.5, 1.0, 0.0, 0.5)

		displayRegion.setCamera(newCamera)
		displayRegion1.setCamera(newCamera)

		displayRegion2.setCamera(newCamera)
	
		displayRegion3.setCamera(newCamera)
		# Make sure the displayRegion clears its area before it draws into it.
		displayRegion.setClearDepthActive(1)
		displayRegion.setClearColorActive(1)
		displayRegion.setClearColor(Vec4(0.0,0.3,0.0,1))
	
		displayRegion1.setClearDepthActive(1)
		displayRegion1.setClearColorActive(1)
		displayRegion1.setClearColor(Vec4(0.0,0.0,0.3,1))


		displayRegion2.setClearDepthActive(1)
		displayRegion2.setClearColorActive(1)
		displayRegion2.setClearColor(Vec4(0.3,0.0,0.0,1))


		displayRegion3.setClearDepthActive(1)
		displayRegion3.setClearColorActive(1)
		displayRegion3.setClearColor(Vec4(0.3,0.3,0.3,1))

View=QuadView()
run()