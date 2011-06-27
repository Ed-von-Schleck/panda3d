#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c
#define WIN_SYS_LIBS $[WIN_SYS_LIBS] ws2_32.lib
                   
#begin lib_target
  #define TARGET pcegui
  #define USE_PACKAGES cegui
  #define LOCAL_LIBS \
    pgraph dgraph display pgraphnodes cull putil \
    gsgbase gobj linmath mathutil \
    pstatclient

  #define SOURCES  \
    ceguiInputHandler.h \
    ceguiPanda3dRenderer.h \
    ceguiPanda3dGeometryBuffer.h \
    ceguiPanda3dRenderTarget.h \
    ceguiPanda3dTexture.h \
    ceguiPanda3dTextureTarget.h \
    ceguiInputHandler.cxx \
    ceguiPanda3dRenderer.cxx \
    ceguiPanda3dGeometryBuffer.cxx \
    ceguiPanda3dRenderTarget.cxx \
    ceguiPanda3dTexture.cxx \
    ceguiPanda3dTextureTarget.cxx \
    config_cegui.h config_cegui.cxx \
    ceguiSupport.h ceguiSupport.cxx

  #define INSTALL_HEADERS \
    config_cegui.h \
    ceguiSupport.h ceguiInputHandler.h
    ceguiPanda3dRenderer.h \
    ceguiPanda3dGeometryBuffer.h \
    ceguiPanda3dRenderTarget.h \
    ceguiPanda3dTexture.h

  #define IGATESCAN \
    ceguiSupport.h
    //    all

#end lib_target
