cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB INC_FILES ${PROJECT_SOURCE_DIR}/*.h)
add_library (RisLib STATIC ${SRC_FILES})

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(RisLib Threads::Threads)

install(FILES ${INC_FILES} DESTINATION "/var/tmp/install/include/RisLib")
install(FILES "/var/tmp/Dev_RisLibLx/build/RisLib/libRisLib.a" DESTINATION "/var/tmp/install/lib")
#install(TARGETS RisLib DESTINATION "/var/tmp/install/lib")
install(FILES RisLibConfig.cmake DESTINATION "/var/tmp/install/lib/cmake/RisLib")


