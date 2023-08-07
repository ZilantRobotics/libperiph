list(APPEND LIBPERIPH_DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/ublox.c
    ${CMAKE_CURRENT_LIST_DIR}/ublox_commands.c
    ${CMAKE_CURRENT_LIST_DIR}/ublox_emulation.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform_specific/hal_uart/CMakeLists.txt)
