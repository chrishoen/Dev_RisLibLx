cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library (RisLib STATIC ${SRC_FILES})

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(RisLib Threads::Threads)

