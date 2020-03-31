cmake_minimum_required(VERSION 2.6)

project(platoon)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

add_compile_options(-std=c++11)

option(isWall "this is a -Wall switch" OFF)
option(isG "this is a -g switch" OFF)

if (isWall STREQUAL "ON")
    message(STATUS "isWall is ON")
    add_definitions("-Wall")
else (isWall STREQUAL "OFF")
    message(STATUS "isWall is OFF")
endif ()

if (isG STREQUAL "ON")
    message(STATUS "isG is ON")
    add_definitions("-g")
else (isG STREQUAL "OFF")
    message(STATUS "isG is OFF")
endif ()

include_directories("/include/" ${CMAKE_CURRENT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/customfunction)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include/base)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/communication)
