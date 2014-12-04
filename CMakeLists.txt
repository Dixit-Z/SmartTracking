cmake_minimum_required(VERSION 2.8.4)
project(TrackingBall)
find_package (Threads)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
                 utils/udp.cpp
				 utils/tcp.cpp
                 drone/AtCmd.cpp)

add_executable(TrackingBall ${SOURCE_FILES})

target_link_libraries (TrackingBall ${CMAKE_THREAD_LIBS_INIT})
