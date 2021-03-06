
link_directories(
    ${CPPUNIT_LIBRARY_DIRS})
include_directories(
    ${CPPUNIT_INCLUDE_DIRS}
    ..
    ../sem)

set(DEP_LIBS ${CPPUNIT_LIBRARIES} testHelper cxx-misc-tools)

if(NOT APPLE)
add_executable(rwLock_bench rwLock_bench.cc)
target_link_libraries(rwLock_bench ${DEP_LIBS})
endif(NOT APPLE)

add_executable(fileLock_bench fileLock_bench.cc)
target_link_libraries(fileLock_bench ${DEP_LIBS})

if (DBUS_CPP_FOUND)

find_program(DBUS_XML2CPP dbusxx-xml2cpp)
if (NOT DBUS_XML2CPP)
  message(SEND_ERROR "dbusxx-xml2cpp not found")
endif (NOT DBUS_XML2CPP)

#add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/variant_dbus_proxy.hh
#                          ${CMAKE_CURRENT_BINARY_DIR}/variant_dbus_adaptor.hh
#                   COMMAND ${DBUS_XML2CPP} ${CMAKE_CURRENT_SOURCE_DIR}/variant_dbus_introspect.xml
#                           --proxy=${CMAKE_CURRENT_BINARY_DIR}/variant_dbus_proxy.hh
#                           --adaptor=${CMAKE_CURRENT_BINARY_DIR}/variant_dbus_adaptor.hh
#                   DEPENDS variant_dbus_introspect.xml)

link_directories(${CMAKE_CURRENT_BINARY_DIR}/../variant)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ../variant ${DBUS_CPP_INCLUDE_DIRS})
add_executable(variant_dbus_bench
               variant_dbus_proxy.hh
               variant_dbus_adaptor.hh
               variant_dbus_bench.cc)
target_link_libraries(variant_dbus_bench ${DEP_LIBS})

endif (DBUS_CPP_FOUND)
