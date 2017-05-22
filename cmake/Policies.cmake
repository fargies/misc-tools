
macro(set_policy NAME STATE)
    if(POLICY ${NAME})
        cmake_policy(SET "${NAME}" "${STATE}")
    endif()
endmacro()
