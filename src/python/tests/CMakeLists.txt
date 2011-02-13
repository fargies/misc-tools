
link_directories(${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_BINARY_DIR}/src/cxx/tests)
include_directories(.. ${PYTHON_INCLUDE_DIR})

add_executable(PyValue_test PyValue_test.cc)
target_link_libraries(PyValue_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_executable(PyDict_test PyDict_test.cc)
target_link_libraries(PyDict_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_test(PyValue_test ${CMAKE_CURRENT_BINARY_DIR}/PyValue_test)
add_test(PyDict_test ${CMAKE_CURRENT_BINARY_DIR}/PyDict_test)
