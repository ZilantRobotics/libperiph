list(APPEND LIBPERIPH_DRIVERS_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/flame.c
    ${CMAKE_CURRENT_LIST_DIR}/thunder.c
    ${CMAKE_CURRENT_LIST_DIR}/../../common/ring_buffer.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/..)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common)
