#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c
#define WIN_SYS_LIBS $[WIN_SYS_LIBS] ws2_32.lib
                   
#begin lib_target
  #define TARGET display
  #define LOCAL_LIBS \
    pgraph pgraphnodes cull putil gsgbase gobj linmath mathutil \
    pstatclient

  #define COMBINED_SOURCES $[TARGET]_composite1.cxx $[TARGET]_composite2.cxx 
 
  #define SOURCES  \
    standardMunger.I standardMunger.h \
    config_display.h \
    drawableRegion.I drawableRegion.h \
    displayRegion.I displayRegion.h  \
    frameBufferProperties.I frameBufferProperties.h \
    graphicsEngine.I graphicsEngine.h \
    graphicsOutput.I graphicsOutput.h \
    graphicsBuffer.I graphicsBuffer.h \
    graphicsPipe.I graphicsPipe.h  \
    graphicsPipeSelection.I graphicsPipeSelection.h \
    graphicsStateGuardian.I \
    graphicsStateGuardian.h \
    graphicsThreadingModel.I graphicsThreadingModel.h \
    graphicsWindow.I graphicsWindow.h \
    graphicsWindowInputDevice.I  \
    graphicsWindowInputDevice.h \
    graphicsDevice.h graphicsDevice.I \
    lru.h \
    parasiteBuffer.I parasiteBuffer.h \
    windowProperties.I windowProperties.h \
    renderBuffer.h \
    stencilRenderStates.h \
    displaySearchParameters.h \
    displayInformation.h    
    
 #define INCLUDED_SOURCES  \
    standardMunger.cxx \
    config_display.cxx \
    drawableRegion.cxx \
    displayRegion.cxx \
    frameBufferProperties.cxx \
    graphicsEngine.cxx \
    graphicsOutput.cxx \
    graphicsBuffer.cxx \
    graphicsPipe.cxx \
    graphicsPipeSelection.cxx \
    graphicsStateGuardian.cxx  \
    graphicsThreadingModel.cxx \
    graphicsWindow.cxx graphicsWindowInputDevice.cxx  \
    graphicsDevice.cxx \
    parasiteBuffer.cxx \
    windowProperties.cxx \
    lru.cxx \
    stencilRenderStates.cxx \
    displaySearchParameters.cxx \
    displayInformation.cxx    

  #define INSTALL_HEADERS \
    standardMunger.I standardMunger.h \
    config_display.h \
    drawableRegion.I drawableRegion.h \
    displayRegion.I displayRegion.h \
    frameBufferProperties.I frameBufferProperties.h \
    graphicsEngine.I graphicsEngine.h \
    graphicsOutput.I graphicsOutput.h \
    graphicsBuffer.I graphicsBuffer.h \
    graphicsPipe.I graphicsPipe.h \
    graphicsPipeSelection.I graphicsPipeSelection.h \
    graphicsStateGuardian.I \
    graphicsStateGuardian.h \
    graphicsWindow.I graphicsWindow.h \
    graphicsThreadingModel.I graphicsThreadingModel.h \
    graphicsWindowInputDevice.I graphicsWindowInputDevice.h \
    graphicsDevice.I graphicsDevice.h \
    lru.h \
    parasiteBuffer.I parasiteBuffer.h \
    windowProperties.I windowProperties.h \
    renderBuffer.h \
    stencilRenderStates.h \
    displaySearchParameters.h \
    displayInformation.h    

  #define IGATESCAN all

#end lib_target

#begin test_bin_target
  #define TARGET test_display
  #define LOCAL_LIBS \
    display putil

  #define SOURCES \
    test_display.cxx

#end test_bin_target

