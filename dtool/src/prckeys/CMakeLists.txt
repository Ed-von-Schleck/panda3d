if(HAVE_OPENSSL)

add_executable(make-prc-key makePrcKey.cxx)
use_libraries(make-prc-key OPENSSL)
target_link_libraries(make-prc-key p3core p3pystub)

endif(HAVE_OPENSSL)
