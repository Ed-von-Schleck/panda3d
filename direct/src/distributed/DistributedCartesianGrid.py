
from pandac.PandaModules import *
from direct.interval.IntervalGlobal import *

from direct.distributed.DistributedNode import DistributedNode
from direct.task import Task
from direct.gui import DirectGuiGlobals
from direct.showbase.PythonUtil import report
from direct.distributed.StagedObject import StagedObject

if __debug__:
    # For grid drawing
    from direct.directtools.DirectGeometry import *
    from direct.showbase.PythonUtil import randFloat

from CartesianGridBase import CartesianGridBase

# increase this number if you want to visualize the grid lines
# above water level
GRID_Z_OFFSET = 0.0

class DistributedCartesianGrid(DistributedNode, CartesianGridBase, StagedObject):
    notify = directNotify.newCategory("DistributedCartesianGrid")
    notify.setDebug(0)
    
    VisualizeGrid = config.GetBool("visualize-cartesian-grid", 0)

    def __init__(self, cr):
        # Let the derived classes instantiate the NodePath
        DistributedNode.__init__(self, cr)
        CartesianGridBase.__init__(self)
        StagedObject.__init__(self, StagedObject.OFF)

        # Do we have grid lines visualized?
        if __debug__:
            self.haveGridLines = 0

    def disable(self):
        DistributedNode.disable(self)
        CartesianGridBase.disable(self)

    def isGrid(self):
        return CartesianGridBase.isGrid(self)

    def setParentingRules(self, style, rule):
        CartesianGridBase.setParentingRules(self, style, rule)
        if __debug__:
            if self.VisualizeGrid:
                self.visualizeGrid()

    def handleChildArrive(self, child, zoneId):
        DistributedNode.handleChildArrive(self, child, zoneId)
        CartesianGridBase.handleChildArrive(self, child, zoneId)

    def handleChildArriveZone(self, child, zoneId):
        DistributedNode.handleChildArriveZone(self, child, zoneId)
        CartesianGridBase.handleChildArriveZone(self, child, zoneId)

    def handleChildLeave(self, child, zoneId):
        DistributedNode.handleChildLeave(self, child, zoneId)
        CartesianGridBase.handleChildLeave(self, child, zoneId)
        pass
            
    def handleOffStage(self):
        StagedObject.handleOffStage(self)
        pass
    
    def handleOnStage(self, av = None):
        StagedObject.handleOnStage(self)
        if av:
            av.enableGridInterest()
            pass
        pass
    
    ##################################################
    # Visualization Tools
    ##################################################

    if __debug__:

        def initializeGridLines(self):
            # Grid Lines
            self.gridColor = VBase4(0.4 + randFloat(0.4),
                                    0.4 + randFloat(0.4),
                                    0.4 + randFloat(0.4),
                                    1)
            # A Dark version of the grid color
            color = self.gridColor * 0.5
            color.setW(1)

            self.lines = self.attachNewNode('gridLines')
            self.minorLines = LineNodePath(self.lines)
            self.minorLines.lineNode.setName('minorLines')
            self.minorLines.setColor(color)
            self.minorLines.setThickness(1)

            self.majorLines = LineNodePath(self.lines)
            self.majorLines.lineNode.setName('majorLines')
            self.majorLines.setColor(color)
            self.majorLines.setThickness(5)

            self.centerLines = LineNodePath(self.lines)
            self.centerLines.lineNode.setName('centerLines')
            self.centerLines.setColor(VBase4(1, 0, 0, 0))
            self.centerLines.setThickness(3)

            # Load up grid parts to initialize grid object
            # Polygon used to mark grid plane
            # self.gridBack = loader.loadModel('models/misc/gridBack')
            # self.gridBack.reparentTo(self)
            # self.gridBack.setColor(0.2, 0.2, 0.2, 0.5)

            self.cellLabelParent = None
            self.markerParent = None
            self.haveGridLines = 1

        def updateGrid(self):
            # Update grid lines based upon current grid spacing and grid size
            # First reset existing grid lines
            self.minorLines.reset()
            self.majorLines.reset()
            self.centerLines.reset()
            # Now redraw lines
            numLines = self.gridSize
            scaledSize = numLines * self.cellWidth / 2.0
            center = self.centerLines
            minor = self.minorLines
            major = self.majorLines
            cw = self.cellWidth
            dx = cw * self.gridSize * .5
            for i in range(numLines+1):
                icw = i * cw - dx
                if i == numLines/2:
                    center.moveTo(icw, -scaledSize, GRID_Z_OFFSET)
                    center.drawTo(icw, scaledSize, GRID_Z_OFFSET)
                    center.moveTo(-scaledSize, icw, GRID_Z_OFFSET)
                    center.drawTo(scaledSize, icw, GRID_Z_OFFSET)
                else:
                    if (i % 5) == 0:
                        major.moveTo(icw, -scaledSize, GRID_Z_OFFSET)
                        major.drawTo(icw, scaledSize, GRID_Z_OFFSET)
                        major.moveTo(-scaledSize, icw, GRID_Z_OFFSET)
                        major.drawTo(scaledSize, icw, GRID_Z_OFFSET)
                    else:
                        minor.moveTo(icw, -scaledSize, GRID_Z_OFFSET)
                        minor.drawTo(icw, scaledSize, GRID_Z_OFFSET)
                        minor.moveTo(-scaledSize, icw, GRID_Z_OFFSET)
                        minor.drawTo(scaledSize, icw, GRID_Z_OFFSET)
            center.create()
            minor.create()
            major.create()
            # self.gridBack.setScale(scaledSize)
            self.labelCells()

        def labelCells(self):
            if self.cellLabelParent:
                self.cellLabelParent.removeNode()
            self.cellLabelParent = self.attachNewNode('cellLabels')
            cw = self.cellWidth
            scale = cw / 10.0
            dx = cw * self.gridSize * .5
            font = DirectGuiGlobals.getDefaultFont()
            color = self.gridColor
            for i in range(self.gridSize):
                for j in range(self.gridSize):
                    zoneId = self.startingZone + ((j * self.gridSize) + i)
                    zoneStr = str(zoneId)
                    textNode = TextNode(zoneStr)
                    textNode.setText(zoneStr)
                    textNode.setFont(font)
                    textNode.setTextColor(color)
                    textNode.setAlign(TextNode.ACenter)
                    genTextNode = textNode.generate()
                    textNodePath = self.cellLabelParent.attachNewNode(genTextNode)
                    # Place the text node in the center of the cell
                    textNodePath.setPosHprScale((i * cw - dx) + (cw * 0.5), # x
                                                (j * cw - dx) + (cw * 0.5), # y
                                                GRID_Z_OFFSET+3.0, # z
                                                # Lay them down flat
                                                0, -90, 0, # hpr
                                                scale, scale, scale)
            self.cellLabelParent.flattenLight()

        def markCells(self):
            if self.markerParent:
                self.markerParent.removeNode()
            self.markerParent = self.attachNewNode('markers')
            self.cellMarkers = []
            dx = self.cellWidth * self.gridSize * .5
            for i in range(self.gridSize):
                for j in range(self.gridSize):
                    marker = loader.loadModel("models/misc/smiley")
                    marker.reparentTo(self.markerParent)
                    marker.setPos(i * self.cellWidth - dx,
                                  j * self.cellWidth - dx,
                                  GRID_Z_OFFSET + 1.0)
                    marker.setScale(5)
                    self.cellMarkers.append(marker)

        def unmarkCells(self):
            if self.markerParent:
                self.markerParent.removeNode()
            self.markerParent = None

        def visualizeGrid(self):
            if not self.haveGridLines:
                self.initializeGridLines()
            self.updateGrid()
