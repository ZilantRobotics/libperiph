list(APPEND LIBPERIPH_DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/garmin_lite.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CMAKE_CURRENT_LIST_DIR}/../../../platform_specific/hal_i2c/CMakeLists.txt)