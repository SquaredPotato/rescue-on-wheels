cmake_minimum_required(VERSION 3.0)
project(column)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(~/include)

file(GLOB SOURCE_FILES
        "src/*.h"
        "src/*.cpp"
        )

add_executable(column ${SOURCE_FILES})

target_link_libraries(column m wiringPi)
