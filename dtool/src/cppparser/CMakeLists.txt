if(HAVE_INTERROGATE)

# This is a bit ugly, but CMake doesn't make it easier for us.
if(BISON_FOUND)
  add_custom_command(
    OUTPUT cppBison.h cppBison.cxx
    COMMAND ${BISON_EXECUTABLE}
      --defines=cppBison.h -o cppBison.cxx -p cppyy
      "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.yxx"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.yxx"
  )
else()
  add_custom_command(
    OUTPUT cppBison.h cppBison.cxx
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.h.prebuilt" cppBison.h
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.cxx.prebuilt" cppBison.cxx
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.h.prebuilt" "${CMAKE_CURRENT_SOURCE_DIR}/cppBison.cxx.prebuilt"
  )
endif()

set(sources cppParser_composite1.cxx cppParser_composite2.cxx cppBison.cxx)

add_library(cppParser STATIC ${sources})

endif(HAVE_INTERROGATE)
