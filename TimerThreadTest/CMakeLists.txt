cmake_minimum_required (VERSION 3.8)

project (TimerThreadTest CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable (TimerThreadTest ${SRC_FILES})

target_include_directories(TimerThreadTest PRIVATE RisLib)
target_include_directories(TimerThreadTest PRIVATE CommonLib)

target_link_libraries(TimerThreadTest RisLib)
target_link_libraries(TimerThreadTest CommonLib)
