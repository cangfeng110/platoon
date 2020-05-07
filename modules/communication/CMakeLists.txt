aux_source_directory(. SRC)

option(pegasus-build "this is a pegasus build switch" OFF)

if (pegasus-build STREQUAL "ON")
    message(STATUS "the pegasus build is ON")
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/pglib/libcommon.a)
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/pglib/libipc.a)
    add_executable(app_platoon ${SRC})
    target_link_libraries(app_platoon lcm pthread config++.a protobuf platoon-common platoon-base)
    #link_libraries(/home/igs/zsworkspace/platoon-sil/platoon/include/lib/pglib/libcommon.a)
    #link_libraries(/home/igs/zsworkspace/platoon-sil/platoon/include/lib/pglib/libipc.a)
else (pegasus-build STREQUAL "OFF")
    message(STATUS "the pegasus build is OFF")
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/libcommon.a)
    link_libraries(${CMAKE_CURRENT_SOURCE_DIR}/../../include/lib/libipc.a)
    add_executable(platoon ${SRC})
    target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base)
    #link_libraries(/home/igs/zsworkspace/platoon-sil/platoon/include/lib/libcommon.a)
    #link_libraries(/home/igs/zsworkspace/platoon-sil/platoon/include/lib/libipc.a)
endif ()

#add_executable(app_platoon ${SRC})

#target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base platoon-proto)

#target_link_libraries(app_platoon lcm pthread config++.a protobuf platoon-common platoon-base)


