cmake_minimum_required(VERSION 2.8.4)
project(TrackingBall)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package (Threads)
find_package (Qt4 REQUIRED)
find_package (OpenCV REQUIRED)
find_package (FFMPEG REQUIRED)

include_directories( ${FFMPEG_INCLUDE_DIRS})
include_directories( ${CMAKE_BINARY_DIR} )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(LIBS ${LIBS} ${FFMPEG_LIBRARIES})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp
                 utils/udp.cpp
		 utils/tcp.cpp
                 drone/AtCmd.cpp
                 video.cpp)

add_executable(TrackingBall ${SOURCE_FILES})
                 
target_link_libraries(TrackingBall ${LIBS})
target_link_libraries (TrackingBall ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (TrackingBall ${OpenCV_LIBS})
target_link_libraries (TrackingBall ${FFMPEG_LIBS})

