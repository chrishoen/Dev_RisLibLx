cmake_minimum_required (VERSION 3.8)

project (CommonLib CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_library (CommonLib STATIC ${SRC_FILES})

target_include_directories(CommonLib PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(CommonLib PRIVATE RisLib)

target_link_libraries(CommonLib RisLib)
