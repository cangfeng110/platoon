aux_source_directory(. SRC)

link_libraries(/home/igs/zsworkspace/platoon/build/bin/libcommon.a)

link_libraries(/home/igs/zsworkspace/platoon/build/bin/libipc.a)

add_executable(platoon ${SRC})

#target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base platoon-proto)

target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base)


