building_dll(CORE)
set(component_libs
  p3pnmimage p3pstatclient
  p3net p3nativenet
  p3mathutil p3linmath
  p3event p3pipeline p3putil
  p3downloader p3express
  p3dconfig p3prc
  p3dtoolutil p3dtoolbase
)

if(LINK_ALL_STATIC)
  add_library(p3core STATIC core.cxx)
else()
  add_library(p3core SHARED core.cxx)
endif()
use_libraries(p3core OPENSSL)
target_link_libraries(p3core ${component_libs})
set_target_properties(p3core PROPERTIES LINK_INTERFACE_LIBRARIES "")
install(TARGETS p3core DESTINATION lib)

# This is a bit of a hack to let CMake build p3pystub before p3core.
# We need to do this to get it to specify the correct linker order.
add_dependencies(p3core p3pystub)

if(HAVE_PYTHON AND HAVE_INTERROGATE)
  add_python_module(core ${component_libs})
  target_link_libraries(core p3core p3interrogatedb)
endif()
