project(Panda3D)

cmake_minimum_required(VERSION 2.6)

include_directories("${PROJECT_BINARY_DIR}")

# Build options
set(OPTIMIZE 3 CACHE STRING "Optimization level, 1-4")

option(USE_PYTHON "Enable Python support" ON)
if(USE_PYTHON)
  find_package(PythonLibs)
  find_package(PythonInterp)
  if(PYTHONLIBS_FOUND)
    set(HAVE_PYTHON TRUE)
    INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIR}")
  endif(PYTHONLIBS_FOUND)
endif(USE_PYTHON)

option(USE_OPENGL "Build the OpenGL renderer" ON)
if(USE_OPENGL)
  find_package(OpenGL)
  if(OPENGL_FOUND)
    set(HAVE_GL TRUE)
  endif(OPENGL_FOUND)
endif(USE_OPENGL)

option(HAVE_TINYDISPLAY "Build the software renderer" ON)
option(USE_NVIDIACG "Build the NVIDIA Cg support module" ON)
option(USE_OPENAL "Build the OpenAL audio support" ON)
if(USE_OPENAL)
  find_package(OpenAL)
  if(OPENAL_FOUND)
    set(HAVE_OPENAL TRUE)
  endif(OPENAL_FOUND)
endif(USE_OPENAL)

option(USE_FMODEX "Build the FMOD EX audio support" ON)
option(USE_FFMPEG "Enable support for ffmpeg" ON)
option(USE_ODE "Build the module for ODE physics support" ON)
option(USE_PHYSX "Build the module for PhysX physics support" ON)
option(USE_ZLIB "Enable zlib support" ON)
if(USE_ZLIB)
  find_package(ZLIB)
  if(ZLIB_FOUND)
    set(HAVE_ZLIB TRUE)
  endif(ZLIB_FOUND)
endif(USE_ZLIB)

option(USE_PNG "Enable PNG image support" ON)
if(USE_PNG)
  find_package(PNG)
  if(PNG_FOUND)
    set(HAVE_PNG TRUE)
  endif(PNG_FOUND)
endif(USE_PNG)

option(USE_JPEG "Enable jpeg image support" ON)
if(USE_JPEG)
  find_package(JPEG)
  if(JPEG_FOUND)
    set(HAVE_JPEG TRUE)
  endif(JPEG_FOUND)
endif(USE_JPEG)

option(USE_TIFF "Enable tiff image support" ON)
if(USE_TIFF)
  find_package(TIFF)
  if(TIFF_FOUND)
    set(HAVE_TIFF TRUE)
  endif(TIFF_FOUND)
endif(USE_TIFF)

option(USE_SQUISH "Enable DXT compression support" ON)
if(USE_SQUISH)
  find_path(SQUISH_INCLUDE_DIR squish.h)
  find_library(SQUISH_LIBRARY NAMES squish)
  if(SQUISH_INCLUDE_DIR AND SQUISH_LIBRARY)
    set(SQUISH_FOUND TRUE)
    set(HAVE_SQUISH TRUE)
  else()
    set(SQUISH_FOUND FALSE)
  endif()
endif(USE_SQUISH)

option(USE_FREETYPE "Enable freetype support for font rendering" ON)
if(USE_FREETYPE)
  find_package(Freetype)
  if(FREETYPE_FOUND)
    set(HAVE_FREETYPE TRUE)
  endif(FREETYPE_FOUND)
endif(USE_FREETYPE)

option(USE_OPENSSL "Enable OpenSSL support" ON)
if(USE_OPENSSL)
  find_package(OpenSSL)
  if(OPENSSL_FOUND)
    set(HAVE_OPENSSL TRUE)
  endif(OPENSSL_FOUND)
endif(USE_OPENSSL)

if(UNIX)
  option(USE_X11 "Enable X11 support" ON)
  if(USE_X11)
    find_package(X11)
    if(X11_FOUND)
      set(HAVE_X11 TRUE)
    endif(X11_FOUND)
  endif(USE_X11)
endif(UNIX)

find_package(BISON)

option(HAVE_INTERROGATE "Enable building of interrogate" ON)
option(INTERROGATE_VERBOSE "Enables verbose debugging output of interrogate" OFF)

set(DEFAULT_PRC_DIR "${CMAKE_INSTALL_PREFIX}/panda3d/etc" CACHE PATH "")
set(PRC_DIR_ENVVARS "PANDA_PRC_DIR" CACHE STRING "")
set(PRC_PATH_ENVVARS "PANDA_PRC_PATH" CACHE STRING "")
set(PRC_PATH2_ENVVARS "" CACHE STRING "")
set(PRC_PATTERNS "*.prc" CACHE STRING "")
set(PRC_ENCRYPTED_PATTERNS "*.prc.pe" CACHE STRING "")
set(PRC_ENCRYPTION_KEY "" CACHE STRING "")
set(PRC_EXECUTABLE_PATTERNS "" CACHE STRING "")
set(PRC_EXECUTABLE_ARGS_ENVVAR "PANDA_PRC_XARGS" CACHE STRING "")
set(PRC_PUBLIC_KEYS_FILENAME "" CACHE STRING "")
set(PRC_RESPECT_TRUST_LEVEL CACHE STRING "")
set(PRC_DCONFIG_TRUST_LEVEL 0 CACHE STRING "")
set(PRC_INC_TRUST_LEVEL 0 CACHE STRING "")

set(PANDA_VERSION 2 0 0 CACHE LIST
  "Use semicolons to separate the major, minor, and sequence numbers.")

