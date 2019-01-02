cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB INC_FILES ${PROJECT_SOURCE_DIR}/*.h)
add_library (RisLib STATIC ${SRC_FILES})

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(RisLib Threads::Threads)

install(FILES ${INC_FILES} DESTINATION "/usr/local/include/RisLib")
install(FILES "/var/tmp/Dev_RisLibLx/build/RisLib/libRisLib.a" DESTINATION "/usr/local/lib")
#install(TARGETS RisLib DESTINATION "//usr/local/lib")
install(FILES RisLibConfig.cmake DESTINATION "/usr/local/lib/cmake/RisLib")


