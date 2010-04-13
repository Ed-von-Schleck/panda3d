#define BUILD_DIRECTORY $[HAVE_TINYXML]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#begin lib_target
  #define TARGET p3collada
  #define USE_PACKAGES tinyxml

  #define COMBINED_SOURCES collada_composite1.cxx

  #define SOURCES \
    colladaData.h \
    colladaLoader.h \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

  #define INCLUDED_SOURCES \
    colladaData.cxx \
    colladaLoader.cxx \
    config_collada.cxx \
    load_collada_file.cxx \
    loaderFileTypeDae.cxx

  #define INSTALL_HEADERS \
    colladaData.h \
    colladaLoader.h \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

#end lib_target

