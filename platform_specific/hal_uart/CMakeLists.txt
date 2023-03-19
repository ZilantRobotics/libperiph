if(NOT LIBPERIPH_PLATFORM)
  message(SEND_ERROR "LIBPERIPH_PLATFORM is not specified")
endif()

FILE(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/${LIBPERIPH_PLATFORM}/*.c)
list(APPEND PERIPHERY_SOURCE ${sources})
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common)