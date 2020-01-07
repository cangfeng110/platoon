AUX_SOURCE_DIRECTORY(. SRC)
AUX_SOURCE_DIRECTORY(./C-ITS SRC_CITS)

ADD_EXECUTABLE(tjP-communication ${SRC} ${SRC_CITS})
TARGET_LINK_LIBRARIES(tjP-communication lcm pthread config++ tjP utils c-its)