option(PANDA_OFFICIAL_VERSION
  "This variable will be defined to false in the CVS repository, but
scripts that generate source tarballs and/or binary releases for
distribution, by checking out Panda from an official CVS tag,
should explictly set this to true.  When false, it indicates that
the current version of Panda was checked out from CVS, so it may
not be a complete representation of the indicated version."
  OFF)

set(PANDA_DISTRIBUTOR homebuilt CACHE STRING
  "This string is reported verbatim by PandaSystem::get_distributor().
It should be set by whoever provides a particular distribution of
Panda.  If you build your own Panda, leave this unchanged.")

set(PANDA_PACKAGE_VERSION CACHE STRING
  "This string is used to describe the Panda3D \"package\" associated
with this current build of Panda.  It should increment with major
and minor version changes, but not sequence (or \"bugfix\") changes.
It should be unique for each unique distributor.  The default is
the empty string, which means this build does not precisely match
any distributable Panda3D packages.  If you are making a Panda3D
build which you will be using to produce a distributable Panda3D
package, you should set this string appropriately.")

set(P3D_PLUGIN_VERSION 1 1 0 CACHE LIST
  "We also define a version for the Panda3D plugin/runtime,
i.e. nppanda3d.dll, p3dactivex.ocx, and panda3d.exe.  This is an
independent version number from PANDA_VERSION or
PANDA_PACKAGE_VERSION, because it is anticipated that this plugin
code, once settled, will need to be updated much less frequently
than Panda itself.")

list(GET PANDA_VERSION 0 PANDA_MAJOR_VERSION)
list(GET PANDA_VERSION 1 PANDA_MINOR_VERSION)
list(GET PANDA_VERSION 2 PANDA_SEQUENCE_VERSION)

if(PANDA_OFFICIAL_VERSION)
  set(PANDA_VERSION_STR "${PANDA_MAJOR_VERSION}.${PANDA_MINOR_VERSION}.${PANDA_SEQUENCE_VERSION}")
else(PANDA_OFFICIAL_VERSION)
  set(PANDA_VERSION_STR "${PANDA_MAJOR_VERSION}.${PANDA_MINOR_VERSION}.${PANDA_SEQUENCE_VERSION}c")
endif(PANDA_OFFICIAL_VERSION)
set(PANDA_VERSION_SYMBOL "panda_version_${PANDA_MAJOR_VERSION}_${PANDA_MINOR_VERSION}")

# The panda version as a single number, with three digits reserved
# for each component.
#define PANDA_NUMERIC_VERSION $[+ $[* $[PANDA_MAJOR_VERSION],1000000],$[* $[PANDA_MINOR_VERSION],1000],$[PANDA_SEQUENCE_VERSION]]

list(GET P3D_PLUGIN_VERSION 0 P3D_PLUGIN_MAJOR_VERSION)
list(GET P3D_PLUGIN_VERSION 1 P3D_PLUGIN_MINOR_VERSION)
list(GET P3D_PLUGIN_VERSION 2 P3D_PLUGIN_SEQUENCE_VERSION)

if(PANDA_OFFICIAL_VERSION)
  set(P3D_PLUGIN_VERSION_STR "${P3D_PLUGIN_MAJOR_VERSION}.${P3D_PLUGIN_MINOR_VERSION}.${P3D_PLUGIN_SEQUENCE_VERSION}")
else(PANDA_OFFICIAL_VERSION)
  set(P3D_PLUGIN_VERSION_STR "${P3D_PLUGIN_MAJOR_VERSION}.${P3D_PLUGIN_MINOR_VERSION}.${P3D_PLUGIN_SEQUENCE_VERSION}c")
endif(PANDA_OFFICIAL_VERSION)

# The plugin version as dot-delimited and comma-delimited integer quads,
# according to MS conventions for DLL version numbers.
if(PANDA_OFFICIAL_VERSION)
  set(P3D_PLUGIN_DLL_DOT_VERSION   "${P3D_PLUGIN_MAJOR_VERSION}.${P3D_PLUGIN_MINOR_VERSION}.${P3D_PLUGIN_SEQUENCE_VERSION}.1000")
  set(P3D_PLUGIN_DLL_COMMA_VERSION "${P3D_PLUGIN_MAJOR_VERSION}.${P3D_PLUGIN_MINOR_VERSION}.${P3D_PLUGIN_SEQUENCE_VERSION}.1000")
else(PANDA_OFFICIAL_VERSION)
  set(P3D_PLUGIN_DLL_DOT_VERSION   "${P3D_PLUGIN_MAJOR_VERSION}.${P3D_PLUGIN_MINOR_VERSION}.${P3D_PLUGIN_SEQUENCE_VERSION}.0")
  set(P3D_PLUGIN_DLL_COMMA_VERSION "${P3D_PLUGIN_MAJOR_VERSION},${P3D_PLUGIN_MINOR_VERSION},${P3D_PLUGIN_SEQUENCE_VERSION},0")
endif(PANDA_OFFICIAL_VERSION)

include(PandaFunctions.cmake)

add_subdirectory(dtool)
add_subdirectory(panda)

panda_add_library(core OPENSSL)
panda_add_library(pystub)
panda_add_library(interrogatedb core)

if(HAVE_OPENSSL)
panda_add_executable(make-prc-key core pystub OPENSSL)
endif(HAVE_OPENSSL)

panda_add_executable(interrogate
  interrogatedb core pystub)
panda_add_executable(interrogate_module
  interrogatedb core pystub)

panda_wrap_library(core)
panda_wrap_library(interrogatedb)
