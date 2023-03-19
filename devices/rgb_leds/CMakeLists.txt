list(APPEND DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/ws2812.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform_specific/hal_timers/CMakeLists.txt)