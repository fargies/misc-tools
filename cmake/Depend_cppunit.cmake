
option(TESTS "Compile and run unit tests" ON)

if(TESTS)

    include(FindPkgConfig)
    pkg_check_modules(CPPUNIT cppunit)
    add_custom_target(test-verbose
        COMMAND ${CMAKE_CTEST_COMMAND} --verbose
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

    enable_testing()

endif()
