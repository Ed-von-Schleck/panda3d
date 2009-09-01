#define BUILD_DIRECTORY $[HAVE_PHYSX]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#begin lib_target
  #define TARGET physx
  #define BUILD_TARGET $[HAVE_PHYSX]
  #define USE_PACKAGES physx
  #define LOCAL_LIBS pgraph linmath
  #define COMBINED_SOURCES $[TARGET]_composite.cxx
    #define INTERROGATE_OPTIONS $[INTERROGATE_OPTIONS] -DHAVE_PHYSX

  #define SOURCES \
    NoMinMax.h \
    config_physx.h \
    physxManager.I physxManager.h \

  #define INCLUDED_SOURCES
    physxManager.cxx \

  #define INSTALL_HEADERS \
    NoMinMax.h \
    config_physx.h
    physxManager.I physxManager.h \

  #define IGATESCAN all

#end lib_target

