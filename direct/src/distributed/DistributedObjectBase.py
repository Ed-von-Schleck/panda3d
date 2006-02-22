
from direct.showbase.DirectObject import DirectObject
#from direct.directnotify.DirectNotifyGlobal import directNotify

class DistributedObjectBase(DirectObject):
    """
    The Distributed Object class is the base class for all network based
    (i.e. distributed) objects.  These will usually (always?) have a
    dclass entry in a *.dc file.
    """
    notify = directNotify.newCategory("DistributedObjectBase")

    def __init__(self, cr):
        assert self.notify.debugStateCall(self)
        self.cr = cr
        self.children = {}
        self.parentId = None
        self.zoneId = None

    if __debug__:
        def status(self, indent=0):
            """
            print out "doId(parentId,zoneId) className"
            """
            spaces=' '*(indent+2)
            try:
                print "%s%s:"%(
                    ' '*indent, self.__class__.__name__)
                print "%sfrom DistributedObject doId:%s, parent:%s, zone:%s"%(
                    spaces, 
                    self.doId, self.parentId, self.zoneId),
            except Exception, e: print "%serror printing status"%(spaces,), e

    def doGenerate(self, parentId, zoneId):
        # package generate() call with setLocation()
        # set the location up but don't call the handler yet. The effect is
        # that objects know their location in generate() and can reference their
        # parent, but setLocation() doesn't get called until after generate()
        self.cr.storeObjectLocation(self.doId, parentId, zoneId)
        self.generate()
        # now call the handler
        self.setLocation(parentId, zoneId)

    def getRepository(self):
        return self.cr

    def getDoTable(self):
        return self.getRepository().doId2do

    def getRelatedObjectMgr(self):
        return self.getRepository().relatedObjectMgr

    def _addToDoTables(self):
        self.getRepository().addDOToTables(self)

    def getObject(self, doIds, callback):
        # calls callback when all objects have been found; may be called
        # immediately or at some point in the future
        # always returns None
        return self.getRepository().getObject(doIds, callback)

    def getOwnerView(self, doIds, callback):
        # calls callback when all ownerViews have been found; may be
        # called immediately or at some point in the future
        # always returns None
        return self.getRepository().getOwnerView(doIds, callback)

    def getParentObject(self):
        # immediately returns parent object
        return self.getObjectNow(self.parentId)

    def getObjectNow(self, doId):
        # immediately returns DO or None
        return self.getRepository().getObjectNow(doId)

    def getOwnerViewNow(self, doId):
        # immediately returns OV or None
        return self.getRepository().getOwnerViewNow(doId)

    def getLocation(self):
        try:
            if self.parentId == 0 and self.zoneId == 0:
                return None
            # This is a -1 stuffed into a uint32
            if self.parentId == 0xffffffff and self.zoneId == 0xffffffff:
                return None
            return (self.parentId, self.zoneId)
        except AttributeError:
            return None

    def handleChildArrive(self, childObj, zoneId):
        self.notify.debugCall()
        # A new child has just setLocation beneath us.  Give us a
        # chance to run code when a new child sets location to us. For
        # example, we may want to scene graph reparent the child to
        # some subnode we own.
        ## zone=self.children.setdefault(zoneId, {})
        ## zone[childObj.doId]=childObj

        # Inheritors should override
        pass

    def handleChildLeave(self, childObj, zoneId):
        self.notify.debugCall()
        # A child is about to setLocation away from us.  Give us a
        # chance to run code just before a child sets location away from us.
        ## zone=self.children[zoneId]
        ## del zone[childObj.doId]
        ## if not len(zone):
        ##     del self.children[zoneId]

        # Inheritors should override
        pass

    def getParentObj(self):
        if self.parentId is None:
            return None
        return self.cr.doId2do.get(self.parentId)
