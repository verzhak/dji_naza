
cmake_minimum_required(VERSION 3.0)

project(dji_naza_reader)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_AMV QUIET amv)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CXX_FLAGS "${CXX_FLAGS} ${PC_AMV_CFLAGS} -Wall -pipe -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}" CACHE string "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(sources main.cpp)

add_executable(dji_naza_reader ${sources})
target_link_libraries(dji_naza_reader ${OpenCV_LIBS} ${PC_AMV_LIBRARIES})

