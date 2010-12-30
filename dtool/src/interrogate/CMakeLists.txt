if(HAVE_INTERROGATE)

include_directories(../cppparser ../interrogatedb)

add_executable(interrogate interrogate_composite1.cxx interrogate_composite2.cxx)
target_link_libraries(interrogate cppParser p3interrogatedb p3core p3pystub)
target_link_libraries(interrogate ${OPENSSL_LIBRARIES} ${CMAKE_DL_LIBS})

add_executable(interrogate_module interrogate_module.cxx)
target_link_libraries(interrogate_module p3interrogatedb p3core p3pystub)
target_link_libraries(interrogate_module ${OPENSSL_LIBRARIES} ${CMAKE_DL_LIBS})

add_executable(parse_file parse_file.cxx)
target_link_libraries(parse_file cppParser p3core p3pystub)
target_link_libraries(parse_file ${OPENSSL_LIBRARIES} ${CMAKE_DL_LIBS})

install(TARGETS interrogate interrogate_module parse_file DESTINATION bin)

endif(HAVE_INTERROGATE)
