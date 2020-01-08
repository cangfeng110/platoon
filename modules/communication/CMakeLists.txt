aux_source_directory(. SRC)
add_executable(platoon-communication S{SRC})
target_link_libraries(platoon-communication lcm pthred config++ platoon-common platoon-base)
# AUX_SOURCE_DIRECTORY(. SRC)
# AUX_SOURCE_DIRECTORY(./C-ITS SRC_CITS)

# ADD_EXECUTABLE(tjP-communication ${SRC} ${SRC_CITS})
# TARGET_LINK_LIBRARIES(tjP-communication lcm pthread config++ tjP utils c-its)
