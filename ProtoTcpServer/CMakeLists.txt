cmake_minimum_required (VERSION 3.8)

project (ProtoTcpServer CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (ProtoTcpServer ${SRC_FILES})

target_include_directories(ProtoTcpServer PRIVATE RisLib)
target_include_directories(ProtoTcpServer PRIVATE CommonLib)

target_link_libraries(ProtoTcpServer RisLib)
target_link_libraries(ProtoTcpServer CommonLib)
