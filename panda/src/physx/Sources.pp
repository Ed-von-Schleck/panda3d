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
    physxActor.I physxActor.h \
    physxActorDesc.I physxActorDesc.h \
    physxBodyDesc.I physxBodyDesc.h \
    physxBoxShapeDesc.I physxBoxShapeDesc.h \
    physxCapsuleShapeDesc.I physxCapsuleShapeDesc.h \
    physxDebugGeomNode.I physxDebugGeomNode.h \
    physxManager.I physxManager.h \
    physxObject.I physxObject.h \
    physxPlaneShapeDesc.I physxPlaneShapeDesc.h \
    physxScene.I physxScene.h \
    physxSceneDesc.I physxSceneDesc.h \
    physxShapeDesc.I physxShapeDesc.h \
    physxSphereShapeDesc.I physxSphereShapeDesc.h \

  #define INCLUDED_SOURCES
    physxActor.cxx \
    physxActorDesc.cxx \
    physxBodyDesc.cxx \
    physxBoxShapeDesc.cxx \
    physxCapsuleShapeDesc.cxx \
    physxDebugGeomNode.cxx \
    physxManager.cxx \
    physxObject.cxx \
    physxPlaneShapeDesc.cxx \
    physxScene.cxx \
    physxSceneDesc.cxx \
    physxShapeDesc.cxx \
    physxSphereShapeDesc.cxx \

  #define INSTALL_HEADERS \
    NoMinMax.h \
    config_physx.h
    physxActor.I physxActor.h \
    physxActorDesc.I physxActorDesc.h \
    physxBodyDesc.I physxBodyDesc.h \
    physxBoxShapeDesc.I physxBoxShapeDesc.h \
    physxCapsuleShapeDesc.I physxCapsuleShapeDesc.h \
    physxDebugGeomNode.I physxDebugGeomNode.h \
    physxManager.I physxManager.h \
    physxObject.I physxObject.h \
    physxPlaneShapeDesc.I physxPlaneShapeDesc.h \
    physxScene.I physxScene.h \
    physxSceneDesc.I physxSceneDesc.h \
    physxShapeDesc.I physxShapeDesc.h \
    physxSphereShapeDesc.I physxSphereShapeDesc.h \

  #define IGATESCAN all

#end lib_target

