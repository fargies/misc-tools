cmake_minimum_required(VERSION 2.8)

project(misc-tools CXX C)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(GitVersion)
GitVersion(1 0)

include(Policies)
set_policy(CMP0020 NEW)
set_policy(CMP0042 NEW)
set_policy(CMP0043 NEW)

include(Dependencies)

if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wno-long-long")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Woverloaded-virtual -Wunused-parameter -Wuninitialized")
endif (NOT MSVC)

configure_file("config.h.in" "config.h")

enable_testing()
include(CTest)
include(Lint)

add_subdirectory(src)
add_subdirectory(doc)
