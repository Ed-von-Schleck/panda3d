import DistributedNodeAI
import DistributedSmoothNodeBase

class DistributedSmoothNodeAI(DistributedNodeAI.DistributedNodeAI,
                              DistributedSmoothNodeBase.DistributedSmoothNodeBase):

    def __init__(self, air, name=None):
        DistributedNodeAI.DistributedNodeAI.__init__(self, air, name)
        DistributedSmoothNodeBase.DistributedSmoothNodeBase.__init__(self)

        
    def preGenerate(self):
        DistributedNodeAI.DistributedNodeAI.preGenerate(self)
        DistributedSmoothNodeBase.DistributedSmoothNodeBase.preGenerate(self)
        self.cnode.setEmbeddedVal(self.zoneId)

    def generate(self):
        DistributedNodeAI.DistributedNodeAI.generate(self)
        DistributedSmoothNodeBase.DistributedSmoothNodeBase.generate(self)
        self.cnode.setRepository(self.air, 1, self.air.ourChannel)

    def disable(self):
        DistributedSmoothNodeBase.DistributedSmoothNodeBase.disable(self)
        DistributedNodeAI.DistributedNodeAI.disable(self)

    def delete(self):
        DistributedSmoothNodeBase.DistributedSmoothNodeBase.delete(self)
        DistributedNodeAI.DistributedNodeAI.delete(self)

    # distributed set pos and hpr functions
    # these are invoked by the DC system
    # 'send' (d_set*) versions are inherited from DistributedSmoothNodeBase
    def setSmStop(self, t=None):
        pass

    # These have their FFI functions exposed for efficiency
    def setSmH(self, h, t=None):
        self.setH(h)

    def setSmZ(self, z, t=None):
        self.setZ(z)

    def setSmXY(self, x, y, t=None):
        self.setX(x)
        self.setY(y)

    def setSmXZ(self, x, z, t=None):
        self.setX(x)
        self.setZ(z)

    def setSmPos(self, x, y, z, t=None):
        self.setPos(x, y, z)

    def setSmHpr(self, h, p, r, t=None):
        self.setHpr(h, p, r)

    def setSmXYH(self, x, y, h, t=None):
        self.setX(x)
        self.setY(y)
        self.setH(h)

    def setSmXYZH(self, x, y, z, h, t=None):
        self.setPos(x, y, z)
        self.setH(h)

    def setSmPosHpr(self, x, y, z, h, p, r, t=None):
        self.setPosHpr(x, y, z, h, p, r)

    def setSmPosHprE(self, x, y, z, h, p, r, e, t=None):
        self.setSmPosHpr(x, y, z, h, p, r, t)
        self.setComponentE(e)

    def clearSmoothing(self, bogus = None):
        pass


    # Do we use these on the AIx?
    def setComponentX(self, x):
        self.setX(x)
    def setComponentY(self, y):
        self.setY(y)
    def setComponentZ(self, z):
        self.setZ(z)
    def setComponentH(self, h):
        self.setH(h)
    def setComponentP(self, p):
        self.setP(p)
    def setComponentR(self, r):
        self.setR(r)
    def setComponentE(self, e):
        # Override this in subclasses to handle the subclass-defined embedded data

        # This really doesn't do anything, but the cnode is as good a place as any to store
        # the embedded value received from the controlling object.
        self.cnode.setEmbeddedVal(e)
        pass
    def setComponentT(self, t):
        pass

    def getComponentX(self):
        return self.getX()
    def getComponentY(self):
        return self.getY()
    def getComponentZ(self):
        return self.getZ()
    def getComponentH(self):
        return self.getH()
    def getComponentP(self):
        return self.getP()
    def getComponentR(self):
        return self.getR()
    def getComponentE(self):
        return self.cnode.getEmbeddedVal()
    def getComponentT(self):
        return 0

    @report(types = ['args'], dConfigParam = 'smoothnode')
    def wrtReparentTo(self, parent):
        DistributedNodeAI.DistributedNodeAI.wrtReparentTo(self, parent)

        # Update our broadcast data source with our new current position.
        # Any embedded data will need to be handled separately.
        if self.isGenerated():
            self.cnode.refreshPosHpr()
            pass
        pass
