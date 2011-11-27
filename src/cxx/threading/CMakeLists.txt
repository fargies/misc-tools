
set(threading_SRCS mutex.cc)
set(threading_HDRS mutex.hh)

add_library(threading ${threading_SRCS} ${threading_HDRS})

