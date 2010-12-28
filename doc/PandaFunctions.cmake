set(LIB_PREFIX "p3")
if(INTERROGATE_VERBOSE)
  list(APPEND SYSTEM_IGATE_FLAGS "-v")
endif(INTERROGATE_VERBOSE)
list(APPEND SYSTEM_IGATE_FLAGS -DCPPPARSER -D__STDC__=1 -D__cplusplus -D__x86_64 -D_LP64 -D__const=const -Dvolatile -Dmutable -fnames -string -refcount -assert -python-native)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

macro(panda_add_sources module) # sources
  set(sources ${ARGV})
  list(REMOVE_AT sources 0)
  foreach(source ${sources})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
      set(_PANDA_${module}_SOURCES "${_PANDA_${module}_SOURCES};${CMAKE_CURRENT_SOURCE_DIR}/${source}" CACHE INTERNAL "")
    else()
      set(_PANDA_${module}_SOURCES "${_PANDA_${module}_SOURCES};${CMAKE_CURRENT_BINARY_DIR}/${source}" CACHE INTERNAL "")
    endif()
  endforeach(source)

  set(_PANDA_${module}_INCDIRS "${_PANDA_${module}_INCDIRS};${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "")
  set(_PANDA_${module}_INCDIRS "${_PANDA_${module}_INCDIRS};${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "")

  set(${module}_INCLUDE_DIRS "${_PANDA_${module}_INCDIRS}" CACHE INTERNAL "")
endmacro(panda_add_sources)

macro(panda_add_scan_sources module) # sources
  set(sources ${ARGV})
  list(REMOVE_AT sources 0)
  foreach(source ${sources})
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${source}")
      set(_PANDA_${module}_SCAN_SOURCES "${_PANDA_${module}_SCAN_SOURCES};${CMAKE_CURRENT_BINARY_DIR}/${source}" CACHE INTERNAL "")
    else()
      set(_PANDA_${module}_SCAN_SOURCES "${_PANDA_${module}_SCAN_SOURCES};${CMAKE_CURRENT_SOURCE_DIR}/${source}" CACHE INTERNAL "")
    endif()
  endforeach(source)
endmacro(panda_add_scan_sources)

macro(_panda_process_sources var)
  set(_sources ${ARGV})
  list(REMOVE_AT _sources 0)

  set("${var}")
  foreach(source ${_sources})
    get_filename_component(ext "${source}" EXT)
    get_filename_component(name "${source}" NAME_WE)
    get_filename_component(path "${source}" PATH)

    if(ext MATCHES ".yxx")
      if(BISON_FOUND)
        bison_target("${name}" "${source}" "${CMAKE_CURRENT_BINARY_DIR}/${name}.cxx")
        add_custom_command(
          OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${name}.h"
          COMMAND ${CMAKE_COMMAND} -E copy "${BISON_${name}_OUTPUT_HEADER}" "${CMAKE_CURRENT_BINARY_DIR}/${name}.h"
          DEPENDS "${BISON_${name}_OUTPUT_HEADER}"
        )
      else()
        add_custom_command(
          OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/${name}.h"
            "${CMAKE_CURRENT_BINARY_DIR}/${name}.cxx"
          COMMAND ${CMAKE_COMMAND} -E copy "${path}/${name}.h.prebuilt" "${CMAKE_CURRENT_BINARY_DIR}/${name}.h"
          COMMAND ${CMAKE_COMMAND} -E copy "${path}/${name}.cxx.prebuilt" "${CMAKE_CURRENT_BINARY_DIR}/${name}.cxx"
          DEPENDS "${path}/${name}.h.prebuilt" "${path}/${name}.cxx.prebuilt"
        )
      endif()
      list(APPEND "${var}" "${name}.cxx" "${name}.h")
    else()
      list(APPEND "${var}" "${source}")
    endif()
  endforeach(source)
endmacro(_panda_process_sources)

macro(panda_add_library module) # deps
  set(${module}_LIBRARIES "${LIB_PREFIX}${module}")

  foreach(dep ${ARGV})
    include_directories(${${dep}_INCLUDE_DIRS})
    set(_PANDA_${module}_WRAP_INCDIRS ${_PANDA_${module}_WRAP_INCDIRS} ${${dep}_INCLUDE_DIRS} CACHE INTERNAL "")
  endforeach(dep)

  string(TOUPPER "${module}" module_uc)

  _panda_process_sources(sources ${_PANDA_${module}_SOURCES})

  add_library("${LIB_PREFIX}${module}" SHARED ${sources})
  set_target_properties("${LIB_PREFIX}${module}" PROPERTIES
    VERSION   "${PANDA_MAJOR_VERSION}.${PANDA_MINOR_VERSION}.${PANDA_SEQUENCE_VERSION}"
    SOVERSION "${PANDA_MAJOR_VERSION}.${PANDA_MINOR_VERSION}"
    DEFINE_SYMBOL "BUILDING_${module_uc}")

  set(deps ${ARGV})
  list(REMOVE_AT deps 0)
  foreach(dep ${deps})
    target_link_libraries("${LIB_PREFIX}${module}" ${${dep}_LIBRARIES})
  endforeach(dep)

  set(_PANDA_${module}_SOURCES CACHE INTERNAL "")
  set(_PANDA_${module}_INCDIRS CACHE INTERNAL "")
endmacro(panda_add_library)

macro(panda_add_executable exec) # deps
  foreach(dep ${ARGV})
    include_directories(${${dep}_INCLUDE_DIRS})
  endforeach(dep)

  _panda_process_sources(sources ${_PANDA_${exec}_SOURCES})

  add_executable("${exec}" ${sources})
  set_target_properties("${exec}" PROPERTIES
    VERSION "${PANDA_MAJOR_VERSION}.${PANDA_MINOR_VERSION}.${PANDA_SEQUENCE_VERSION}")

  set(deps ${ARGV})
  list(REMOVE_AT deps 0)
  foreach(dep ${deps})
    target_link_libraries("${exec}" ${${dep}_LIBRARIES})
  endforeach(dep)

  set(_PANDA_${exec}_SOURCES CACHE INTERNAL "")
  set(_PANDA_${exec}_INCDIRS CACHE INTERNAL "")
endmacro(panda_add_executable)

macro(panda_wrap_library module)
  if(HAVE_PYTHON AND HAVE_INTERROGATE)

    set(iflags "-I${PYTHON_INCLUDE_DIRS}")
    foreach(incdir ${_PANDA_${module}_WRAP_INCDIRS})
      list(APPEND iflags "-I${incdir}")
    endforeach(incdir)

    # Find any .N files that would normally be picked up by interrogate.
    # We let CMake add these as dependencies too, to allow rebuilding
    # the wrappers when the .N files have been modified.
    set(nfiles)
    foreach(source ${_PANDA_${module}_SCAN_SOURCES})
      get_filename_component(path "${source}" PATH)
      get_filename_component(nfile "${source}" NAME_WE)
      set(nfile "${path}/${nfile}.N")
      if(EXISTS "${nfile}")
        list(APPEND nfiles "${nfile}")
      endif()
    endforeach(source)

    add_custom_command(
      OUTPUT ${module}_wrap.cxx ${module}.in
      COMMAND bin/interrogate -od "${module}.in" -oc "${module}_wrap.cxx" ${SYSTEM_IGATE_FLAGS} -module ${module} -do-module -S${PROJECT_SOURCE_DIR}/dtool/src/parser-inc ${iflags} ${_PANDA_${module}_SCAN_SOURCES}
      DEPENDS interrogate ${LIB_PREFIX}${module} ${_PANDA_${module}_SCAN_SOURCES} ${nfiles}
    )

    include_directories(${_PANDA_${module}_WRAP_INCDIRS})
    add_library(${module} MODULE ${CMAKE_CURRENT_BINARY_DIR}/${module}_wrap.cxx)
    set_target_properties(${module} PROPERTIES PREFIX "")
    if(WIN32 AND NOT CYGWIN)
      set_target_properties(${module} PROPERTIES SUFFIX ".pyd")
    endif(WIN32 AND NOT CYGWIN)
    target_link_libraries(${module} ${PYTHON_LIBRARIES} ${LIB_PREFIX}interrogatedb)

    set(_PANDA_${module}_WRAP_INCDIRS CACHE INTERNAL "")
    set(_PANDA_${module}_SCAN_SOURCES CACHE INTERNAL "")
  endif(HAVE_PYTHON AND HAVE_INTERROGATE)
endmacro(panda_wrap_library)
