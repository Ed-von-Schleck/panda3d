
from direct.directnotify.DirectNotifyGlobal import directNotify
from direct.distributed.DistributedObjectBase import DistributedObjectBase

#from PyDatagram import PyDatagram
#from PyDatagramIterator import PyDatagramIterator

# Values for DistributedObjectOV.activeState
# these should match DistributedObject.ES*

ESNew          = 1
ESDeleted      = 2
ESDisabling    = 3
ESDisabled     = 4  # values here and lower are considered "disabled"
ESGenerating   = 5  # values here and greater are considered "generated"
ESGenerated    = 6

class DistributedObjectOV(DistributedObjectBase):
    """
    Implementation of the 'owner view' (OV) of a distributed object;
    """
    notify = directNotify.newCategory("DistributedObjectOV")

    def __init__(self, cr):
        assert self.notify.debugStateCall(self)
        try:
            self.DistributedObjectOV_initialized
        except:
            self.DistributedObjectOV_initialized = 1
            DistributedObjectBase.__init__(self, cr)

            # This count tells whether the object can be deleted right away,
            # or not.
            self.delayDeleteCount = 0
            # This flag tells whether a delete has been requested on this
            # object.
            self.deleteImminent = 0

            # Keep track of our state as a distributed object.  This
            # is only trustworthy if the inheriting class properly
            # calls up the chain for disable() and generate().
            self.activeState = ESNew

    if __debug__:
        def status(self, indent=0):
            """
            print out "doId(parentId, zoneId) className"
                and conditionally show generated, disabled
            """
            spaces=' '*(indent+2)
            try:
                print "%s%s:"%(
                    ' '*indent, self.__class__.__name__)
                print "%sfrom DistributedObjectOV doId:%s, parent:%s, zone:%s"%(
                    spaces,
                    self.doId, self.parentId, self.zoneId),
                flags=[]
                if self.activeState == ESGenerated:
                    flags.append("generated")
                if self.activeState < ESGenerating:
                    flags.append("disabled")
                if len(flags):
                    print "(%s)"%(" ".join(flags),),
                print
            except Exception, e: print "%serror printing status"%(spaces,), e

    def deleteOrDelay(self):
        if self.delayDeleteCount > 0:
            self.deleteImminent = 1
        else:
            self.disableAnnounceAndDelete()

    def delayDelete(self, flag):
        # Flag should be 0 or 1, meaning increment or decrement count
        # Also see DelayDelete.py

        if (flag == 1):
            self.delayDeleteCount += 1
        elif (flag == 0):
            self.delayDeleteCount -= 1
        else:
            self.notify.error("Invalid flag passed to delayDelete: " + str(flag))

        if (self.delayDeleteCount < 0):
            self.notify.error("Somebody decremented delayDelete for doId %s without incrementing"
                              % (self.doId))
        elif (self.delayDeleteCount == 0):
            assert self.notify.debug(
                "delayDeleteCount for doId %s now 0" %
                (self.doId))
            if self.deleteImminent:
                assert self.notify.debug(
                    "delayDeleteCount for doId %s -- deleteImminent" %
                    (self.doId))
                self.disableAnnounceAndDelete()
        else:
            self.notify.debug(
                "delayDeleteCount for doId %s now %s" %
                (self.doId, self.delayDeleteCount))

        # Return the count just for kicks
        return self.delayDeleteCount

    def disableAnnounceAndDelete(self):
        self.disableAndAnnounce()
        self.delete()

    def disableAndAnnounce(self):
        """
        Inheritors should *not* redefine this function.
        """
        # We must send the disable announce message *before* we
        # actually disable the object.  That way, the various cleanup
        # tasks can run first and take care of restoring the object to
        # a normal, nondisabled state; and *then* the disable function
        # can properly disable it (for instance, by parenting it to
        # hidden).
        if self.activeState != ESDisabled:
            self.activeState = ESDisabling
            messenger.send(self.uniqueName("disable"))
            self.disable()

    def announceGenerate(self):
        """
        Sends a message to the world after the object has been
        generated and all of its required fields filled in.
        """
        assert self.notify.debug('announceGenerate(): %s' % (self.doId))
        if self.activeState != ESGenerated:
            self.activeState = ESGenerated
            messenger.send(self.uniqueName("generate"), [self])

    def disable(self):
        """
        Inheritors should redefine this to take appropriate action on disable
        """
        assert self.notify.debug('disable(): %s' % (self.doId))
        if self.activeState != ESDisabled:
            self.activeState = ESDisabled

    def isDisabled(self):
        """
        Returns true if the object has been disabled and/or deleted,
        or if it is brand new and hasn't yet been generated.
        """
        return (self.activeState < ESGenerating)

    def isGenerated(self):
        """
        Returns true if the object has been fully generated by now,
        and not yet disabled.
        """
        assert self.notify.debugStateCall(self)
        return (self.activeState == ESGenerated)

    def delete(self):
        """
        Inheritors should redefine this to take appropriate action on delete
        """
        assert self.notify.debug('delete(): %s' % (self.doId))
        try:
            self.DistributedObjectOV_deleted
        except:
            self.DistributedObjectOV_deleted = 1
            self.cr = None
            self.dclass = None

    def generate(self):
        """
        Inheritors should redefine this to take appropriate action on generate
        """
        assert self.notify.debugStateCall(self)
        self.activeState = ESGenerating
        # this has already been set at this point
        #self.cr.storeObjectLocation(self.doId, self.parentId, self.zoneId)

    def generateInit(self):
        """
        This method is called when the DistributedObjectOV is first introduced
        to the world... Not when it is pulled from the cache.
        """
        self.activeState = ESGenerating

    def getDoId(self):
        """
        Return the distributed object id
        """
        return self.doId

    def updateRequiredFields(self, dclass, di):
        dclass.receiveUpdateBroadcastRequired(self, di)
        self.announceGenerate()

    def updateAllRequiredFields(self, dclass, di):
        dclass.receiveUpdateAllRequired(self, di)
        self.announceGenerate()

    def updateRequiredOtherFields(self, dclass, di):
        # First, update the required fields
        dclass.receiveUpdateBroadcastRequiredOwner(self, di)

        # Announce generate after updating all the required fields,
        # but before we update the non-required fields.
        self.announceGenerate()

        dclass.receiveUpdateOther(self, di)

    def getCacheable(self):
        return False

    def sendUpdate(self, fieldName, args = [], sendToId = None):
        if self.cr:
            dg = self.dclass.clientFormatUpdate(
                fieldName, sendToId or self.doId, args)
            self.cr.send(dg)
        else:
            self.notify.warning("sendUpdate failed, because self.cr is not set")

    def taskName(self, taskString):
        return ('%s-%s-OV' % (taskString, self.getDoId()))

    def uniqueName(self, idString):
        return ('%s-%s-OV' % (idString, self.getDoId()))
