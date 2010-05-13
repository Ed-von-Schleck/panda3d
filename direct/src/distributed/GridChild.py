from direct.distributed.DistributedSmoothNodeBase import DistributedSmoothNodeBase
from direct.distributed.GridParent import GridParent

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
            self._gridInterest = None
            pass
        pass

    def delete(self):
        self.__setGridParent(None)
        self.enableGridInterest(False)
        pass

    def setGridCell(self, grid, zoneId):
        if grid is None:
            self.__setGridParent(None)
            self.__clearGridInterest()
            pass
        else:
            if not self._gridParent:
                self.__setGridParent(GridParent(self))
                pass
            
            # Does the (wrt)ReparentTo() operation
            self._gridParent.setGridCell(grid, zoneId)

            # Moves the grid interest along with this child
            if self._gridInterestEnabled:
                self.__setGridInterest(grid, zoneId)
                pass
            pass
        pass

    def enableGridInterest(self, enabled = True):
        self._gridInterestEnabled = enabled
        if not enabled:
            self.__clearGridInterest()
            pass
        pass

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
            pass
        self._gridParent = gridParent
        pass

    
    def __setGridInterest(self, grid, zoneId):
        assert not self.cr.noNewInterests()
        if self.cr.noNewInterests():
            self.notify.warning(
                'startProcessVisibility(%s): tried to open a new interest during logout'
                % self.doId)
            return
        
        if self._gridInterest:
            self.cr.alterInterest(self._gridInterest,
                                  grid.getDoId(), zoneId)
        else:
            self._gridInterest = self.cr.addTaggedInterest(grid.getDoId(), zoneId,
                                                           self.cr.ITAG_GAME,
                                                           self.uniqueName('gridvis'))
            pass
        pass

    def __clearGridInterest(self):
        if self._gridInterest:
            self.cr.removeTaggedInterest(self.gridInterest)
            self._gridInterest = None
            pass
        
                




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

    def setGridCell(self, grid, zoneId):
        GridChild.setGridCell(self, grid, zoneId)
        if grid is not None and self.isGenerated(): # we get our cnode in DSmoothNodeBase.generate()
            self.cnode.setEmbeddedVal(zoneId)
            pass
        pass

