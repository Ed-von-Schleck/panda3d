building_dll(CORE)
set(sources event_composite1.cxx event_composite2.cxx)

set(scan_headers
  asyncTask.h asyncTaskChain.h asyncTaskCollection.h asyncTaskManager.h
  asyncTaskPause.h asyncTaskSequence.h config_event.h buttonEvent.h
  buttonEventList.h genericAsyncTask.h pointerEvent.h pointerEventList.h
  pythonTask.h event.h eventHandler.h eventParameter.h eventQueue.h
  eventReceiver.h pt_Event.h throw_event.h
)

set(install_headers
  asyncTask.h asyncTask.I asyncTaskChain.h asyncTaskChain.I
  asyncTaskCollection.h asyncTaskCollection.I asyncTaskManager.h
  asyncTaskManager.I asyncTaskPause.h asyncTaskPause.I asyncTaskSequence.h
  asyncTaskSequence.I buttonEvent.I buttonEvent.h buttonEventList.I
  buttonEventList.h config_event.h genericAsyncTask.h genericAsyncTask.I
  pointerEvent.I pointerEvent.h pointerEventList.I pointerEventList.h
  pythonTask.h pythonTask.I event.I event.h eventHandler.h eventHandler.I
  eventParameter.I eventParameter.h eventQueue.I eventQueue.h
  eventReceiver.h pt_Event.h throw_event.I throw_event.h
)

add_library(p3event ${sources})
interrogate(p3event core ${scan_headers} ${sources})

install_headers(${install_headers})

