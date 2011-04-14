// DIR_TYPE "metalib" indicates we are building a shared library that
// consists mostly of references to other shared libraries.  Under
// Windows, this directly produces a DLL (as opposed to the regular
// src libraries, which don't produce anything but a pile of OBJ files
// under Windows).

#define DIR_TYPE metalib
#define BUILD_DIRECTORY $[HAVE_CEGUI]
#define BUILDING_DLL BUILDING_PANDACEGUI
 
#define COMPONENT_LIBS \
    pcegui

#define LOCAL_LIBS pgraph
#define OTHER_LIBS interrogatedb:c dconfig:c dtoolconfig:m \
	           dtoolutil:c dtoolbase:c dtool:m prc:c


#begin metalib_target
#define TARGET pandacegui

  #define SOURCES pandacegui.cxx pandacegui.h
  #define INSTALL_HEADERS pandacegui.h

#end metalib_target

