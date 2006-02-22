"""
The DoInterestManager keeps track of which parent/zones that we currently
have interest in.  When you want to "look" into a zone you add an interest
to that zone.  When you want to get rid of, or ignore, the objects in that
zone, remove interest in that zone.

p.s. A great deal of this code is just code moved from ClientRepository.py.
"""

from pandac.PandaModules import *
from MsgTypes import *
from direct.showbase.PythonUtil import *
from direct.showbase import DirectObject
from PyDatagram import PyDatagram
from direct.directnotify.DirectNotifyGlobal import directNotify
from direct.fsm import FSM

# scope value for interest changes that have no complete event
NO_SCOPE = 0

class RFSM(FSM.FSM):
    # re-entrant FSM
    # it's not really re-entrant, it allows you to queue up operations
    # that happen after the current state transition
    def __init__(*args, **kwargs):
        apply(FSM.FSM.__init__, args, kwargs)
        # queue of functors to call after current transition
        args[0]._postTransFuncs = []
    def cleanup(self):
        self.callAfterTrans(Functor(FSM.FSM.cleanup, self))
    def callAfterTrans(self, functor):
        if not self.state:
            self._postTransFuncs.append(functor)
        else:
            functor()
    def _setStateCompleted(self):
        for functor in self._postTransFuncs:
            functor()
        self._postTransFuncs = []

class _ActiveInterestCompleteState(RFSM):
    defaultTransitions = {
        'Off'             : ['PendingComplete', 'Complete', ],
        'PendingComplete' : ['Off', 'PendingComplete', 'PendingHold', 'PendingChildren', 'Complete', ],
        'PendingHold'     : ['Off', 'PendingComplete', 'PendingChildren', 'Complete', ],
        'PendingChildren' : ['Off', 'PendingComplete', 'PendingHold', 'Complete', ],
        'Complete'        : ['Off', 'PendingComplete', 'Complete', ],
        }
    def __init__(self, notify, scopeId, interest):
        RFSM.__init__(self, interest.name+'_ActiveCompleteState')
        self._interest = interest
        self._scopeId = scopeId
        self._holdState = None
        self._notify = notify
        self.handleInterestOpen()

    # API ############
    def handleInterestOpen(self):
        if self._interest._hasCompleteEvent():
            state = 'PendingComplete'
        else:
            state = 'Complete'
        self.request(state)
    def handleInterestComplete(self, scopeId):
        assert self.state == 'PendingComplete', 'not expecting a complete...'
        if scopeId == self._scopeId:
            self._maybeComplete()
        
    def addHold(self):
        self._holdCount += 1
    def removeHold(self):
        assert hasattr(self, '_holdCount')
        assert self._holdCount > 0
        self._holdCount -= 1
        if not self._isHoldActive():
            self._maybeComplete()
    ##################

    def enterOff(self):
        if hasattr(self, '_holdCount'):
            del self._holdCount
    def exitOff(self):
        self._holdCount = 0
    def enterComplete(self):
        self._interest._sendCompleteEvent()

    def _isHoldActive(self):
        # returns True if we should hold the complete event
        if not hasattr(self, '_holdCount'):
            return False
        return self._holdCount > 0

    def _maybeComplete(self):
        # it might be time to complete
        assert self.state != 'Complete'
        if len(self._interest._children) > 0:
            state = 'PendingChildren'
        elif self._isHoldActive():
            state = 'PendingHold'
        else:
            state = 'Complete'
        if state != self.state:
            self.request(state)

class _ClosingInterestCompleteState(RFSM):
    defaultTransitions = {
        'Off'             : ['PendingComplete', ],
        'PendingComplete' : ['Off' ],
        # used to have 'Complete' state but it was causing FSM re-entry crashes,
        # and all it did was call the callback anyway
        }
    def __init__(self, notify, scopeId, callback, interest):
        RFSM.__init__(self, interest.name+'_ClosingCompleteState')
        self._scopeId = scopeId
        self._callback = callback
        self._notify = notify
        self._interest = interest
        self.request('PendingComplete')
    def handleInterestComplete(self, scopeId):
        assert self.state == 'PendingComplete'
        if scopeId == self._scopeId:
            self._interest._sendCompleteEvent()
            self._callback()

