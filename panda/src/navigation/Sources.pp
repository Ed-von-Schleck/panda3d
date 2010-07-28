#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c Recast:c Detour:c

#begin lib_target
  #define TARGET p3navigation
  #define LOCAL_LIBS display pgraph express putil pandabase
  #define C++FLAGS -I/usr/local/include/recast -I/usr/local/include/detour

  #define COMBINED_SOURCES navigation_composite1.cxx

  #define SOURCES \
    config_navigation.h \
    detourAgentNode.h \
    detourNavMeshNode.h detourNavMeshNode.I \
    navMeshAttrib.h navMeshAttrib.I \
    recastNavMesh.h recastNavMesh.I

  #define INCLUDED_SOURCES \
    config_navigation.cxx \
    detourAgentNode.cxx \
    detourNavMeshNode.cxx \
    navMeshAttrib.cxx \
    recastNavMesh.cxx

  #define INSTALL_HEADERS \
    config_navigation.h \
    detourAgentNode.h \
    detourNavMeshNode.h detourNavMeshNode.I \
    navMeshAttrib.h navMeshAttrib.I \
    recastNavMesh.h recastNavMesh.I

  #define IGATESCAN all

#end lib_target

