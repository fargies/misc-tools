
set(Python_ADDITIONAL_VERSIONS 2.7)
include(FindPythonLibs)

if(PYTHONLIBS_FOUND)

# For some reason this is not exported
set(PYTHONLIBS_FOUND "${PYTHONLIBS_FOUND}" CACHE STRING "" FORCE)

include_directories(${PYTHON_INCLUDE_DIR})

add_library(iter SHARED iter.cc)
target_link_libraries(iter ${PYTHON_LIBRARIES})
set_target_properties(iter PROPERTIES PREFIX "")

set(python_SRCS
        PyValue.cc
        PyDict.cc PyDictValue.cc PyDictIterator.cc
        PySeq.cc PySeqValue.cc PySeqIterator.cc
        PyList.cc
        )
add_library(python++ SHARED ${python_SRCS})
target_link_libraries(python++ ${PYTHON_LIBRARIES})

add_subdirectory(tests)

endif(PYTHONLIBS_FOUND)
