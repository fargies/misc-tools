cmake_minimum_required(VERSION 2.8)

project(misc-tools CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(GitVersion)
GitVersion(1 0)

include(Policies)
set_policy(CMP0042 NEW)

include(FindPkgConfig)
pkg_check_modules(DBUS_CPP "dbus-c++-1 >= 0.6.0-pre1")

include(CheckFunctionExists)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

configure_file("config.h.in" "config.h")

include_directories(.)

enable_testing()
include(CTest)

add_subdirectory(src)
add_subdirectory(doc)
