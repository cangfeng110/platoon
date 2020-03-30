cmake_minimum_required(VERSION 2.6)

project(platoon)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

add_compile_options(-std=c++11)

option(isWall "this is a -Wall switch" OFF)
option(isG "this is a -g switch" OFF)

if (isWall)
    add_definitions("-Wall")
endif (isWall)

if (isG)
    add_definitions("-g")
endif (isG)

include_directories("/include/" ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/customfunction)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/communication)
