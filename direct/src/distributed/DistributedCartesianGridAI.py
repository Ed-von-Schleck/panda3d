
from pandac.PandaModules import *

from direct.task import Task
from DistributedNodeAI import DistributedNodeAI
from CartesianGridBase import CartesianGridBase
from GridChild import GridChild

class DistributedCartesianGridAI(DistributedNodeAI, CartesianGridBase):
    notify = directNotify.newCategory("DistributedCartesianGridAI")

    RuleSeparator = ":"

    def __init__(self, air, startingZone, gridSize, gridRadius, cellWidth,
            style="Cartesian"):
        DistributedNodeAI.__init__(self, air)
        self.style = style
        self.startingZone = startingZone
        self.gridSize = gridSize
        self.gridRadius = gridRadius
        self.cellWidth = cellWidth

        # Keep track of all AI objects added to the grid
        self.gridObjects = {}
        self.updateTaskStarted = 0

    def delete(self):
        DistributedNodeAI.delete(self)
        self.stopUpdateGridTask()

    def isGrid(self):
        # If this distributed object is a DistributedGrid return 1.
        # 0 by default
        return 1

    def getCellWidth(self):
        return self.cellWidth
    
    def getParentingRules(self):
        self.notify.debug("calling getter")
        rule = ("%i%s%i%s%i" % (self.startingZone, self.RuleSeparator,
                                self.gridSize, self.RuleSeparator,
                                self.gridRadius))
        return [self.style, rule]

    def handleChildArrive(self, child, zoneId):
        assert isinstance(child, GridChild), "Must be a GridChild to be setLocation()ed here"
        DistributedNodeAI.handleChildArrive(self, child, zoneId)
        if (zoneId >= self.startingZone):
            child.setGridCell(self, zoneId)
        else:
            child.setGridCell(None, 0)
            pass
        pass

    def handleChildArriveZone(self, child, zoneId):
        DistributedNodeAI.handleChildArriveZone(self, child, zoneId)
        if (zoneId >= self.startingZone):
            child.setGridCell(self, zoneId)
        else:
            child.setGridCell(None, 0)
            pass
        pass
        
    def handleChildLeave(self, child, zoneId):
        child.setGridCell(None, 0)
        DistributedNodeAI.handleChildLeave(self, child, zoneId)
        pass

    def parentObjectToArea(self, child):
        """
        Assumes the child is either parented directly to the grid or detached
        and has the desired absolute position wrt the grid.
        This attaches the child to the grid with a grid cell node in-between.
        The relative transform between the child and grid is maintained.
        """
        assert isinstance(child, GridChild), "Must be a GridChild to do this"
        assert not child.getParent() or child.getParent() == self, "Parent must be grid or None"
        child.reparentTo(self) # go ahead and put us here
        zoneId = self.getZoneFromXYZ(child.getPos())
        child.setGridCell(self, zoneId)
        return zoneId
        
    # Reparent and setLocation on av to DistributedOceanGrid
    def addObjectToGrid(self, av, useZoneId=None, startAutoUpdate=True):
        self.notify.debug("setting parent to grid %s" % self)

        # track this object
        self.gridObjects[av.doId] = av

        # Put the avatar on the grid
        self.handleChildCellChange(av, useZoneId)

        if (not self.updateTaskStarted) and startAutoUpdate:
            self.startUpdateGridTask()

    def removeObjectFromGrid(self, av):
        # TODO: WHAT LOCATION SHOULD WE SET THIS TO?
        #av.wrtReparentTo(self.parentNP)
        #av.setLocation(self.air.districtId, 1000)

        # Remove grid parent for this av
        self.gridObjects.pop(av.doId, None)

        # Stop task if there are no more av's being managed
        if not self.gridObjects:
            self.stopUpdateGridTask()

    #####################################################################
    # updateGridTask
    # This task is similar to the processVisibility task for the local client.
    # A couple differences:
    #  - we are not doing setInterest on the AI (that is a local client
    #    specific call).
    #  - we assume that the moving objects on the grid are parented to a
    #    gridParent, and are broadcasting their position relative to that
    #    gridParent.  This makes the task's math easy.  Just check to see
    #    when our position goes out of the current grid cell.  When it does,
    #    call handleChildCellChange

    def startUpdateGridTask(self):
        self.stopUpdateGridTask()
        self.updateTaskStarted = 1
        taskMgr.doMethodLater(1, self.updateGridTask, self.taskName("updateGridTask"))

    def stopUpdateGridTask(self):
        taskMgr.remove(self.taskName("updateGridTask"))
        self.updateTaskStarted = 0

    def updateGridTask(self, task=None):
        # Run through all grid objects and update their parents if needed
        missingObjs = []
        for avId in self.gridObjects.keys():
            av = self.gridObjects[avId]
            # handle a missing object after it is already gone?
            if (av.isEmpty()):
                task.setDelay(1.0)
                del self.gridObjects[avId]
                continue
            pos = av.getPos()
            if ((pos[0] < 0 or pos[1] < 0) or
                (pos[0] > self.cellWidth or pos[1] > self.cellWidth)):
                # we are out of the bounds of this current cell
                self.handleChildCellChange(av)
        # Do this every second, not every frame
        if (task):
            set_trace()
            task.setDelay(1.0)
        return Task.again

    def handleChildCellChange(self, child, cellId = None):
        # Calculate zone id
        # Get position of av relative to this grid
        if cellId is None:
            pos = child.getPos(self)
            zoneId = self.getZoneFromXYZ(pos)
        else:
            # zone already calculated, position of object might not
            # give the correct zone
            pos = None
            zoneId = cellId
            pass
        
        if not self.isValidZone(zoneId):
            self.notify.warning(
                "%s handleChildCellChange %s: not a valid zone (%s) for pos %s" %(self.doId, child.doId, zoneId, pos))                     
            return

        # broadcast the new location
        child.b_setLocation(self.doId, zoneId)

