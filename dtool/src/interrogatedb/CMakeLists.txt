if(HAVE_INTERROGATE)

building_dll(INTERROGATEDB)
set(sources interrogatedb_composite1.cxx interrogatedb_composite2.cxx)

set(scan_headers interrogate_interface.h)

set(install_headers
  interrogate_interface.h
  interrogate_request.h vector_int.h
  config_interrogatedb.h
  py_panda.h
  vector_int.h
)

# Note, p3interrogatedb is a top-level lib and not a directory lib.
if(LINK_ALL_STATIC)
  add_library(p3interrogatedb STATIC ${sources})
else()
  add_library(p3interrogatedb SHARED ${sources})
endif()

if(HAVE_PYTHON)
  interrogate(p3interrogatedb interrogatedb ${scan_headers})
  add_python_module(interrogatedb p3interrogatedb)
  target_link_libraries(interrogatedb p3prc p3dtoolbase)
endif(HAVE_PYTHON)

install_headers(${install_headers})
install(TARGETS p3interrogatedb DESTINATION lib)

endif(HAVE_INTERROGATE)
