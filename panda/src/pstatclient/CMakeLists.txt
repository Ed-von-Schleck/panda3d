building_dll(CORE)
set(sources pstatclient_composite1.cxx pstatclient_composite2.cxx)

set(scan_headers
  config_pstats.h pStatClient.h pStatClientImpl.h pStatClientVersion.h
  pStatClientControlMessage.h pStatCollector.h pStatCollectorDef.h
  pStatCollectorForward.h pStatFrameData.h pStatProperties.h
  pStatServerControlMessage.h pStatThread.h pStatTimer.h
)

set(install_headers
  config_pstats.h pStatClient.I pStatClient.h pStatClientImpl.I
  pStatClientImpl.h pStatClientVersion.I pStatClientVersion.h
  pStatClientControlMessage.h pStatCollector.I pStatCollector.h
  pStatCollectorDef.h pStatCollectorForward.I pStatCollectorForward.h
  pStatFrameData.I pStatFrameData.h pStatProperties.h
  pStatServerControlMessage.h pStatThread.I pStatThread.h pStatTimer.I
  pStatTimer.h
)

add_library(p3pstatclient ${sources})
interrogate(p3pstatclient core ${scan_headers} ${sources})

install_headers(${install_headers})

