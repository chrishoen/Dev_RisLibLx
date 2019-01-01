cmake_minimum_required (VERSION 3.8)

project (Test CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (Test ${SRC_FILES})

target_include_directories(Test PRIVATE RisLib)
target_include_directories(Test PRIVATE CommonLib)

target_link_libraries(Test RisLib)
target_link_libraries(Test CommonLib)
