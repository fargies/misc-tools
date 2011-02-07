
add_executable(iterator_test iterator_test.cc)
target_link_libraries(iterator_test testHelper)

add_test(iterator_test ${CMAKE_BINARY_DIR}/src/cxx/iterator_test)

add_subdirectory(sem)
add_subdirectory(tests)
add_subdirectory(bench)