class Interest(RFSM):
    defaultTransitions = {
        'Off'     : ['Active',],
        'Active'  : ['Closing', 'Active',],
        'Closing' : ['Off',],
        }
    def __init__(self, doneCallback, notify, handle, description,
                 scopeId=None, event=None, parentInterest=None):
        RFSM.__init__(self, description)
        self._completeState = None
        self._holdIds = set()
        self._doneCallback = Functor(doneCallback, handle)
        self._notify = notify
        self.handleInterestChange(handle, scopeId, event, parentInterest, description)
    def destroy(self):
        del self._doneCallback
        self.cleanup()

    # API ############
    def isActive(self):
        return self.getCurrentOrNextState() == 'Active'

    def handleInterestChange(self, handle, scopeId=None, event=None, parent=None, description=None):
        # app wants to set this interest to this configuration
        assert (getattr(self, '_handle', handle) == handle)
        if description is None:
            assert hasattr(self, 'description')
            description = self._description
        self._notify.debug('handleInterestChange(): handle=%s, scopeId=%s, event=%s, parent=%s, description=%s' % (
            handle, scopeId, event, parent, description))
        self.request('Active', handle, description, scopeId, event, parent)
    def handleInterestComplete(self, scopeId):
        # the server just told us that this interest is complete
        assert self.state in ('Active', 'Closing')
        self._completeState.handleInterestComplete(scopeId)
    def handleInterestRemove(self, scopeId=None, event=None):
        # app is requesting that we remove this interest
        self._notify.debug(
            'removeInterest(): handle=%s, event=%s' % (self._handle, event))
        self.request('Closing', scopeId, event)

    def addHold(self):
        # hold up send of interest-complete event
        assert self.isActive()
        self._completeState.addHold()
    def removeHold(self):
        assert self.isActive()
        self._completeState.removeHold()
    def isHoldActive(self):
        if self.isActive():
            return self._completeState._isHoldActive()
        else:
            return False

    def addChild(self, child):
        assert self.isActive()
        assert child not in self._children
        self._children.add(child)
    def removeChild(self, child):
        assert self.isActive()
        assert child in self._children
        self._children.remove(child)

    def _handleChildCompleted(self):
        if self.isActive():
            self._completeState._maybeComplete()

    ##################

    def enterActive(self, handle, description, scopeId, event, parent):
        self._handle = handle
        self._description = description
        if scopeId is None:
            scopeId = NO_SCOPE
        self._scopeId = scopeId
        self._event = event
        self._parent = parent
        if self._parent is not None:
            self._parent.addChild(self)
        self._children = set()
        self._completeState = _ActiveInterestCompleteState(self._notify, self._scopeId, self)

    def exitActive(self):
        self._cleanupCompleteState(self._completeState)
        self._completeState = None
        if self._parent is not None:
            self._parent.removeChild(self)
        del self._handle
        del self._scopeId
        del self._event
        # we need this in the closing state
        #del self._parent
        del self._children

    def _cleanupCompleteState(self, completeState):
        completeState.cleanup()

    def _hasCompleteEvent(self):
        assert self.isActive()
        return self._scopeId != NO_SCOPE
    def _sendCompleteEvent(self):
        assert self.state != 'Off'
        # internal func. If we've got an event, now's the time to send it.
        # Call this even if we don't have an event to send, it handles
        # parent completion.
        # keep a reference to parent around in case it goes away when we send
        # the complete event
        parent = self._parent
        if self._event is not None:
            event = self._event
            self._scopeId = NO_SCOPE
            self._event = None
            messenger.send(event)
        if parent is not None:
            parent._handleChildCompleted()

    def enterClosing(self, scopeId=None, event=None):
        if event is not None:
            self._scopeId = scopeId
            self._event = event
            self._completeState = _ClosingInterestCompleteState(self._notify, self._scopeId, self._closeCompleted, self)
        else:
            self.forceTransition('Off')
    def _closeCompleted(self):
        self.request('Off')
    def exitClosing(self):
        if self._completeState is not None:
            self._cleanupCompleteState(self._completeState)
            self._completeState = None
        self._doneCallback()
        del self._parent

    
