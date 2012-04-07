
set(threading_SRCS mutex.cc locker.cc cond.cc)
set(threading_HDRS mutex.hh locker.hh cond.hh)

add_library(threading SHARED ${threading_SRCS} ${threading_HDRS})

add_library(threading-tests SHARED mutex_test.cc)
target_link_libraries(threading-tests threading)

