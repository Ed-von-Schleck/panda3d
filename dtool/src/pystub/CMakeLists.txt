set(sources pystub.cxx)

set(install_headers
  pystub.h
)

panda_add_sources(pystub ${sources})

install(FILES ${install_headers} DESTINATION include/panda3d)
