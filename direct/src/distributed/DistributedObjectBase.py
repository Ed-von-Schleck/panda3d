
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
            print out "doId(parentId, zoneId) className"
            """
            spaces=' '*(indent+2)
            try:
                print "%s%s:"%(
                    ' '*indent, self.__class__.__name__)
                print "%sfrom DistributedObject doId:%s, parent:%s, zone:%s"%(
                    spaces,
                    self.doId, self.parentId, self.zoneId),
            except Exception, e: print "%serror printing status"%(spaces,), e

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
        """
        A new child has just setLocation beneath us.  Give us a
        chance to run code when a new child sets location to us. For
        example, we may want to scene graph reparent the child to
        some subnode we own.
        """
        assert self.notify.debugCall()
        # Inheritors should override
        pass

    def handleChildArriveZone(self, childObj, zoneId):
        """
        A child has just changed zones beneath us with setLocation.
        Give us a chance to run code when an existing child sets
        location to us. For example, we may want to scene graph
        reparent the child to some subnode we own.
        """
        assert self.notify.debugCall()
        # Inheritors should override
        pass

    def handleChildLeave(self, childObj, zoneId):
        """
        A child is about to setLocation away from us.  Give us a
        chance to run code just before a child sets location away from us.
        """
        assert self.notify.debugCall()
        # Inheritors should override
        pass

    def handleChildLeaveZone(self, childObj, zoneId):
        """
        A child is about to setLocation to another zone beneath us.
        Give us a chance to run code just before a child sets
        location to that zone.
        """
        assert self.notify.debugCall()
        # Inheritors should override
        pass

    def handleQueryObjectChildrenLocalDone(self, context):
        assert self.notify.debugCall()
        # Inheritors should override
        pass

    def getParentObj(self):
        if self.parentId is None:
            return None
        return self.cr.doId2do.get(self.parentId)

    def hasParentingRules(self):
        return self.dclass.getFieldByName('setParentingRules') != None

    def getAutoInterests(self):
        
        # returns the sub-zones under this object that are automatically
        # opened for us by the server.
        # have we already cached it?
        def _getAutoInterests(cls):
            # returns set of auto-interests for this class and all derived
            # have we already computed this class's autoInterests?
            if 'autoInterests' in cls.__dict__:
                autoInterests = cls.autoInterests
            else:
                autoInterests = set()
                # grab autoInterests from base classes
                for base in cls.__bases__:
                    autoInterests.update(_getAutoInterests(base))
                # grab autoInterests from this class
                repos = getattr(self,'cr', getattr(self, 'air'))
                if cls.__name__ in repos.dclassesByName:
                    dclass = repos.dclassesByName[cls.__name__]
                    field = dclass.getFieldByName('AutoInterest')
                    if field is not None:
                        from pandac.PandaModules import DCPacker
                        p = DCPacker()
                        p.setUnpackData(field.getDefaultValue())
                        len = p.rawUnpackUint16()/4
                        for i in xrange(len):
                            zone = int(p.rawUnpackUint32())
                            autoInterests.add(zone)
                    autoInterests.update(autoInterests)
                    cls.autoInterests = autoInterests
            return set(autoInterests)
        autoInterests = _getAutoInterests(self.__class__)
        # if the server starts supporting multiple auto-interest per class, this check
        # should be removed
        if len(autoInterests) > 1:
            self.notify.error(
                'only one auto-interest allowed per DC class, %s has %s autoInterests (%s)' %
                (self.dclass.getName(), len(autoInterests), list(autoInterests)))
        _getAutoInterests = None
        return list(autoInterests)

