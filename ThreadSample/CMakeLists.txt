cmake_minimum_required (VERSION 3.8)

project (ThreadSample CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (ThreadSample ${SRC_FILES})

target_include_directories(ThreadSample PRIVATE RisLib)
target_include_directories(ThreadSample PRIVATE CommonLib)

target_link_libraries(ThreadSample RisLib)
target_link_libraries(ThreadSample CommonLib)
