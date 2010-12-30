set(sources pipeline_composite1.cxx pipeline_composite2.cxx)

set(install_headers
  asyncTaskBase.h asyncTaskBase.I contextSwitch.h blockerSimple.h
  blockerSimple.I conditionVar.h conditionVar.I conditionVarDebug.h
  conditionVarDebug.I conditionVarDirect.h conditionVarDirect.I
  conditionVarDummyImpl.h conditionVarDummyImpl.I conditionVarFull.h
  conditionVarFull.I conditionVarFullDebug.h conditionVarFullDebug.I
  conditionVarFullDirect.h conditionVarFullDirect.I conditionVarFullWin32Impl.h
  conditionVarFullWin32Impl.I conditionVarImpl.h conditionVarPosixImpl.h
  conditionVarPosixImpl.I conditionVarWin32Impl.h conditionVarWin32Impl.I
  conditionVarSimpleImpl.h conditionVarSimpleImpl.I conditionVarSpinlockImpl.h
  conditionVarSpinlockImpl.I config_pipeline.h cycleData.h cycleData.I
  cycleDataLockedReader.h cycleDataLockedReader.I cycleDataLockedStageReader.h
  cycleDataLockedStageReader.I cycleDataReader.h cycleDataReader.I
  cycleDataStageReader.h cycleDataStageReader.I cycleDataStageWriter.h
  cycleDataStageWriter.I cycleDataWriter.h cycleDataWriter.I cyclerHolder.h
  cyclerHolder.I externalThread.h lightMutex.I lightMutex.h lightMutexDirect.h
  lightMutexDirect.I lightMutexHolder.I lightMutexHolder.h lightReMutex.I
  lightReMutex.h lightReMutexDirect.h lightReMutexDirect.I lightReMutexHolder.I
  lightReMutexHolder.h mainThread.h mutexDebug.h mutexDebug.I mutexDirect.h
  mutexDirect.I mutexHolder.h mutexHolder.I mutexSimpleImpl.h mutexSimpleImpl.I
  mutexTrueImpl.h pipeline.h pipeline.I pipelineCycler.h pipelineCycler.I
  pipelineCyclerLinks.h pipelineCyclerLinks.I pipelineCyclerBase.h
  pipelineCyclerDummyImpl.h pipelineCyclerDummyImpl.I
  pipelineCyclerTrivialImpl.h pipelineCyclerTrivialImpl.I
  pipelineCyclerTrueImpl.h pipelineCyclerTrueImpl.I pmutex.h pmutex.I
  pythonThread.h reMutex.I reMutex.h reMutexDirect.h reMutexDirect.I
  reMutexHolder.I reMutexHolder.h psemaphore.h psemaphore.I thread.h thread.I
  threadImpl.h threadDummyImpl.h threadDummyImpl.I threadPosixImpl.h
  threadPosixImpl.I threadSimpleImpl.h threadSimpleImpl.I threadSimpleManager.h
  threadSimpleManager.I threadWin32Impl.h threadWin32Impl.I threadPriority.h
)

set(scan_headers
  asyncTaskBase.h contextSwitch.h blockerSimple.h conditionVar.h
  conditionVarDebug.h conditionVarDirect.h conditionVarDummyImpl.h
  conditionVarFull.h conditionVarFullDebug.h conditionVarFullDirect.h
  conditionVarFullWin32Impl.h conditionVarImpl.h conditionVarPosixImpl.h
  conditionVarWin32Impl.h conditionVarSimpleImpl.h conditionVarSpinlockImpl.h
  config_pipeline.h cycleData.h cycleDataLockedReader.h
  cycleDataLockedStageReader.h cycleDataReader.h cycleDataStageReader.h
  cycleDataStageWriter.h cycleDataWriter.h cyclerHolder.h externalThread.h
  lightMutex.h lightMutexDirect.h lightMutexHolder.h lightReMutex.h
  lightReMutexDirect.h lightReMutexHolder.h mainThread.h mutexDebug.h
  mutexDirect.h mutexHolder.h mutexSimpleImpl.h mutexTrueImpl.h pipeline.h
  pipelineCycler.h pipelineCyclerLinks.h pipelineCyclerBase.h
  pipelineCyclerDummyImpl.h pipelineCyclerTrivialImpl.h
  pipelineCyclerTrueImpl.h pmutex.h pythonThread.h reMutex.h reMutexDirect.h
  reMutexHolder.h psemaphore.h thread.h threadImpl.h threadDummyImpl.h
  threadPosixImpl.h threadSimpleImpl.h threadSimpleManager.h threadWin32Impl.h
  threadPriority.h
)

add_library(p3pipeline ${sources})
interrogate(p3pipeline core ${scan_headers} ${sources})

install_headers(${install_headers})
