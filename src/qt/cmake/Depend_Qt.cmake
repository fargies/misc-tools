
find_package(Qt5Core QUIET)

if(Qt5Core_FOUND)
    message(STATUS "Using Qt ${Qt5Core_VERSION_STRING}")
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)

    set(QT_LIBRARIES
        ${Qt5Core_LIBRARIES}
        ${Qt5Gui_LIBRARIES}
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Network_LIBRARIES} CACHE STRING "Libraries for Qt dependencies"
       )
    set(QT_INCLUDES
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS} CACHE STRING "Includes for Qt dependencies"
       )

    set(QT_DEPS Qt5::Core Qt5::Widgets)

    # Use the compile definitions defined in the Qt 5 Widgets module
    set(QT_DEFINITIONS ${Qt5Widgets_DEFINITIONS} CACHE STRING "Compile definitions for Qt")

    # Add compiler flags for building executables (-fPIE)
    set(QT_COMPILE_FLAGS ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} CACHE STRING "Compile flags for Qt")

    macro(qt_add_resources)
        qt5_add_resources(${ARGN})
    endmacro()

    macro(qt_wrap_ui)
        qt5_wrap_ui(${ARGN})
    endmacro()
else()
    find_package(Qt4 4.7 REQUIRED QtGui QtNetwork)

    if(NOT Qt4_FOUND)
        message(FATAL_ERROR "Can't find Qt4 and Qt5 modules")
    endif()

    message(STATUS "Using Qt ${QTVERSION}")

    # QT_INCLUDES already set
    set(QT_INCLUDES "${QT_INCLUDES}" CACHE STRING "Includes for Qt dependencies")
    set(QT_DEPS Qt4::QtNetwork Qt4::QtCore Qt4::QtGui)

    set(QT_DEFINITIONS)
    set(QT_COMPILE_FLAGS)

    macro(qt_add_resources)
        qt4_add_resources(${ARGN})
    endmacro()

    macro(qt_wrap_ui)
        qt4_wrap_ui(${ARGN})
    endmacro()
endif()

set(CMAKE_AUTOMOC True)