class DoInterestManager(DirectObject.DirectObject):
    """
    Top level Interest Manager
    """
    notify = directNotify.newCategory("DoInterestManager")
    try:
        tempbase = base
    except:
        tempbase = simbase
    InterestDebug = tempbase.config.GetBool('interest-debug', True)
    del tempbase
    notify.setDebug(InterestDebug)

    # 'handle' is a number that represents a single interest set that the
    # client has requested; the interest set may be modified
    _HandleSerialNum = 0
    # high bit is reserved for server interests
    _HandleMask = 0x7FFF

    # 'scope' refers to a single request to change an interest set
    _ScopeIdSerialNum = 100
    _ScopeIdMask = 0x3FFFFFFF # avoid making Python create a long

    _HoldSerials = SerialNum()

    def __init__(self):
        assert DoInterestManager.notify.debugCall()
        DirectObject.DirectObject.__init__(self)
        self._interests = {}
        self._interestCompleteHolds = {}
        if __debug__:
            self._debug_currentInterests = []

    def addInterest(self, parentId, zoneIdList, description, event=None,
                    parentInterest=None):
        """
        Look into a (set of) zone(s).
        """
        assert DoInterestManager.notify.debugCall()
        handle = self._getNextHandle()
        assert handle not in self._interests
        if event is not None:
            scopeId = self._getNextScopeId()
        else:
            scopeId = NO_SCOPE
        parent = self._interests.get(parentInterest)
        self._interests[handle] = Interest(self._handleInterestDone, self.notify,
                                           handle, description, scopeId, event, parent)
        self._sendAddInterest(handle, scopeId, parentId, zoneIdList)
        assert self.printInterestsIfDebug()
        return handle

    def addInterestHold(self, handle):
        # returns a delayId
        assert DoInterestManager.notify.debugCall()
        assert handle in self._interests
        holdId = DoInterestManager._HoldSerials.next()
        self._interests[handle].addHold(holdId)
        self._interestCompleteHolds[holdId] = handle
        return holdId

    def removeInterestHold(self, holdId):
        assert holdId in self._interestCompleteHolds
        handle = self._interestCompleteHolds[holdId]
        del self._interestCompleteHolds[holdId]
        assert handle in self._interests
        interest = self._interests[handle]
        interest.removeHold()

    def removeInterest(self, handle, event=None):
        """
        Stop looking in a (set of) zone(s)
        """
        assert DoInterestManager.notify.debugCall()
        existed = False
        interest = self._interests.get(handle)
        if interest is not None:
            existed = True
            if event is not None:
                scopeId = self._getNextScopeId()
            else:
                scopeId = NO_SCOPE
            interest.handleInterestRemove(scopeId, event)
            self._sendRemoveInterest(handle, scopeId)
        else:
            DoInterestManager.notify.warning("removeInterest: handle not found: %s" % (handle))
        assert self.printInterestsIfDebug()
        return existed

    def alterInterest(self, handle, parentId, zoneIdList, description=None, event=None, parentInterest=None):
        """
        Removes old interests and adds new interests.

        Note that when an interest is changed, only the most recent
        change's event will be triggered. Previous events are abandoned.
        If this is a problem, consider opening multiple interests.
        """
        assert DoInterestManager.notify.debugCall()
        exists = False
        if self._interests.has_key(handle):
            interest = self._interests[handle]
            if event is not None:
                scopeId = self._getNextScopeId()
            else:
                scopeId = NO_SCOPE
            parent = None
            if parentInterest is not None:
                parent = self._interests.get(parentInterest)
            interest.handleInterestChange(handle, scopeId, event, parent, description)
            DoInterestManager.notify.debug(
                'alterInterest(): handle=%s, parent=%s, zoneIds=%s, description=%s, event=%s' % (
                handle, parentId, zoneIdList, description, event))
            self._sendAddInterest(handle, scopeId, parentId, zoneIdList)
            exists = True
            assert self.printInterestsIfDebug()
        else:
            DoInterestManager.notify.warning("alterInterest: handle not found: %s" % (handle))
        return exists

    def _handleInterestDone(self, handle):
        assert handle in self._interests
        del self._interests[handle]

    def _getNextHandle(self):
        handle = DoInterestManager._HandleSerialNum
        while True:
            handle = (handle + 1) & DoInterestManager._HandleMask
            # skip handles that are already in use
            if handle not in self._interests:
                break
            DoInterestManager.notify.warning('interest %s already in use' % handle)
        DoInterestManager._HandleSerialNum = handle
        return DoInterestManager._HandleSerialNum
    def _getNextScopeId(self):
        scopeId = DoInterestManager._ScopeIdSerialNum
        while True:
            scopeId = (scopeId + 1) & DoInterestManager._ScopeIdMask
            # skip over the 'no scope' id
            if scopeId != NO_SCOPE:
                break
        DoInterestManager._ScopeIdSerialNum = scopeId
        return DoInterestManager._ScopeIdSerialNum

    if __debug__:
        def printInterestsIfDebug(self):
            if DoInterestManager.notify.getDebug():
                self.printInterests()
            return 1 # for assert()

        def printInterests(self):
            print "*********************** Interest Sets **************"
            for interest in self._interests.values():
                print interest
            print "************************** History *****************"
            print "(Handle, ScopeId, ParentId, ZoneIdList)"
            for i in self._debug_currentInterests:
                print i
            print "****************************************************"

    def _sendAddInterest(self, handle, scopeId, parentId, zoneIdList):
        """
        Part of the new otp-server code.

        handle is a client-side created number that refers to
                a set of interests.  The same handle number doesn't
                necessarily have any relationship to the same handle
                on another client.
        """
        assert DoInterestManager.notify.debugCall()
        if __debug__:
            if isinstance(zoneIdList, types.ListType):
                zoneIdList.sort()
            self._debug_currentInterests.append(
                (handle, scopeId, parentId, zoneIdList))
        if parentId == 0:
            DoInterestManager.notify.error(
                'trying to set interest to invalid parent: %s' % parentId)
        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_ADD_INTEREST)
        datagram.addUint16(handle)
        datagram.addUint32(scopeId)
        datagram.addUint32(parentId)
        if isinstance(zoneIdList, types.ListType):
            vzl = list(zoneIdList)
            vzl.sort()
            uniqueElements(vzl)
            for zone in vzl:
                datagram.addUint32(zone)
        else:
           datagram.addUint32(zoneIdList)
        self.send(datagram)

    def _sendRemoveInterest(self, handle, scopeId):
        """
        handle is a client-side created number that refers to
                a set of interests.  The same handle number doesn't
                necessarily have any relationship to the same handle
                on another client.
        """
        assert DoInterestManager.notify.debugCall()
        datagram = PyDatagram()
        # Add message type
        datagram.addUint16(CLIENT_REMOVE_INTEREST)
        datagram.addUint16(handle)
        if scopeId != NO_SCOPE:
            datagram.addUint32(scopeId)            
        self.send(datagram)

    def handleInterestDoneMessage(self, di):
        """
        This handles the interest done messages and may dispatch an event
        """
        assert DoInterestManager.notify.debugCall()
        handle = di.getUint16()
        scopeId = di.getUint32()
        interest = self._interests[handle]
        if self.InterestDebug:
            print ('INTEREST DEBUG: interestDone(): handle=%s, name=%s' %
                   (handle, interest._description))
        DoInterestManager.notify.debug(
            "handleInterestDoneMessage--> Received handle %s, scope %s" % (
            handle, scopeId))
        interest.handleInterestComplete(scopeId)
        assert self.printInterestsIfDebug()

