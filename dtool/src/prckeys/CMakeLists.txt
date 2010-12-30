if(HAVE_OPENSSL)

add_executable(make-prc-key makePrcKey.cxx)
target_link_libraries(make-prc-key p3core p3pystub)
target_link_libraries(make-prc-key ${OPENSSL_LIBRARIES})

endif(HAVE_OPENSSL)
