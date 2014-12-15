cmake_minimum_required(VERSION 2.8.4)
project(TrackingBall)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package (Threads)
find_package (Qt4 REQUIRED)
find_package (OpenCV REQUIRED)
find_package (FFMPEG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
                 utils/udp.cpp
				 utils/tcp.cpp
                 drone/AtCmd.cpp
                 video.cpp)
                 
set(TrackingBall_UIa
            )

#QT4_WRAP_CPP(helloworld_HEADERS_MOC ${helloworld_HEADERS})
QT4_WRAP_UI(TrackingBall_UIb ${TrackingBall_UIa})
#QT4_ADD_RESOURCES(helloworld_RESOURCES_RCC ${helloworld_RESOURCES})
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(TrackingBall ${TrackingBall_UIb} ${SOURCE_FILES})

target_link_libraries (TrackingBall ${QT_LIBRARIES})
target_link_libraries (TrackingBall ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (TrackingBall ${OpenCV_LIBS})