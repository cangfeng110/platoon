cmake_minimum_required(VERSION 2.6)

project(platoon)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/communication)