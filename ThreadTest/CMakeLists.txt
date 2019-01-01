cmake_minimum_required (VERSION 3.8)

project (ThreadTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (ThreadTest ${SRC_FILES})

target_include_directories(ThreadTest PRIVATE RisLib)
target_include_directories(ThreadTest PRIVATE CommonLib)

target_link_libraries(ThreadTest RisLib)
target_link_libraries(ThreadTest CommonLib)
