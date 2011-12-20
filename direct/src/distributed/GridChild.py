from direct.distributed.DistributedSmoothNodeBase import DistributedSmoothNodeBase
from direct.distributed.GridParent import GridParent
from pandac.PandaModules import EmbeddedValue

class GridChild:
    """
    Any object that expects to be parented to a grid should inherit from this.
    It works with GridParent to manage its grid cell hierarchy in the scenegraph.
    """
    def __init__(self):
        try:
            self.__initiallized
        except AttributeError:
            self._gridParent = None

            self._gridInterestEnabled = False
            self._gridInterests = {}

    def delete(self):
        self.__setGridParent(None)
        self.enableGridInterest(False)

    @report(types = ['args'], dConfigParam = 'smoothnode')
    def setGridCell(self, grid, zoneId, updateInterest=False):
        if not hasattr(self,'getParent'):
            return
        if grid is None:
            self.__setGridParent(None)
            self.__clearGridInterest()
        else:
            hadParent = True
            if not self._gridParent:
                hadParent = False
                self.__setGridParent(GridParent(self))

            # Does the (wrt)ReparentTo() operation
            self._gridParent.setGridCell(grid, zoneId)

            # Moves the grid interest along with this child
            if updateInterest or hadParent == False:
                self.updateGridInterest(grid, zoneId)

    def updateGridInterest(self, grid, zoneId):
        # add additional grid interests (sometimes we want more
        # than just one)
        #if self._gridInterestEnabled:
        self.__setGridInterest(grid, zoneId)
        
    def enableGridInterest(self, enabled = True):
        self._gridInterestEnabled = enabled
        if enabled and self.isOnAGrid():
            # enable all grid interests I may have
            for currGridId, interestInfo in self._gridInterests.items():
                currGrid = getBase().getRepository().doId2do.get(currGridId)
                if currGrid:
                    self.__setGridInterest(currGrid, interestInfo[1])
                else:
                    self.notify.warning("unknown grid interest %s"%currGridId)
        else:
            self.__clearGridInterest()

    def isOnAGrid(self):
        return self._gridParent is not None

    def getGrid(self):
        if self._gridParent:
            return self._gridParent.getGrid()
        else:
            return None

    def getGridZone(self):
        if self._gridParent:
            return self._gridParent.getGridZone()
        else:
            return None
        
    def __setGridParent(self, gridParent):
        if self._gridParent and self._gridParent is not gridParent:
            self._gridParent.delete()
        self._gridParent = gridParent

    
    def __setGridInterest(self, grid, zoneId):
        assert not self.cr.noNewInterests()
        if self.cr.noNewInterests():
            self.notify.warning(
                'startProcessVisibility(%s): tried to open a new interest during logout'
                % self.doId)
            return

        # check zoneId
        if zoneId < 0 or not grid.isValidZone(zoneId):
            # make sure zoneId is valid
            self.notify.warning("__setGridInterest: given bad zoneId [%s] [%s] [%s] [%s] [%s]"%(
                zoneId, grid, self, str(self.getPos(grid)), str(self._gridInterests)))
            printStack()
            # try to catch this case in dev
            assert 0, "Invalid zoneId"
            return

        gridDoId = grid.getDoId()
        existingInterest = self._gridInterests.get(gridDoId)
        if self._gridInterestEnabled:
            if existingInterest and existingInterest[0]:
                self.cr.alterInterest(existingInterest[0],
                                      grid.getDoId(), zoneId)
                existingInterest[1] = zoneId
            else:
                newInterest = self.cr.addTaggedInterest(gridDoId, zoneId,
                                                        self.cr.ITAG_GAME,
                                                        self.uniqueName('gridvis'))
                self._gridInterests[gridDoId] = [newInterest,zoneId]
        else:
            # indicate we want this grid interest once gridInterestEnabled is True
            # this is needed on AI so location gets updated as child moves around
            self._gridInterests[gridDoId] = [None,zoneId]

    def getGridInterestIds(self):
        return self._gridInterests.keys()

    def getGridInterestReference(self,gridDoId):
        return self._gridInterests.get(gridDoId,[None,None])[0]
    def getGridInterestZoneId(self,gridDoId):
        return self._gridInterests.get(gridDoId,[None,None])[1]

    def clearinGridInterest(self):
        pass

    def __clearGridInterest(self):
        #if self._gridInterestEnabled:
        self.clearinGridInterest()
        for currGridId, interestInfo in self._gridInterests.items():
            if interestInfo[0]:
                self.cr.removeTaggedInterest(interestInfo[0])
        self._gridInterests = {}

    #--------------------------------------------------------------------------
    # Function:   verify position of child is within any limits
    # Parameters: none
    # Changes:    none
    # Returns:    False if no change made
    #--------------------------------------------------------------------------
    def checkPosition(self):
        return False





class SmoothGridChild(GridChild):
    """
    SmoothNodes have a special requirement in that they need to send
    their current cell along with their telemetry data stream. This
    allows the distributed receiving objects to update their grid parent
    according to this value, rather than the setLocation() data.

    Use this instead of GridNode when you expect this object to send its
    telemetry data out.
    """
    def __init__(self):
        GridChild.__init__(self)
        assert isinstance(self, DistributedSmoothNodeBase), \
               'All GridChild objects must be instances of DistributedSmoothNodeBase'

    @report(types = ['args'], dConfigParam = 'smoothnode')
    def setGridCell(self, grid, zoneId, updateInterest=False):
        GridChild.setGridCell(self, grid, zoneId, updateInterest=updateInterest)
        if grid and self.isGenerated(): # we get our cnode in DistributedSmoothNodeBase.generate()
            self.cnode.setEmbeddedVal(zoneId)

    @report(types = ['args'], dConfigParam = 'smoothnode')
    def updateCurrentZone(self):
        if self.isOnAGrid():
            currGrid = self.getGrid()
            # check actual zone matches zone I have interest in
            zoneId = currGrid.getZoneFromXYZ(self.getPos(currGrid))
            existingInterest = self._gridInterests.get(currGrid.doId)
            if existingInterest and existingInterest[1] != zoneId:
                # we want to update interest here since actual position
                # of grid child is now in the new zone
                self.updateGridInterest(currGrid, zoneId)

    def transformTelemetry(self, x, y, z, h, p, r, e):
        # We don't really need to transform telemetry, but
        # we do update our grid cell such that the new 
        # telemetry is correct now.
        # We do this instead of overriding setSmPosHprE()
        # because we're a mixin class.
        if self.isOnAGrid():
            self.setGridCell(self.getGrid(), e) # causes a wrtReparent() which updates 
                                                # all previous smooth positions
        return x, y, z, h, p, r
