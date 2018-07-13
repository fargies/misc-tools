
if(TESTS)

link_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${CPPUNIT_LIBRARY_DIRS})
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../sem
    ${CMAKE_BINARY_DIR}
    ${CPPUNIT_INCLUDE_DIRS})

add_library(testHelper SHARED test_main.cc)
target_link_libraries(testHelper ${CPPUNIT_LIBRARIES})

set(test_SRCS
    autoptr_test.cc chrono_test.cc
    dbg_test.cc extref_test.cc
    iterator_test.cc
    key_test.cc locker_test.cc
    mutex_test.cc refcounter_test.cc
    thread_test.cc
    test_main.cc)
if(NOT APPLE)
    set(test_SRCS ${test_SRCS} sem_test.cc
    dispatcher_fd_test.cc dispatcher_test.cc
    fileLock_test.cc
    timer_test.cc timer_thread_test.cc)
endif(NOT APPLE)

add_executable(test_all ${test_SRCS})
target_link_libraries(test_all testHelper cxx-misc-tools)

if(PYTHONLIBS_FOUND)
    target_link_libraries(test_all python++-tests)
else(PYTHONLIBS_FOUND)
    message(SEND_ERROR "Python not found")
endif(PYTHONLIBS_FOUND)

if(EXPAT)
    target_link_libraries(test_all expat++-tests)
endif(EXPAT)

add_test(test_all ${CMAKE_CURRENT_BINARY_DIR}/test_all)

endif(TESTS)
