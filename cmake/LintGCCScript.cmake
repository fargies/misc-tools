# Arguments: SRCDIRS (list of directories to analyse)
# Argument: MODE (clean to remove files)
separate_arguments(SRCDIRS)
string(TOLOWER "${MODE}" MODE)

set(S "${CMAKE_CURRENT_LIST_DIR}/../")
set(B "${CMAKE_BINARY_DIR}")

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

foreach(F IN LISTS SRCDIRS)
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cpp" "${S}/${F}/*.hpp")
    list(APPEND GLOB_PATTERN "${S}/${F}/*.cc" "${S}/${F}/*.hh")
endforeach()
file(GLOB_RECURSE SRCS RELATIVE "${S}" ${GLOB_PATTERN})

function(gen SRCS)
    file(STRINGS "${S}/.clang_complete" OPTS)
    foreach(F IN LISTS SRCS)
        if("${S}/${F}" IS_NEWER_THAN "${B}/${F}.gcc")
            message(STATUS "Running GCC linter on \"${F}\"")
            get_filename_component(F_DIR "${B}/${F}" PATH)
            file(MAKE_DIRECTORY "${F_DIR}")
            execute_process(
                COMMAND "${GCC_EXE}" -c -Wall -pedantic -fsyntax-only ${OPTS} "${F}"
                    -pedantic -Wno-long-long -Wnon-virtual-dtor
                    -Woverloaded-virtual -Wunused-parameter -Wuninitialized
                    -o /dev/null
                WORKING_DIRECTORY "${S}"
                ERROR_FILE "${B}/${F}.gcc"
                ERROR_STRIP_TRAILING_WHITESPACE)
        endif()
        file(STRINGS "${B}/${F}.gcc" OUT)
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
        if(EXISTS "${B}/${F}.gcc")
            message(STATUS "Removing ${F}.gcc")
            file(REMOVE "${B}/${F}.gcc")
        endif()
    endforeach()
endfunction()

if(MODE STREQUAL "clean")
    clean("${SRCS}")
else()
    gen("${SRCS}")
endif()
