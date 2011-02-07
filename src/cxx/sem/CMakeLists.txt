
add_library(rwLock rwLock.cc rwLock.hh)

add_library(sem sem.cc sem.hh)
target_link_libraries(sem rt)

