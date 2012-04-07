
link_directories(${CMAKE_CURRENT_BINARY_DIR}/..)
include_directories(.. ../sem ${CMAKE_BINARY_DIR})

include(FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit)

add_library(testHelper SHARED test_main.cc)
target_link_libraries(testHelper ${CPPUNIT_LIBRARIES})

set(test_SRCS
    chrono_test.cc
    sem_test.cc
    fileLock_test.cc
    iterator_test.cc
    mutex_test.cc)

add_executable(test_all ${test_SRCS})
target_link_libraries(test_all testHelper cxx-misc-tools)

if(PYTHONLIBS_FOUND)
    target_link_libraries(test_all python++-tests)
endif(PYTHONLIBS_FOUND)

if(EXPAT)
    target_link_libraries(test_all expat++-tests)
endif(EXPAT)

add_test(test_all ${CMAKE_CURRENT_BINARY_DIR}/test_all)

