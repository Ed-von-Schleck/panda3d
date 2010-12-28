set(sources pandabase.cxx)

set(install_headers
  pandabase.h pandasymbols.h
)

panda_add_sources(core ${sources})

install(FILES ${install_headers} DESTINATION include/panda3d)
