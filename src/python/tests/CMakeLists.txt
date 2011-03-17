
link_directories(${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_BINARY_DIR}/src/cxx/tests)
include_directories(.. ${PYTHON_INCLUDE_DIR})

add_executable(PyValue_test PyValue_test.cc)
target_link_libraries(PyValue_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_executable(PyDict_test PyDict_test.cc)
target_link_libraries(PyDict_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_executable(PySeq_test PySeq_test.cc)
target_link_libraries(PySeq_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_executable(PyList_test PyList_test.cc)
target_link_libraries(PyList_test testHelper python-c++ ${PYTHON_LIBRARIES})

add_test(PyValue_test ${CMAKE_CURRENT_BINARY_DIR}/PyValue_test)
add_test(PyDict_test ${CMAKE_CURRENT_BINARY_DIR}/PyDict_test)
add_test(PySeq_test ${CMAKE_CURRENT_BINARY_DIR}/PySeq_test)
add_test(PyList_test ${CMAKE_CURRENT_BINARY_DIR}/PyList_test)

