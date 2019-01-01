cmake_minimum_required (VERSION 3.8)

project (CancelTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (CancelTest ${SRC_FILES})

target_include_directories(CancelTest PRIVATE RisLib)
target_include_directories(CancelTest PRIVATE CommonLib)

target_link_libraries(CancelTest RisLib)
target_link_libraries(CancelTest CommonLib)
