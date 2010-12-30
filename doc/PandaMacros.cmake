# Figure out which interrogate options to use.
set(IMOD_FLAGS ${INTERROGATE_MODULE_OPTIONS})
set(IGATE_FLAGS ${INTERROGATE_OPTIONS} -DCPPPARSER -D__cplusplus -Dvolatile -Dmutable)
if(INTERROGATE_VERBOSE)
  list(APPEND IGATE_FLAGS "-v")
endif(INTERROGATE_VERBOSE)

if(WIN32)
  list(APPEND IGATE_FLAGS -longlong __int64 -D_X86_ -D__STDC__=1 -DWIN32_VC -D"_declspec(param)=" -D"__declspec(param)=" -D_near  -D_far -D__near  -D__far -D_WIN32 -D__stdcall -DWIN32)
endif(WIN32)

if(UNIX)
  file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/include/parser-inc/")
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dM -E - INPUT_FILE /dev/null OUTPUT_FILE "${PROJECT_BINARY_DIR}/include/parser-inc/igate_system_config.h")
endif(UNIX)

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/panda3d/")
file(WRITE "${PROJECT_BINARY_DIR}/panda3d/__init__.py" "")

macro(building_dll name)
  add_definitions(-DBUILDING_${name})
endmacro(building_dll)

# We provide this macro so that we can put required header
# files into the project binary directory, to avoid having
# a lot of include_directories() across directories.
macro(install_headers)
  foreach(header ${ARGV})
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${header}")
      install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${header}" DESTINATION include/panda3d)
      configure_file("${CMAKE_CURRENT_BINARY_DIR}/${header}" "${PROJECT_BINARY_DIR}/include/${header}" COPYONLY)
    else()
      install(FILES "${header}" DESTINATION include/panda3d)
      configure_file("${header}" "${PROJECT_BINARY_DIR}/include/${header}" COPYONLY)
    endif()
  endforeach(header)
endmacro(install_headers)

# This doesn't actually invoke interrogate.  It merely adds the
# sources to the list of scan sources associated with the target.
# Interrogate will be invoked when add_python_module is called.
macro(interrogate target module)
  if(HAVE_PYTHON AND HAVE_INTERROGATE)

    set(sources ${ARGV})
    list(REMOVE_AT sources 0 1)

    # Find any .N files that would normally be picked up by interrogate.
    # We let CMake add these as dependencies too, to allow rebuilding
    # the wrappers when the .N files have been modified.
    set(deps)
    foreach(source ${sources})
      get_filename_component(basename "${source}" NAME_WE)
      if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${basename}.N")
        list(APPEND deps "${CMAKE_CURRENT_SOURCE_DIR}/${basename}.N")
      endif()

      list(APPEND deps "${CMAKE_CURRENT_SOURCE_DIR}/${source}")
    endforeach(source)

    set_target_properties("${target}" PROPERTIES IGATE_SOURCES "${sources}")
    set_target_properties("${target}" PROPERTIES IGATE_SRCDIR "${CMAKE_CURRENT_SOURCE_DIR}")
    set_target_properties("${target}" PROPERTIES IGATE_DEPS "${deps}")
  endif()
endmacro(interrogate)

macro(add_python_module module)
  if(HAVE_PYTHON AND HAVE_INTERROGATE)
    set(targets ${ARGV})
    list(REMOVE_AT targets 0)
    set(infiles)
    set(sources)

    foreach(target ${targets})
      get_target_property(scansrc "${target}" IGATE_SOURCES)
      get_target_property(srcdir "${target}" IGATE_SRCDIR)
      get_target_property(deps "${target}" IGATE_DEPS)

      add_custom_command(
        OUTPUT "${target}_igate.cxx" "${target}.in"
        COMMAND interrogate
          -od "${target}.in"
          -oc "${target}_igate.cxx"
          -module ${module} -library ${target} ${IGATE_FLAGS}
          -srcdir "${srcdir}"
          -I "${PROJECT_BINARY_DIR}/include"
          -S "${PROJECT_SOURCE_DIR}/dtool/src/parser-inc"
          -S "${PROJECT_BINARY_DIR}/include/parser-inc"
          ${scansrc}
        DEPENDS interrogate ${deps}
      )

      list(APPEND infiles "${target}.in")
      list(APPEND sources "${target}_igate.cxx")
    endforeach(target)

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${module}_module.cxx"
      COMMAND interrogate_module
        -oc "${CMAKE_CURRENT_BINARY_DIR}/${module}_module.cxx"
        -module ${module} -library ${module}
        ${IMOD_FLAGS} ${infiles}
      DEPENDS interrogate_module ${infiles}
    )

    add_library(${module} MODULE "${module}_module.cxx" ${sources})
    target_link_libraries(${module} ${PYTHON_LIBRARIES})

    set_target_properties(${module} PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/panda3d"
      PREFIX ""
    )
    if(WIN32 AND NOT CYGWIN)
      set_target_properties(${module} PROPERTIES SUFFIX ".pyd")
    endif(WIN32 AND NOT CYGWIN)
  endif()
endmacro(add_python_module)
