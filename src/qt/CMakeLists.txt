
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(Depend_Qt)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${QT_INCLUDES})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${QT_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QT_COMPILE_FLAGS}")

add_library(qt-misc-tools SHARED
    InputEventRecorder.cpp InputEventRecorder.hpp
    InputEventSerializer.cpp InputEventSerializer.hpp
    InputEventSerializerQt5.cpp
    InputEventSerializerQt4.cpp
    TestRecorder.cpp TestRecorder.hpp
    SignalWaiter.cpp SignalWaiter.hpp
)
target_link_libraries(qt-misc-tools ${QT_DEPS})

add_subdirectory(tests)
