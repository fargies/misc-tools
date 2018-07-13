# Arguments: SRCDIRS (list of directories to analyse)
# Argument: MODE (clean to remove files)
separate_arguments(SRCDIRS)
string(TOLOWER "${MODE}" MODE)

set(S "${CMAKE_CURRENT_LIST_DIR}/../")
set(B "${CMAKE_BINARY_DIR}")

find_program(CLANGPP_EXE NAMES clang++
    PATHS ${CLANGPP_EXE} ENV PATH $ENV{CLANGPP_PATH}
    CMAKE_FIND_ROOT_PATH_BOTH)

foreach(F IN LISTS SRCDIRS)
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cpp" "${S}/${F}/*.hpp")
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cc" "${S}/${F}/*.hh")
endforeach()
file(GLOB_RECURSE SRCS RELATIVE "${S}" ${GLOB_PATTERN})

function(gen SRCS)
    file(STRINGS "${S}/.clang_complete" OPTS)
    foreach(F IN LISTS SRCS)
        if("${S}/${F}" IS_NEWER_THAN "${B}/${F}.clang")
            message(STATUS "Running Clang linter on \"${F}\"")
            get_filename_component(F_DIR "${B}/${F}" PATH)
            file(MAKE_DIRECTORY "${F_DIR}")
            execute_process(
                COMMAND "${CLANGPP_EXE}" -pipe --analyze -Wall ${OPTS} "${F}" -o /dev/null
                WORKING_DIRECTORY "${S}"
                ERROR_FILE "${B}/${F}.clang"
                ERROR_STRIP_TRAILING_WHITESPACE)
        endif()
        file(STRINGS "${B}/${F}.clang" OUT)
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
        if(EXISTS "${B}/${F}.clang")
            message(STATUS "Removing ${F}.clang")
            file(REMOVE "${B}/${F}.clang")
        endif()
    endforeach()
endfunction()

if(MODE STREQUAL "clean")
    clean("${SRCS}")
else()
    gen("${SRCS}")
endif()
