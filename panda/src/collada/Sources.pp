#define BUILD_DIRECTORY $[HAVE_TINYXML]

#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#begin lib_target
  #define TARGET p3collada
  #define USE_PACKAGES tinyxml
  #define LOCAL_LIBS display express putil pandabase

  #define COMBINED_SOURCES collada_composite1.cxx

  #define SOURCES \
    colladaAsset.h colladaAsset.I \
    colladaData.h colladaData.I \
    colladaInstance.h colladaInstance.I colladaInstance.N \
    colladaLibrary.h colladaLibrary.I colladaLibrary.N \
    colladaLoader.h colladaLoader.I \
    colladaNode.h \
    colladaVisualScene.h \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

  #define INCLUDED_SOURCES \
    colladaAsset.cxx \
    colladaData.cxx \
    colladaLoader.cxx \
    colladaNode.cxx \
    colladaVisualScene.cxx \
    config_collada.cxx \
    load_collada_file.cxx \
    loaderFileTypeDae.cxx

  #define INSTALL_HEADERS \
    colladaAsset.h colladaAsset.I \
    colladaData.h colladaData.I \
    colladaInstance.h colladaInstance.I \
    colladaLibrary.h colladaLibrary.I \
    colladaLoader.h colladaLoader.I \
    colladaNode.h \
    colladaVisualScene.h \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

  #define IGATESCAN all

#end lib_target

