aux_source_directory(. SRC)

add_executable(app_platoon ${SRC})

target_link_libraries(app_platoon lcm pthread config++.a platoon-common platoon-base)


