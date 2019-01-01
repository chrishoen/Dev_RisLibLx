cmake_minimum_required (VERSION 3.8)

project (Parms CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (Parms ${SRC_FILES})

target_include_directories(Parms PRIVATE RisLib)
target_include_directories(Parms PRIVATE CommonLib)

target_link_libraries(Parms RisLib)
target_link_libraries(Parms CommonLib)
