from pandac.PandaModules import *


class GridParent:
    """
    This class manages a node between the GridChild and the Grid so that the
    GridChild can multiplex its telemetry from the cell (scenegraph parent)
    rather than the grid (distributed parent).
    """
    
    # this lets GridParents share CellOrigins
    GridZone2CellOrigin = {} # The nodes beneath the grid
    GridZone2count = {}      # Reference counts to know when to delete them
    
    @staticmethod
    def getCellOrigin(grid, zoneId):
        tup = (grid, zoneId)
        if tup not in GridParent.GridZone2count:
            GridParent.GridZone2count[tup] = 0
            # For readability when debugging, append the zone to the name
            GridParent.GridZone2CellOrigin[tup] = grid.attachNewNode("cellOrigin-%s" % zoneId)
            # Get grid cell origin
            cellPos = grid.getZoneCellOrigin(zoneId)
            # Set the gridNode's position
            GridParent.GridZone2CellOrigin[tup].setPos(*cellPos)
        GridParent.GridZone2count[tup] += 1
        return GridParent.GridZone2CellOrigin[tup]
    
    @staticmethod
    def releaseCellOrigin(grid, zoneId):
        tup = (grid, zoneId)
        GridParent.GridZone2count[tup] -= 1
        if GridParent.GridZone2count[tup] == 0:
            GridParent.GridZone2count.pop(tup)
            GridParent.GridZone2CellOrigin.pop(tup).removeNode()

    def __init__(self, child):
        # The object on the grid will need to broadcast his position relative to
        # his current grid cell in order to use 16 bit telemetry.  To do this, we
        # will have a node attached to the grid cell origin, and the object will
        # wrtReparent himself to it when crossing into that grid cell.
        # We don't need to create a node for each cell origin.  We just need two
        # nodes:  one that we are currently parented to, and the other that we will
        # wrtReparentTo.  Just before wrtReparenting to the new node, set it's
        # position to the new grid cell origin.
        self._child = child
        self._grid = None
        self._zoneId = None
        
        # NOTE: this node gets renamed when it is put on a zone, so if you
        # are looking for it by name, try cellOrigin*.
        self.ownCellOrigin = NodePath("cellOrigin")
        self.cellOrigin = self.ownCellOrigin
        pass

    def delete(self):
        if self._child and \
           self._child.getParent() == self.cellOrigin:
            self._child.detachNode()
        self._child = None

        # Remove the gridNodes
        if self.ownCellOrigin is not None:
            self.ownCellOrigin.removeNode()
            self.ownCellOrigin = None

        if self._grid is not None:
            self.releaseCellOrigin(self._grid, self._zoneId)
            self._grid = None
            self._zoneId = None

    def setGridCell(self, grid, zoneId):
        # This will wrtReparent the child to a new grid node in the scenegraph
        # If the child currently has no parent, it will simply be reparented to
        # the new node.

        newCellOrigin = self.getCellOrigin(grid, zoneId)
        if self._child.getParent():
            self._child.wrtReparentTo(newCellOrigin)
        else:
            self._child.reparentTo(newCellOrigin)
            pass

        if self._grid is not None:
            self.releaseCellOrigin(self._grid, self._zoneId)
            pass

        self.cellOrigin = newCellOrigin
        self._grid = grid
        self._zoneId = zoneId
        pass
    
