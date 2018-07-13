
# Configuration
list(APPEND INCDIRS ${QT_INCLUDES} ${CPPUNIT_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
list(APPEND SRCDIRS src tests)

# Clang configuration
find_program(CLANGPP_EXE NAMES clang++
    PATHS ${CLANGPP_EXE} ENV PATH $ENV{CLANGPP_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH)
if(CLANGPP_EXE)
    message(STATUS "Clang++ linter found")
    set(CLANG_COMPLETE "${CMAKE_SOURCE_DIR}/.clang_complete")

    file(WRITE "${CLANG_COMPLETE}" "")
    file(APPEND "${CLANG_COMPLETE}" "--std=c++11\n")
    file(APPEND "${CLANG_COMPLETE}" "-I./src\n")
    file(APPEND "${CLANG_COMPLETE}" "-I${CMAKE_BINARY_DIR}/src\n")
    file(APPEND "${CLANG_COMPLETE}" "-I${SYSROOT}/usr/include\n")
    file(APPEND "${CLANG_COMPLETE}" "-I${SYSROOT}/usr/local/include\n")

    foreach(DIR ${INCDIRS})
        file(APPEND "${CMAKE_SOURCE_DIR}/.clang_complete" "-I${DIR}\n")
    endforeach(DIR)
    add_custom_target(lint-clang
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintClangScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    add_custom_target(lint-clang-clean
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}" -DMODE="clean"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintClangScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif(CLANGPP_EXE)

find_program(CPPCHECK_EXE NAMES cppcheck
    PATHS ${CPPCHECK_EXE} ENV PATH $ENV{CLANGPP_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH)
if(CPPCHECK_EXE)
    message(STATUS "CppCheck linter found")
    add_custom_target(lint-cppcheck
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintCppCheckScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    add_custom_target(lint-cppcheck-clean
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}" -DMODE="clean"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintCppCheckScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif(CPPCHECK_EXE)

find_program(GCC_EXE NAMES gcc g++
    PATHS ${GCC_EXE} ENV PATH $ENV{GCC_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH)
if(GCC_EXE)
    execute_process(
        COMMAND "${GCC_EXE}" --version
        OUTPUT_VARIABLE GCC_VERSION
        ERROR_QUIET)
    if(GCC_VERSION MATCHES " LLVM ")
        message(STATUS "LLVM-gcc not suitable for linting")
        # Looking harder
        unset(GCC_EXE CACHE)
        find_program(GCC_EXE NAMES gcc-7 g++-7 gcc-8 g++-8 gcc-9 g++-9
            PATHS ${GCC_EXE} ENV PATH $ENV{GCC_PATH}
            CMAKE_FIND_ROOT_PATH_BOTH)
    endif()
endif()
if(GCC_EXE)
    message(STATUS "GCC linter found")
    add_custom_target(lint-gcc
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintGCCScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    add_custom_target(lint-gcc-clean
        COMMAND ${CMAKE_COMMAND} -DSRCDIRS="${SRCDIRS}" -DMODE="clean"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintGCCScript.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()


add_custom_target(lint DEPENDS lint-cppcheck lint-clang lint-gcc)
add_custom_target(lint-clean DEPENDS lint-cppcheck-clean lint-clang-clean lint-gcc-clean)

option(COVERAGE "Enable code coverage" OFF)
if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

    find_program(LCOV_EXE NAMES lcov
        PATHS ${LCOV_PATH} ENV PATH $ENV{LCOV_PATH}
        CMAKE_FIND_ROOT_PATH_BOTH)
    if(LCOV_EXE)
        message(STATUS "Lcov found")
        add_custom_target(lcov
            COMMAND ${CMAKE_COMMAND}
                -P "${CMAKE_CURRENT_LIST_DIR}/LintLCov.cmake"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
        add_custom_target(lcov-result
            COMMAND ${CMAKE_COMMAND} -DMODE="result"
                -P "${CMAKE_CURRENT_LIST_DIR}/LintLCov.cmake"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
            DEPENDS lcov)
    endif(LCOV_EXE)
    add_custom_target(cov-clean ALL
        COMMAND ${CMAKE_COMMAND} -DMODE="clean"
            -P "${CMAKE_CURRENT_LIST_DIR}/LintLCov.cmake"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif(COVERAGE)
get_directory_property(dirs INCLUDE_DIRECTORIES)
