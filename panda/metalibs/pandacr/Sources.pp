// DIR_TYPE "metalib" indicates we are building a shared library that
// consists mostly of references to other shared libraries.  Under
// Windows, this directly produces a DLL (as opposed to the regular
// src libraries, which don't produce anything but a pile of OBJ files
// under Windows).

#define DIR_TYPE metalib
#define BUILDING_DLL BUILDING_PANDACR
#define DIRECTORY_IF_CHROMIUM yes

//#define USE_CHROMIUM yes

#define COMPONENT_LIBS \
    crgsg wcrdisplay

#define LOCAL_LIBS gsgbase display express
#define OTHER_LIBS dtoolconfig dtool
//#define WIN_SYS_LIBS $[WIN_SYS_LIBS] 

#begin metalib_target
  #define TARGET pandacr

  #define SOURCES pandacr.cxx pandacr.h
  #define INSTALL_HEADERS pandacr.h

#end metalib_target
