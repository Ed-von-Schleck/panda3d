"""DistributedSmoothNodeBase module: contains the DistributedSmoothNodeBase class"""

from ClockDelta import *

class DistributedSmoothNodeBase:
    """common base class for DistributedSmoothNode and DistributedSmoothNodeAI
    """
    def __init__(self):
        pass

    def delete(self):
        pass

    ### distributed set pos and hpr functions ###

    ### These functions send the distributed update to set the
    ### appropriate values on the remote side.  These are
    ### composite fields, with all the likely combinations
    ### defined; each function maps (via the dc file) to one or
    ### more component operations on the remote client.

    def d_setSmStop(self):
        self.sendUpdate("setSmStop", [globalClockDelta.getFrameNetworkTime()])
    def d_setSmH(self, h):
        self.sendUpdate("setSmH", [h, globalClockDelta.getFrameNetworkTime()])
    def d_setSmXY(self, x, y):
        self.sendUpdate("setSmXY", [x, y,
                                    globalClockDelta.getFrameNetworkTime()])
    def d_setSmXZ(self, x, z):
        self.sendUpdate("setSmXZ", [x, z,
                                    globalClockDelta.getFrameNetworkTime()])
    def d_setSmPos(self, x, y, z):
        self.sendUpdate("setSmPos", [x, y, z,
                                     globalClockDelta.getFrameNetworkTime()])
    def d_setSmHpr(self, h, p, r):
        self.sendUpdate("setSmHpr", [h, p, r,
                                     globalClockDelta.getFrameNetworkTime()])
    def d_setSmXYH(self, x, y, h):
        self.sendUpdate("setSmXYH", [x, y, h,
                                     globalClockDelta.getFrameNetworkTime()])
    def d_setSmXYZH(self, x, y, z, h):
        self.sendUpdate("setSmXYZH", [x, y, z, h,
                                      globalClockDelta.getFrameNetworkTime()])
    def d_setSmPosHpr(self, x, y, z, h, p, r):
        self.sendUpdate("setSmPosHpr", [x, y, z, h, p, r,
                                        globalClockDelta.getFrameNetworkTime()])

    def b_clearSmoothing(self):
        self.d_clearSmoothing()
        self.clearSmoothing()
    def d_clearSmoothing(self):
        self.sendUpdate("clearSmoothing", [0])
