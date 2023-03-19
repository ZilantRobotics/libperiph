list(APPEND PERIPHERY_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/${LIBPERIPH_PLATFORM}/hal_uart_common.c
    ${CMAKE_CURRENT_LIST_DIR}/${LIBPERIPH_PLATFORM}/hal_uart_rx.c
    ${CMAKE_CURRENT_LIST_DIR}/${LIBPERIPH_PLATFORM}/hal_uart_tx.c
)
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common)