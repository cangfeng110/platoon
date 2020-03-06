aux_source_directory(. SRC)

add_executable(platoon ${SRC})
<<<<<<< HEAD
# need pthread ,otherwise can't build in pegasus
target_link_libraries(platoon lcm config++.a pthread platoon-common platoon-base)
=======

target_link_libraries(platoon lcm pthread config++.a platoon-common platoon-base)
>>>>>>> refs/remotes/origin/dev

