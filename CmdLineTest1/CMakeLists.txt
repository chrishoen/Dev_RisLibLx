cmake_minimum_required (VERSION 3.8)

project (CmdLineTest1 CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (CmdLineTest1 ${SRC_FILES})

target_include_directories(CmdLineTest1 PRIVATE RisLib)
target_include_directories(CmdLineTest1 PRIVATE CommonLib)

target_link_libraries(CmdLineTest1 RisLib)
target_link_libraries(CmdLineTest1 CommonLib)
