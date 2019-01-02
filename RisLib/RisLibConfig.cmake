
add_library(RisLib STATIC IMPORTED)
set_target_properties(RisLib PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libRisLib.a")
target_include_directories(RisLib INTERFACE "/usr/local/include/RisLib")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(RisLib INTERFACE Threads::Threads)
