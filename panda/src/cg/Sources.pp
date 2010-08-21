#define BUILD_DIRECTORY $[HAVE_CG]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#define USE_PACKAGES cg

#begin lib_target
  #define TARGET p3cg
  #define LOCAL_LIBS display pgraph gobj express putil pandabase

  #define COMBINED_SOURCES cg_composite1.cxx

  #define SOURCES \
    config_cg.h \
    cgContextManager.h \
    cgEffect.I cgEffect.h

  #define INCLUDED_SOURCES \
    config_cg.cxx \
    cgContextManager.cxx \
    cgEffect.cxx

  #define INSTALL_HEADERS \
    config_cg.h \
    cgContextManager.h \
    cgEffect.I cgEffect.h

  #define IGATESCAN all

#end lib_target

