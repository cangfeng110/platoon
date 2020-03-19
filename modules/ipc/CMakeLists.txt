CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
project(ipc)


#set include files: *.h, *.hpp, *.hh in this project
FILE(GLOB_RECURSE PROJECT_HEADERS  "include/*.h" "include/*.hpp" "message/protobuf/*.pb.h" "conf/*.pb.h")
#set source files:*.cpp, *.c, *.cc in this project
FILE(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp" "src/*.cc" "message/protobuf/*.pb.cc" "conf/*.pb.cc")
#set library files: *.lib in this project
FILE(GLOB_RECURSE PROJECT_LIBRARIES "../../bin/libcommon.a")


set(temp_new_headers)
function_auto_delete_headers("${PROJECT_HEADERS}" temp_new_headers)
set(PROJECT_HEADERS "${temp_new_headers}")


set(temp_new_srcs)
function_auto_delete_sources("${PROJECT_SOURCES}" temp_new_srcs)
set(PROJECT_SOURCES "${temp_new_srcs}")

#message(STATUS "${PROJECT_NAME}'s PROJECT_HEADERS: ${PROJECT_HEADERS}")
#message(STATUS "${PROJECT_NAME}'s PROJECT_SOURCES: ${PROJECT_SOURCES}")
#message(STATUS "${PROJECT_NAME}'s PROJECT_LIBRARIES: ${PROJECT_LIBRARIES}")
link_directories(/home/saic/Desktop/atd-master/bin)

ADD_LIBRARY (${PROJECT_NAME} STATIC ${PROJECT_SOURCES} ${PROJECT_HEADERS})
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${PROJECT_LIBRARIES} protobuf glog pthread gtest gtest_main lcm)
set(temp_link_libs ${PROJECT_LIBRARIES}  common protobuf-lite protobuf common lcm )
macro_auto_target_link(${PROJECT_NAME} "${temp_link_libs}")
add_dependencies(${PROJECT_NAME} common)
