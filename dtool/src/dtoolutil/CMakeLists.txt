configure_file(pandaVersion.h.in pandaVersion.h)
configure_file(checkPandaVersion.h.in checkPandaVersion.h)
configure_file(checkPandaVersion.cxx.in checkPandaVersion.cxx)

set(sources dtoolutil_composite1.cxx dtoolutil_composite2.cxx)

set(scan_headers
  config_dtoolutil.h executionEnvironment.h filename.h
  filename_assist.h load_dso.h dSearchPath.h pandaFileStream.h
  pandaFileStreamBuf.h pandaSystem.h pandaVersion.h pfstream.h pfstreamBuf.h
  vector_string.h gnu_getopt.h
)

set(install_headers
  checkPandaVersion.h config_dtoolutil.h executionEnvironment.I
  executionEnvironment.h filename.I filename.h load_dso.h dSearchPath.I
  dSearchPath.h pandaFileStream.h pandaFileStream.I pandaFileStreamBuf.h
  pandaSystem.h pandaVersion.h pfstream.h pfstream.I pfstreamBuf.h
  vector_string.h gnu_getopt.h vector_src.cxx vector_src.h
)

panda_add_sources(core ${sources})
panda_add_scan_sources(core ${scan_headers} ${sources})

install(FILES ${install_headers} DESTINATION include/panda3d)

