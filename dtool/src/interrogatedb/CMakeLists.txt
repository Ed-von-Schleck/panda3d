if(HAVE_INTERROGATE)

set(sources interrogatedb_composite1.cxx interrogatedb_composite2.cxx)

set(scan_headers interrogate_interface.h)

set(install_headers
  interrogate_interface.h
  interrogate_request.h vector_int.h
  config_interrogatedb.h
  py_panda.h
  vector_int.h
)

panda_add_sources(interrogatedb ${sources})
panda_add_scan_sources(interrogatedb ${scan_headers})

install(FILES ${install_headers} DESTINATION include/panda3d)

endif(HAVE_INTERROGATE)
