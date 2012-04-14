
file(GLOB threading_HDRS threading/*.hh)
file(GLOB threading_SRCS threading/*.hh threading/*.cc)
set(threading_DEPS pthread)

file(GLOB sem_HDRS sem/*.hh)
file(GLOB sem_SRCS sem/*.hh sem/*.cc)
set(sem_DEPS rt)

file(GLOB variant_HDRS variant/*.hh)
file(GLOB variant_SRCS variant/*.hh variant/*.cc)
set(variant_DEPS ${DBUS_CPP_LIBRARIES})
include_directories(${DBUS_CPP_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

set(cxx_misc_SRCS
    fileLock.cc fileLock.hh
    chrono.cc chrono.hh
    ${threading_SRCS} ${sem_SRCS} ${variant_SRCS})

add_library(cxx-misc-tools SHARED ${cxx_misc_SRCS})

target_link_libraries(cxx-misc-tools
    rt python++ expat++
    ${threading_DEPS} ${sem_DEPS} ${variant_DEPS})

set_target_properties(cxx-misc-tools PROPERTIES LINK_INTERFACE_LIBRARIES "")

add_subdirectory(bench)
add_subdirectory(expat)
add_subdirectory(python)
add_subdirectory(tests)

