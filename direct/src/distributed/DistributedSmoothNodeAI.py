from AIBaseGlobal import *
import DistributedNodeAI
import ClockDelta

class DistributedSmoothNodeAI(DistributedNodeAI.DistributedNodeAI):
    def __init__(self, air, name=None):
        DistributedNodeAI.DistributedNodeAI.__init__(self, air, name)

    def delete(self):
        DistributedNodeAI.DistributedNodeAI.delete(self)

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
    def setComponentT(self, t):
        pass

    def getRealNetworkTime(self):
        return ClockDelta.globalClockDelta.getRealNetworkTime()

    def d_setSmStop(self):
        self.sendUpdate("setSmStop", [self.getRealNetworkTime()])
    def setSmStop(self, t):
        self.setComponentT(t)

    def d_setSmH(self, h):
        self.sendUpdate("setSmH", [h, self.getRealNetworkTime()])
    def setSmH(self, h, t):
        self.setComponentH(h)
        self.setComponentT(t)

    def d_setSmXY(self, x, y):
        self.sendUpdate("setSmXY", [x, y, self.getRealNetworkTime()])
    def setSmXY(self, x, y, t):
        self.setComponentX(x)
        self.setComponentY(y)
        self.setComponentT(t)

    def d_setSmXZ(self, x, z):
        self.sendUpdate("setSmXZ", [x, z, self.getRealNetworkTime()])
    def setSmXZ(self, x, z, t):
        self.setComponentX(x)
        self.setComponentZ(z)
        self.setComponentT(t)

    def d_setSmPos(self, x, y, z):
        self.sendUpdate("setSmXYZ", [x, y, z, self.getRealNetworkTime()])
    def setSmPos(self, x, y, z, t):
        self.setComponentX(x)
        self.setComponentY(y)
        self.setComponentZ(z)
        self.setComponentT(t)

    def d_setSmHpr(self, h, p, r):
        self.sendUpdate("setSmHPR", [h, p, r, self.getRealNetworkTime()])
    def setSmHpr(self, h, p, r, t):
        self.setComponentH(h)
        self.setComponentP(p)
        self.setComponentR(r)
        self.setComponentT(t)

    def d_setSmXYH(self, x, y, h):
        self.sendUpdate("setSmXYH", [x, y, h, self.getRealNetworkTime()])
    def setSmXYH(self, x, y, h, t):
        self.setComponentX(x)
        self.setComponentY(y)
        self.setComponentH(h)
        self.setComponentT(t)

    def d_setSmXYZH(self, x, y, z, h):
        self.sendUpdate("setSmXYZH", [x, y, z, h, self.getRealNetworkTime()])
    def setSmXYZH(self, x, y, z, h, t):
        self.setComponentX(x)
        self.setComponentY(y)
        self.setComponentZ(z)
        self.setComponentH(h)
        self.setComponentT(t)

    def d_setSmPosHpr(self, x, y, z, h, p, r):
        self.sendUpdate("setSmPosHpr", [x, y, z, h, p, r,
                                        self.getRealNetworkTime()])
    def setSmPosHpr(self, x, y, z, h, p, r, t):
        self.setComponentX(x)
        self.setComponentY(y)
        self.setComponentZ(z)
        self.setComponentH(h)
        self.setComponentP(p)
        self.setComponentR(r)
        self.setComponentT(t)
