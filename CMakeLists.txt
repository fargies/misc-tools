cmake_minimum_required(VERSION 2.8)

project(misc-tools CXX)

enable_testing()
include(CTest)

set(CTEST_PROJECT_NAME "misc-tools")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")
set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "minia.ath.cx")
set(CTEST_DROP_LOCATION "/CDash/submit.php?project=misc-tools")
set(CTEST_DROP_SITE_CDASH TRUE)
set(CTEST_OUTPUT_ON_FAILURE TRUE)

add_subdirectory(src)

