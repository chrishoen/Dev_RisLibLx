cmake_minimum_required (VERSION 3.8)

project (TimerTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (TimerTest ${SRC_FILES})

target_include_directories(TimerTest PRIVATE RisLib)
target_include_directories(TimerTest PRIVATE CommonLib)

target_link_libraries(TimerTest RisLib)
target_link_libraries(TimerTest CommonLib)
