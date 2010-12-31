building_dll(PYSTUB)
set(sources pystub.cxx)

set(install_headers
  pystub.h
)

# We compile this as a dynamic library, even with LINK_ALL_STATIC.
# This is because there doesn't seem to be a way to force a
# certain linker order in CMake, and pystub needs to be linked last.
add_library(p3pystub SHARED ${sources})

install_headers(${install_headers})
install(TARGETS p3pystub DESTINATION lib)
