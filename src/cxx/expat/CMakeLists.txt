
find_library(EXPAT expat)

if (EXPAT)

add_library(expat++ expat.cc expat++.hh)
target_link_libraries(expat++ expat)

add_library(expat++-tests SHARED expat_test.cc)
target_link_libraries(expat++-tests expat++)

endif (EXPAT)

