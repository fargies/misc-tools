
set(cxx_misc_SRCS
    fileLock.cc fileLock.hh
    chrono.cc chrono.hh)
add_library(cxx-misc-tools SHARED ${cxx_misc_SRCS})
target_link_libraries(cxx-misc-tools
    rt expat++ sem rwLock threading Variant
    python++)

add_subdirectory(sem)
add_subdirectory(tests)
add_subdirectory(bench)
add_subdirectory(variant)
add_subdirectory(threading)
add_subdirectory(expat)
add_subdirectory(python)

