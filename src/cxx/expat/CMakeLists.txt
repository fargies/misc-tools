
find_library(EXPAT expat)

if (EXPAT)

add_library(expat++ expat.cc expat++.hh)
target_link_libraries(expat++ ${})

add_executable(expat_test expat_test.cc)
target_link_libraries(expat_test testHelper expat++)

add_test(expat_test ${CMAKE_CURRENT_BINARY_DIR}/expat_test)

endif (EXPAT)
