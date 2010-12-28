set(sources dconfig_composite1.cxx)

set(scan_headers
  config_dconfig.h dconfig.h
)

set(install_headers
  config_dconfig.h dconfig.I dconfig.h
)

panda_add_sources(core ${sources})
panda_add_scan_sources(core ${scan_headers} ${sources})

install(FILES ${install_headers} DESTINATION include/panda3d)