if __debug__:
    class dummyClass: pass

    def testInterest():
        class Acceptor(DirectObject.DirectObject):
            def __init__(self, event, callback=None):
                self._gotEvent = False
                self.accept(event, self._handleEvent)
                self._callback = callback
            def _handleEvent(self):
                self._gotEvent = True
                if self._callback is not None:
                    self._callback()
            def gotEvent(self):
                return self._gotEvent
            def reset(self):
                self._gotEvent = False

        def createInterest(scopeId=None, event=None):
            class Done:
                def __init__(self):
                    self.done = False
            done = Done()
            def handleDone(handle, done=done):
                done.done = True
            notify = dummyClass()
            notify.debug = lambda str: 0
            interest = Interest(handleDone, notify, 1122, 'test interest', scopeId=scopeId, event=event)
            return interest, done
            
        def testCreate():
            interest, done = createInterest()
            assert interest.isActive()
            interest.destroy()
            assert not done.done
            
        def testComplete():
            scopeId = 23
            event = 'doneEvent'
            acceptor = Acceptor(event)
            interest, done = createInterest(scopeId=scopeId, event=event)

            assert interest.isActive()
            assert not done.done
            assert not acceptor.gotEvent()
            interest.handleInterestComplete(scopeId-1)
            assert interest.isActive()
            assert not done.done
            assert not acceptor.gotEvent()
            interest.handleInterestComplete(scopeId)
            assert interest.isActive()
            assert not done.done
            assert acceptor.gotEvent()
            interest.handleInterestRemove()
            assert not interest.isActive()
            assert done.done

        def testRemoveEvent():
            scopeId = 23
            event = 'doneEvent'
            acceptor = Acceptor(event)
            interest, done = createInterest()

            assert interest.isActive()
            assert not done.done
            assert not acceptor.gotEvent()
            interest.handleInterestRemove(scopeId, event)
            assert not interest.isActive()
            assert not done.done
            assert not acceptor.gotEvent()
            interest.handleInterestComplete(scopeId)
            assert not interest.isActive()
            assert done.done
            assert acceptor.gotEvent()

        def testRemoveInCompleteEvent():
            scopeId = 23
            event = 'doneEvent'
            interest, done = createInterest(scopeId=scopeId, event=event)
            def closeInterest(interest=interest):
                interest.handleInterestRemove()
            acceptor = Acceptor(event, callback=closeInterest)

            interest.handleInterestComplete(scopeId)


        testCreate()
        testComplete()
        testRemoveEvent()
        testRemoveInCompleteEvent()
        
    def test():
        testInterest()
        print 'tests completed'

    if __dev__:
        test()

"""
from direct.distributed import DoInterestManager
reload(DoInterestManager)
DoInterestManager.test()
"""
