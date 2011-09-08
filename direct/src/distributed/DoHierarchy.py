from direct.directnotify.DirectNotifyGlobal import directNotify

class Node:
    def __init__(self, doId):
        self.doId = doId
        self.children = {}
        pass

    def __repr__(self):
        return str(self)
    
    def __str__(self):
        return '<%s>' % (self.doId,)

    def __lt__(self, other):
        return self.doId < other.doId
    
    def __getitem__(self, zone):
        return self.children.get(zone)

    def addChild(self, zoneId, node):
        self.children.setdefault(zoneId, set()).add(node)
        pass

    def prettyStr(self, indent = 0, depth = 0):
        padding = ' '*indent
        out = '%s%d (%s)\n' % (padding, self.doId, `go(self.doId)`)
        depth -= 1
        if depth:
            for zone, nodes in sorted(self.children.items()):
                out += '  %s[%d]       <%s>\n' % (padding, zone, `go(self.doId)`)
                for node in sorted(nodes):
                    out += node.prettyStr(indent+4, depth)
                    pass
                pass
            pass
        return out

    def getChildren(self, zone = None):
        if zone is None:
            return self.children
        else:
            return self.children.get(zone)
        pass
    
    
class DoHierarchy:
    """
    This table has been a source of memory leaks, with DoIds getting left in the table indefinitely.
    DoHierarchy guards access to the table and ensures correctness.
    """
    notify = directNotify.newCategory("DoHierarchy")

    def __init__(self):
        # parentId->zoneId->set(child DoIds)
        self._table = {}
        self._allDoIds = set()

    def isEmpty(self):
        assert ((len(self._table) == 0) == (len(self._allDoIds) == 0))
        return len(self._table) == 0 and len(self._allDoIds) == 0

    def __len__(self):
        return len(self._allDoIds)

    def clear(self):
        assert self.notify.debugCall()
        self._table = {}
        self._allDoIds = set()

    def getDoIds(self, getDo, parentId, zoneId=None, classType=None):
        """
        Moved from DoCollectionManager
        ==============================
        parentId is any distributed object id.
        zoneId is a uint32, defaults to None (all zones).  Try zone 2 if
            you're not sure which zone to use (0 is a bad/null zone and
            1 has had reserved use in the past as a no messages zone, while
            2 has traditionally been a global, uber, misc stuff zone).
        dclassType is a distributed class type filter, defaults
            to None (no filter).

        If dclassName is None then all objects in the zone are returned;
        otherwise the list is filtered to only include objects of that type.
        """
        parent=self._table.get(parentId)
        if parent is None:
            return []
        if zoneId is None:
            r = []
            for zone in parent.values():
                for obj in zone:
                    r.append(obj)
        else:
            r = parent.get(zoneId, [])
        if classType is not None:
            a = []
            for doId in r:
                obj = getDo(doId)
                if isinstance(obj, classType):
                    a.append(doId)
            r = a
        return r

    def storeObjectLocation(self, do, parentId, zoneId):
        doId = do.doId
        if doId in self._allDoIds:
            self.notify.error(
                'storeObjectLocation(%s %s) already in _allDoIds; duplicate generate()? or didn\'t clean up previous instance of DO?' % (
                do.__class__.__name__, do.doId))
        parentZoneDict = self._table.setdefault(parentId, {})
        zoneDoSet = parentZoneDict.setdefault(zoneId, set())
        zoneDoSet.add(doId)
        self._allDoIds.add(doId)
        self.notify.debug('storeObjectLocation: %s(%s) @ (%s, %s)' % (
            do.__class__.__name__, doId, parentId, zoneId))

    def deleteObjectLocation(self, do, parentId, zoneId):
        doId = do.doId
        if doId not in self._allDoIds:
            self.notify.error(
                'deleteObjectLocation(%s %s) not in _allDoIds; duplicate delete()? or invalid previous location on a new object?' % (
                do.__class__.__name__, do.doId))
        # jbutler: temp hack to get by the assert, this will be fixed soon
        if (doId not in self._allDoIds):
            return
        parentZoneDict = self._table.get(parentId)
        if parentZoneDict is not None:
            zoneDoSet = parentZoneDict.get(zoneId)
            if zoneDoSet is not None:
                if doId in zoneDoSet:
                    zoneDoSet.remove(doId)
                    self._allDoIds.remove(doId)
                    self.notify.debug('deleteObjectLocation: %s(%s) @ (%s, %s)' % (
                        do.__class__.__name__, doId, parentId, zoneId))
                    if len(zoneDoSet) == 0:
                        del parentZoneDict[zoneId]
                        if len(parentZoneDict) == 0:
                            del self._table[parentId]
                else:
                    self.notify.error(
                        "deleteObjectLocation: objId: %s not found" % doId)
            else:
                self.notify.error(
                    "deleteObjectLocation: zoneId: %s not found" % zoneId)
        else:
            self.notify.error(
                "deleteObjectLocation: parentId: %s not found" % parentId)

    def genTree(self):
        """
        Returns a list of top level nodes and a dictionary of doIds to nodes.

        The nodes contain links each doId's children nodes.
        """
        topnodes = {}
        nodes = {}

        # Build the tree(s) by iterating through _table
        # adding child nodes to their parents
        for doId, zones in self._table.items():
            node = nodes.setdefault(doId, Node(doId))
            topnodes[doId] = node
            for zone, doIds in zones.items():
                for doId in doIds:
                    node.addChild(zone,nodes.setdefault(doId, Node(doId)))
                    pass
                pass
            pass

        # Determine the root(s) of the tree(s)
        # by culling out any nodes found to be a
        # child of another
        for doId, zones in self._table.items():
            for zone, doIds in zones.items():
                for doId in doIds:
                    topnodes.pop(doId, None)
                    pass
                pass
            pass

        return topnodes.values(),nodes
    

    def printTree(self, doId = None, depth = 0):
        topnodes, nodes =  self.genTree()

        if doId:
            if doId in nodes:
                nodes = [nodes[doId]]
            else:
                nodes = []
                pass
            pass
        else:
            nodes = topnodes
            pass

        for node in nodes:
            print node.prettyStr(depth = depth)
            pass
        pass
    
    def getChildren(self, doId):
        topnodes, nodes =  self.genTree()
