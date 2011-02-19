
add_library(chrono chrono.cc chrono.hh)
target_link_libraries(chrono rt)

add_library(fileLock fileLock.cc fileLock.hh)
target_link_libraries(fileLock rt)

add_subdirectory(sem)
add_subdirectory(tests)
add_subdirectory(bench)
add_subdirectory(variant)

add_subdirectory(expat)
