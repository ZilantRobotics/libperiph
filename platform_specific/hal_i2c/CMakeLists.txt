if(NOT LIBPERIPH_PLATFORM)
  message(SEND_ERROR "LIBPERIPH_PLATFORM is not specified")
endif()

list(APPEND PERIPHERY_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/${LIBPERIPH_PLATFORM}/hal_i2c.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common)