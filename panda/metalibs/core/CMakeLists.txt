set(component_libs
  p3downloader p3express
  p3interrogatedb p3dconfig p3prc
  p3dtoolutil p3dtoolbase
)

if(LINK_ALL_STATIC)
  add_library(p3core STATIC core.cxx)
else()
  add_library(p3core SHARED core.cxx)
endif()
target_link_libraries(p3core ${component_libs} ${OPENSSL_LIBRARIES})
set_target_properties(p3core PROPERTIES LINK_INTERFACE_LIBRARIES "")
install(TARGETS p3core DESTINATION lib)

if(HAVE_PYTHON AND HAVE_INTERROGATE)
  add_python_module(core ${component_libs})
  target_link_libraries(core p3core)
endif()
