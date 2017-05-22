
link_directories(
    ${CPPUNIT_LIBRARY_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}/..)
include_directories(
    ${CPPUNIT_INCLUDE_DIRS}
    ..
    ${PYTHON_INCLUDE_DIR})

set(python_test_SRCS
    PyValue_test.cc
    PyDict_test.cc
    PySeq_test.cc)

add_library(python++-tests SHARED ${python_test_SRCS})
target_link_libraries(python++-tests python++ ${CPPUNIT_LIBRARIES})
