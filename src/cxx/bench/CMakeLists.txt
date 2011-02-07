
link_directories(${CMAKE_CURRENT_BINARY_DIR}/..)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../sem)
include_directories(.. ../sem)

add_executable(rwLock_bench rwLock_bench.cc)
target_link_libraries(rwLock_bench testHelper rwLock chrono)

add_executable(fileLock_bench fileLock_bench.cc)
target_link_libraries(fileLock_bench testHelper fileLock chrono)

