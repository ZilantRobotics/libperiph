list(APPEND LIBPERIPH_DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/hmc5883l.c
    ${CMAKE_CURRENT_LIST_DIR}/rm3100.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform_specific/hal_i2c/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../platform_specific/hal_spi/CMakeLists.txt)