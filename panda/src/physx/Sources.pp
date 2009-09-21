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
    physxBoxShape.I physxBoxShape.h \
    physxBoxShapeDesc.I physxBoxShapeDesc.h \
    physxCapsuleShape.I physxCapsuleShape.h \
    physxCapsuleShapeDesc.I physxCapsuleShapeDesc.h \
    physxContactReport.I physxContactReport.h \
    physxDebugGeomNode.I physxDebugGeomNode.h \
    physxManager.I physxManager.h \
    physxMaterial.I physxMaterial.h \
    physxMaterialDesc.I physxMaterialDesc.h \
    physxObject.I physxObject.h \
    physxPlaneShape.I physxPlaneShape.h \
    physxPlaneShapeDesc.I physxPlaneShapeDesc.h \
    physxScene.I physxScene.h \
    physxSceneDesc.I physxSceneDesc.h \
    physxShape.I physxShape.h \
    physxShapeDesc.I physxShapeDesc.h \
    physxSphereShape.I physxSphereShape.h \
    physxSphereShapeDesc.I physxSphereShapeDesc.h \
    physxTriggerReport.I physxTriggerReport.h \

  #define INCLUDED_SOURCES
    physxActor.cxx \
    physxActorDesc.cxx \
    physxBodyDesc.cxx \
    physxBoxShapeDesc.cxx \
    physxCapsuleShape.cxx \
    physxCapsuleShapeDesc.cxx \
    physxContactReport.cxx \
    physxDebugGeomNode.cxx \
    physxManager.cxx \
    physxMaterial.cxx \
    physxMaterialDesc.cxx \
    physxObject.cxx \
    physxPlaneShape.cxx \
    physxPlaneShapeDesc.cxx \
    physxScene.cxx \
    physxSceneDesc.cxx \
    physxShape.cxx \
    physxShapeDesc.cxx \
    physxSphereShape.cxx \
    physxSphereShapeDesc.cxx \
    physxTriggerReport.cxx \

  #define INSTALL_HEADERS \
    NoMinMax.h \
    config_physx.h
    physxActor.I physxActor.h \
    physxActorDesc.I physxActorDesc.h \
    physxBodyDesc.I physxBodyDesc.h \
    physxBoxShape.I physxBoxShape.h \
    physxBoxShapeDesc.I physxBoxShapeDesc.h \
    physxCapsuleShape.I physxCapsuleShape.h \
    physxCapsuleShapeDesc.I physxCapsuleShapeDesc.h \
    physxContactReport.I physxContactReport.h \
    physxDebugGeomNode.I physxDebugGeomNode.h \
    physxManager.I physxManager.h \
    physxMaterial.I physxMaterial.h \
    physxMaterialDesc.I physxMaterialDesc.h \
    physxObject.I physxObject.h \
    physxPlaneShape.I physxPlaneShape.h \
    physxPlaneShapeDesc.I physxPlaneShapeDesc.h \
    physxScene.I physxScene.h \
    physxSceneDesc.I physxSceneDesc.h \
    physxShape.I physxShape.h \
    physxShapeDesc.I physxShapeDesc.h \
    physxSphereShape.I physxSphereShape.h \
    physxSphereShapeDesc.I physxSphereShapeDesc.h \
    physxTriggerReport.I physxTriggerReport.h \

  #define IGATESCAN all

#end lib_target

