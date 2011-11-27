
include(FindPythonLibs)

if(PYTHONLIBS_FOUND)

include_directories(${PYTHON_INCLUDE_DIR})

add_library(iter SHARED iter.cc)
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

