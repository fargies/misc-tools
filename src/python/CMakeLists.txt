
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIR})

set(iter_SRCS
    iter.cc)
add_library(iter SHARED ${iter_SRCS})
set_target_properties(iter PROPERTIES PREFIX "")

