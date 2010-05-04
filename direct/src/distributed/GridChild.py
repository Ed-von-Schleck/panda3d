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
            pass
        pass

    def __setGridParent(self, gridParent):
        if self._gridParent and self._gridParent is not gridParent:
            self._gridParent.delete()
            pass
        self._gridParent = gridParent
        pass

    def setGridCell(self, grid, zoneId):
        try:
            if self.uniqueId == '1240950699.65ian':
                set_trace()
        except:
            pass
        if grid is None:
            self.__setGridParent(None)
            pass
        else:
            if not self._gridParent:
                self.__setGridParent(GridParent(self))
                pass
            
            # Does the (wrt)ReparentTo() operation
            self._gridParent.setGridCell(grid, zoneId)
            pass
        pass

    def isOnAGrid(self):
        return self._gridParent is not None
    
    def delete(self):
        self.__setGridParent(None)
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
    
