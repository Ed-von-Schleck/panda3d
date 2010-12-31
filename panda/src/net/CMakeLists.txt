building_dll(CORE)
set(sources net_composite1.cxx net_composite2.cxx)

set(scan_headers
  config_net.h connection.h connectionListener.h connectionManager.h
  connectionReader.h connectionWriter.h datagramQueue.h datagramTCPHeader.h
  datagramUDPHeader.h netAddress.h netDatagram.h datagramGeneratorNet.h
  datagramSinkNet.h queuedConnectionListener.h queuedConnectionManager.h
  queuedConnectionReader.h recentConnectionReader.h queuedReturn.h
)

set(install_headers
  config_net.h connection.h connectionListener.h connectionManager.h
  connectionReader.I connectionReader.h connectionWriter.h datagramQueue.h
  datagramTCPHeader.I datagramTCPHeader.h datagramUDPHeader.I
  datagramUDPHeader.h netAddress.h netDatagram.I netDatagram.h
  queuedConnectionListener.I datagramGeneratorNet.I datagramGeneratorNet.h
  datagramSinkNet.I datagramSinkNet.h queuedConnectionListener.h
  queuedConnectionManager.h queuedConnectionReader.h queuedReturn.I
  queuedReturn.h recentConnectionReader.h
)

add_library(p3net ${sources})
interrogate(p3net core ${scan_headers} ${sources})

install_headers(${install_headers})

