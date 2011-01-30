
set(chrono_SRCS
    chrono.cc chrono.hh)
add_library(chrono ${chrono_SRCS})
target_link_libraries(chrono rt)

include(FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit) 

add_library(testHelper SHARED test_main.cc)
target_link_libraries(testHelper ${CPPUNIT_LIBRARIES})

add_executable(chrono_test chrono_test.cc)
target_link_libraries(chrono_test testHelper chrono)

install(TARGETS chrono_test DESTINATION bin)

add_test(chrono_test chrono_test)

