
link_directories(${CMAKE_CURRENT_BINARY_DIR}/..)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../sem)
include_directories(.. ../sem)

include(FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit) 

add_library(testHelper SHARED test_main.cc)
target_link_libraries(testHelper ${CPPUNIT_LIBRARIES})

add_executable(chrono_test chrono_test.cc)
target_link_libraries(chrono_test testHelper chrono)

add_executable(fileLock_test fileLock_test.cc)
target_link_libraries(fileLock_test testHelper sem fileLock)

add_executable(sem_test sem_test.cc)
target_link_libraries(sem_test testHelper sem)

add_test(chrono_test chrono_test)
add_test(fileLock_test fileLock_test)
add_test(sem_test sem_test)

