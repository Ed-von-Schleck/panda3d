#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
                   dtoolutil:c dtoolbase:c dtool:m prc:c

#begin lib_target
  #define TARGET gsgbase
  #define LOCAL_LIBS \
    putil linmath

  #define COMBINED_SOURCES $[TARGET]_composite1.cxx

  #define SOURCES \
    config_gsgbase.h \
    displayRegionBase.I displayRegionBase.h \
    drawableRegion.I drawableRegion.h \
    graphicsOutputBase.I graphicsOutputBase.h \
    graphicsStateGuardianBase.h \
    renderBuffer.h

  #define INCLUDED_SOURCES \
    config_gsgbase.cxx \
    displayRegionBase.cxx \
    drawableRegion.cxx \
    graphicsOutputBase.cxx \
    graphicsStateGuardianBase.cxx

  #define INSTALL_HEADERS \
    displayRegionBase.I displayRegionBase.h \
    drawableRegion.I drawableRegion.h \
    graphicsOutputBase.I graphicsOutputBase.h \
    graphicsStateGuardianBase.h \
    renderBuffer.h

  #define IGATESCAN all

#end lib_target

#begin test_bin_target
  #define TARGET test_gsgbase
  #define LOCAL_LIBS \
    gsgbase

  #define SOURCES \
    test_gsgbase.cxx

#end test_bin_target

