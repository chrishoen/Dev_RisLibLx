cmake_minimum_required (VERSION 3.8)

project (AlphaTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (AlphaTest ${SRC_FILES})

target_include_directories(AlphaTest PRIVATE RisLib)
target_include_directories(AlphaTest PRIVATE CommonLib)

target_link_libraries(AlphaTest RisLib)
target_link_libraries(AlphaTest CommonLib)
