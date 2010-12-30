building_dll(CORE)
set(sources dconfig_composite1.cxx)

set(scan_headers
  config_dconfig.h dconfig.h
)

set(install_headers
  config_dconfig.h dconfig.I dconfig.h
)

add_library(p3dconfig ${sources})
interrogate(p3dconfig core ${scan_headers} ${sources})

install_headers(${install_headers})
