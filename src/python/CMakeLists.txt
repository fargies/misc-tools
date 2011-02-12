
include(FindPythonLibs)

if(PYTHONLIBS_FOUND)

include_directories(${PYTHON_INCLUDE_DIR})

add_library(iter SHARED iter.cc)
set_target_properties(iter PROPERTIES PREFIX "")

set(PythonCpp_SRCS
        PyDict.cc PyDictValue.cc
        PyDictIterator.cc)
add_library(python-c++ SHARED ${PythonCpp_SRCS})

add_subdirectory(tests)

endif(PYTHONLIBS_FOUND)

