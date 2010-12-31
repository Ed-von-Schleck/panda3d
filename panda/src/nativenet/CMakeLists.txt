building_dll(CORE)
set(sources nativenet_composite1.cxx)

set(scan_headers
  buffered_datagramconnection.h buffered_datagramreader.h ringbuffer.h
  socket_ip.h socket_tcp_listen.h time_accumulator.h time_out.h
  socket_address.h socket_portable.h time_base.h time_span.h
  buffered_datagramwriter.h socket_base.h socket_selector.h socket_udp.h
  socket_udp_incoming.h time_clock.h membuffer.h socket_fdset.h socket_tcp.h
  socket_udp_outgoing.h time_general.h
)

set(install_headers
  buffered_datagramconnection.h config_nativenet.h ringbuffer.h ringbuffer.i
  socket_ip.h socket_tcp_listen.h time_accumulator.h time_out.h
  buffered_datagramreader.h buffered_datagramreader.i socket_address.h
  socket_portable.h time_base.h time_span.h buffered_datagramwriter.h
  socket_base.h socket_selector.h socket_udp.h socket_udp_incoming.h
  time_clock.h membuffer.h membuffer.i socket_fdset.h socket_tcp.h
  socket_udp_outgoing.h time_general.h
)

add_library(p3nativenet ${sources})
use_libraries(p3nativenet OPENSSL)
interrogate(p3nativenet core ${scan_headers} ${sources})

install_headers(${install_headers})

