cmake_minimum_required (VERSION 3.8)

project (ThreadServices CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (ThreadServices ${SRC_FILES})

target_include_directories(ThreadServices PRIVATE RisLib)
target_include_directories(ThreadServices PRIVATE CommonLib)

target_link_libraries(ThreadServices RisLib)
target_link_libraries(ThreadServices CommonLib)
