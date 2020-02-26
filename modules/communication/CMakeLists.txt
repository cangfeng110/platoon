aux_source_directory(. SRC)

add_executable(platoon-communication ${SRC})

target_link_libraries(platoon-communication lcm pthread config++.a platoon-common platoon-base)



