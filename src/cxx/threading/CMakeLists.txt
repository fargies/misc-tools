
set(threading_SRCS mutex.cc locker.cc cond.cc)
set(threading_HDRS mutex.hh locker.hh cond.hh)

add_library(threading ${threading_SRCS} ${threading_HDRS})

