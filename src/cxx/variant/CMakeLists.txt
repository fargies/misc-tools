
include_directories(${CMAKE_BINARY_DIR} ${DBUS_CPP_INCLUDE_DIRS})
add_library(Variant SHARED Variant.cc Variant.hh)
target_link_libraries(Variant ${DBUS_CPP_LIBRARIES})
