cmake_minimum_required(VERSION 2.8)

project(misc-tools CXX C)

include(FindPkgConfig)
pkg_check_modules(DBUS_CPP "dbus-c++-1 >= 0.6.0-pre1")

include(CheckFunctionExists)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

configure_file("config.h.in"
               "config.h")

include_directories(.)

enable_testing()
include(CTest)

set(CTEST_PROJECT_NAME "misc-tools")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "minia.ath.cx")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=misc-tools")
set(CTEST_DROP_SITE_CDASH TRUE)

add_subdirectory(src)

add_subdirectory(doc)

