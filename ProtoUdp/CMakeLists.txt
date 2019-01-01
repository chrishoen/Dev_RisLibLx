cmake_minimum_required (VERSION 3.8)

project (ProtoUdp CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (ProtoUdp ${SRC_FILES})

target_include_directories(ProtoUdp PRIVATE RisLib)
target_include_directories(ProtoUdp PRIVATE CommonLib)

target_link_libraries(ProtoUdp RisLib)
target_link_libraries(ProtoUdp CommonLib)
