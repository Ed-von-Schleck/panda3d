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
    colladaAssetElement.h colladaAssetElement.I \
    colladaCamera.h colladaCamera.I \
    colladaContributor.h colladaContributor.I \
    colladaDocument.h colladaDocument.I \
    colladaEffect.h colladaEffect.I \
    colladaElement.h colladaElement.I \
    colladaGeometry.h \
    colladaInstance.h colladaInstance.I colladaInstance.N \
    colladaLibrary.h colladaLibrary.I colladaLibrary.N \
    colladaLight.h colladaLight.I \
    colladaMaterial.h colladaMaterial.I \
    colladaMesh.h \
    colladaNode.h colladaNode.I \
    colladaOptics.h colladaOptics.I \
    colladaProfile.h colladaProfile.I \
    colladaRoot.h colladaRoot.I \
    colladaSource.h \
    colladaVisualScene.h colladaVisualScene.I \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

  #define INCLUDED_SOURCES \
    colladaAsset.cxx \
    colladaAssetElement.cxx \
    colladaCamera.cxx \
    colladaContributor.cxx \
    colladaDocument.cxx \
    colladaEffect.cxx \
    colladaGeometry.cxx \
    colladaLibrary.cxx \
    colladaLight.cxx \
    colladaMaterial.cxx \
    colladaMesh.cxx \
    colladaNode.cxx \
    colladaOptics.cxx \
    colladaProfile.cxx \
    colladaRoot.cxx \
    colladaVisualScene.cxx \
    config_collada.cxx \
    load_collada_file.cxx \
    loaderFileTypeDae.cxx

  #define INSTALL_HEADERS \
    colladaAsset.h colladaAsset.I \
    colladaAssetElement.h colladaAssetElement.I \
    colladaCamera.h colladaCamera.I \
    colladaContributor.h colladaContributor.I \
    colladaDocument.h colladaDocument.I \
    colladaEffect.h colladaEffect.I \
    colladaElement.h colladaElement.I \
    colladaGeometry.h \
    colladaInstance.h colladaInstance.I \
    colladaLibrary.h colladaLibrary.I \
    colladaLight.h colladaLight.I \
    colladaMaterial.h colladaMaterial.I \
    colladaMesh.h \
    colladaNode.h colladaNode.I \
    colladaOptics.h colladaNode.I \
    colladaProfile.h colladaProfile.I \
    colladaRoot.h colladaRoot.I \
    colladaSource.h \
    colladaVisualScene.h colladaVisualScene.I \
    config_collada.h \
    load_collada_file.h \
    loaderFileTypeDae.h

  #define IGATESCAN all

#end lib_target

