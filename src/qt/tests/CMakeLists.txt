
if(TESTS)

link_directories(
    ${CMAKE_BINARY_DIR}/src/qt)

include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/qt
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/src/qt)

add_definitions(${QT_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QT_COMPILE_FLAGS}")

# Name collides with other testHelper
add_library(qtTestHelper SHARED
    test_helpers.cpp test_helpers.hpp
    test_helpers_stddump.cpp test_helpers_stddump.hpp
    QtRunner.cpp QtRunner.hpp)
target_link_libraries(qtTestHelper ${CPPUNIT_LIBRARIES} ${QT_DEPS})

set(test_SRCS
    test_main.cpp
    tst_InputEventSerializer.cpp
    tst_TestRecorder.cpp)

add_executable(test_qt ${test_SRCS})
target_link_libraries(test_qt qtTestHelper qt-misc-tools ${QT_DEPS})

add_test(test_qt ${CMAKE_CURRENT_BINARY_DIR}/test_qt)

install(TARGETS test_qt
        RUNTIME DESTINATION tests/${PROJECT_NAME} COMPONENT tests)

endif(TESTS)
