# Argument: MODE (clean to remove gcda files)
string(TOLOWER "${MODE}" MODE)

get_filename_component(S "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)
get_filename_component(B "${CMAKE_BINARY_DIR}" ABSOLUTE)

function(run cmd)
    execute_process(COMMAND ${cmd} ${ARGN}
        WORKING_DIRECTORY "${B}"
        ERROR_VARIABLE OUT
        OUTPUT_VARIABLE OUT)
    message("${OUT}")
endfunction()

if(MODE STREQUAL "clean")
    file(GLOB_RECURSE GCDA RELATIVE "${B}" "${B}/*.gcda")
    foreach(F IN LISTS GCDA)
        message(STATUS "Removing ${F}")
        file(REMOVE "${B}/${F}")
    endforeach()
elseif(MODE STREQUAL "result")
    find_program(LCOV_EXE NAMES lcov
        PATHS ${LCOV_PATH} ENV PATH $ENV{LCOV_PATH}
        CMAKE_FIND_ROOT_PATH_BOTH)

    message(STATUS "Displaying result")
    run(lcov -l ../coverage/lcov.info)
else()
    find_program(LCOV_EXE NAMES lcov
        PATHS ${LCOV_PATH} ENV PATH $ENV{LCOV_PATH}
        CMAKE_FIND_ROOT_PATH_BOTH)

    file(MAKE_DIRECTORY "${S}/coverage")

    macro(moveInfo)
        if(NOT EXISTS "${COVDIR}/temp.info")
            message(FATAL_ERROR "Command failed to generate info file")
        endif()
        file(RENAME "${COVDIR}/temp.info" "${COVDIR}/lcov.info")
    endmacro()

    file(GLOB_RECURSE GCDA RELATIVE "${B}" "${B}/*.gcda")
    if(NOT GCDA)
        message(FATAL_ERROR "No GCDA files found, please generate some info")
    endif()

    set(COVDIR "${S}/coverage")
    message(STATUS "Generating lcov info")
    run(lcov --quiet -c -d . -o "${COVDIR}/temp.info")
    moveInfo()

    message(STATUS "Removing external coverage info")
    run(lcov --quiet -e "${COVDIR}/lcov.info" -o "${COVDIR}/temp.info" "*${S}/src/*")
    moveInfo()

    message(STATUS "Removing build-directory coverage info")
    run(lcov --quiet -r "${COVDIR}/lcov.info" -o "${COVDIR}/temp.info" "*${B}*")
    moveInfo()

    message(STATUS "Removing tests-directory coverage info")
    run(lcov --quiet -r "${COVDIR}/lcov.info" -o "${COVDIR}/temp.info" "*/tests/*")
    moveInfo()
endif()
