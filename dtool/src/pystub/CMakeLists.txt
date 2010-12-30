set(sources pystub.cxx)

set(install_headers
  pystub.h
)

add_library(p3pystub SHARED ${sources})

install_headers(${install_headers})
install(TARGETS p3pystub DESTINATION lib)
