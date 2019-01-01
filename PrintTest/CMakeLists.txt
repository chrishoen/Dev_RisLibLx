cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (PrintTest ${SRC_FILES})

target_include_directories(PrintTest PRIVATE RisLib)
target_include_directories(PrintTest PRIVATE CommonLib)

target_link_libraries(PrintTest RisLib)
target_link_libraries(PrintTest CommonLib)
