#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c \
                   COLLADASaxFrameworkLoader:c COLLADAFramework:c COLLADABaseUtils:c GeneratedSaxParser:c xml2:c pcre:c MathMLSolver:c

#begin lib_target
  #define TARGET p3collada
  #define LOCAL_LIBS display pgraph express putil pandabase
  #define C++FLAGS -I/usr/local/include/opencollada

  #define COMBINED_SOURCES collada_composite1.cxx

  #define SOURCES \
    colladaLoader.h \
    config_collada.h \
    loaderFileTypeDae.h

  #define INCLUDED_SOURCES \
    colladaLoader.cxx \
    config_collada.cxx \
    loaderFileTypeDae.cxx

  #define INSTALL_HEADERS \
    colladaLoader.h \
    config_collada.h \
    loaderFileTypeDae.h

  //#define IGATESCAN all

#end lib_target

