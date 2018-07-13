# Argument: SRCDIRS (list of directories to analyse)
# Argument: MODE (clean to remove files)
separate_arguments(SRCDIRS)
string(TOLOWER "${MODE}" MODE)

set(S "${CMAKE_CURRENT_LIST_DIR}/../")
set(B "${CMAKE_BINARY_DIR}")

find_program(CPPCHECK_EXE NAMES cppcheck
    PATHS ${CPPCHECK_EXE} ENV PATH $ENV{CPPCHECK_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH)

foreach(F IN LISTS SRCDIRS)
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cpp" "${S}/${F}/*.hpp")
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cc" "${S}/${F}/*.hh")
endforeach()
file(GLOB_RECURSE SRCS RELATIVE "${S}" ${GLOB_PATTERN})

function(gen SRCS)
    if(EXISTS "${S}/.cppcheck.supp")
        message(STATUS "Suppression file found")
        list(APPEND OPTS "--suppressions-list=.cppcheck.supp")
    endif()

    foreach(F IN LISTS SRCS)
        if("${S}/${F}" IS_NEWER_THAN "${B}/${F}.cppcheck")
            message(STATUS "Running CppCheck linter on \"${F}\"")
            get_filename_component(F_DIR "${B}/${F}" PATH)
            file(MAKE_DIRECTORY "${F_DIR}")
            execute_process(
                COMMAND "${CPPCHECK_EXE}" --language=c++ --force --quiet
                    "--template={file}:{line}:{column}: {severity}({id}): {message}"
                    --enable=information,performance,portability,style,warning
                    ${OPTS} --suppress=unmatchedSuppression
                    --suppress=missingInclude --suppress=ConfigurationNotChecked "${F}"
                WORKING_DIRECTORY "${S}"
                ERROR_FILE "${B}/${F}.cppcheck"
                ERROR_STRIP_TRAILING_WHITESPACE)
        endif()
        file(STRINGS "${B}/${F}.cppcheck" OUT)
        foreach(E IN LISTS OUT)
            string(FIND "${E}" "${F}" POS)
            if("${POS}" EQUAL 0)
                message("${E}")
            endif()
        endforeach()
    endforeach()
endfunction()

function(clean SRCS)
    foreach(F IN LISTS SRCS)
        if(EXISTS "${B}/${F}.cppcheck")
            message(STATUS "Removing ${F}.cppcheck")
            file(REMOVE "${B}/${F}.cppcheck")
        endif()
    endforeach()
endfunction()

if(MODE STREQUAL "clean")
    clean("${SRCS}")
else()
    gen("${SRCS}")
endif()
