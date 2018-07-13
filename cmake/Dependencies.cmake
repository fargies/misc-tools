
include(CheckFunctionExists)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

include(Depend_cppunit)

include(FindPkgConfig)
pkg_check_modules(DBUS_CPP "dbus-c++-1 >= 0.6.0-pre1")
