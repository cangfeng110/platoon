aux_source_directory(. SRC)

add_executable(platoon ${SRC})

#target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base platoon-proto)

target_link_libraries(platoon lcm pthread config++.a protobuf platoon-common platoon-base)


