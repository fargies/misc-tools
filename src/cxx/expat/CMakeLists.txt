
find_library(EXPAT expat)

if (EXPAT)

add_library(expat++ expat.cc expat++.hh)
target_link_libraries(expat++ expat)

link_directories(${CPPUNIT_LIBRARY_DIRS})
include_directories(${CPPUNIT_INCLUDE_DIRS})

add_library(expat++-tests SHARED expat_test.cc)
target_link_libraries(expat++-tests expat++ ${CPPUNIT_LIBRARIES})

endif (EXPAT)
