/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"
#include <string.h>
#include "main.h"
#include "libperiph_common.h"

extern UART_HandleTypeDef huart1;
#define UART_1_PTR &huart1

#if defined(SECOND_UART)
    extern UART_HandleTypeDef huart2;
    #define UART_2_PTR &huart2
#else
    #define UART_2_PTR NULL
#endif

typedef enum {
    NO_FLAGS = 0,
    HALF_RECEIVED_FLAG,
    FULL_RECEIVED_FLAG,
    BOTH_FLAGS,
} UartRxStatus_t;

typedef struct {
    UART_HandleTypeDef* huart_ptr;
    uint8_t* buffer;
    uint16_t full_size;
    UartRxStatus_t status;
    void (*rx_callback)();
    uint32_t total_rx_counter;
} UartRxConfig_t;

static UartRxConfig_t uart_rx[2];


int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size) {
    if (instance == UART_FIRST) {
        uart_rx[instance].huart_ptr = UART_1_PTR;
    } else if (instance == UART_SECOND) {
        uart_rx[instance].huart_ptr = UART_2_PTR;
    } else {
        return STATUS_ERROR;
    }

    uart_rx[instance].buffer = buffer;
    uart_rx[instance].full_size = size;
    uart_rx[instance].total_rx_counter = 0;
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(uart_rx[instance].huart_ptr, buffer, size);
    return (status == HAL_OK) ? STATUS_OK : STATUS_ERROR;
}

void uartRegisterRxCallback(UartInstance_t instance, void (*rx_callback)()) {
    if (instance > UART_SECOND) {
        return;
    }

    uart_rx[instance].rx_callback = rx_callback;
}


size_t uartGetLastReceivedIndex(UartInstance_t instance) {
    if (instance > UART_SECOND) {
        return 0;
    }

    const UartRxConfig_t* config = &uart_rx[instance];
    uint16_t dma_counter = __HAL_DMA_GET_COUNTER(config->huart_ptr->hdmarx);
    if (config->full_size == dma_counter) {
        return config->full_size - 1;
    }
    return config->full_size - dma_counter - 1;
}

uint8_t* uartRxDmaPop() {
    UartRxConfig_t* config = &uart_rx[UART_FIRST];

    if (config->status == NO_FLAGS) {
        return NULL;
    }

    uint8_t* buffer_ptr = config->buffer;
    if (config->status == FULL_RECEIVED_FLAG || config->status == BOTH_FLAGS) {
        buffer_ptr += config->full_size / 2;
    }
    config->status = NO_FLAGS;
    return buffer_ptr;
}

uint32_t uartRxGetStats(UartInstance_t instance) {
    return (instance < UART_AMOUNT) ? uart_rx[UART_FIRST].total_rx_counter : 0;
}

void uartRxResetStats() {
    uart_rx[UART_FIRST].total_rx_counter = 0;
    uart_rx[UART_SECOND].total_rx_counter = 0;
}


void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == uart_rx[UART_FIRST].huart_ptr) {
        uart_rx[UART_FIRST].status |= HALF_RECEIVED_FLAG;
        if (uart_rx[UART_FIRST].rx_callback != NULL) {
            (*uart_rx[UART_FIRST].rx_callback)();
        }
        uart_rx[UART_FIRST].total_rx_counter += uart_rx[UART_FIRST].full_size;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == uart_rx[UART_FIRST].huart_ptr) {
        uart_rx[UART_FIRST].status |= FULL_RECEIVED_FLAG;
        if (uart_rx[UART_FIRST].rx_callback != NULL) {
            (*uart_rx[UART_FIRST].rx_callback)();
        }
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    const UartRxConfig_t* config;
    if (huart == uart_rx[UART_FIRST].huart_ptr) {
        config = &uart_rx[UART_FIRST];
    } else if (huart == uart_rx[UART_SECOND].huart_ptr) {
        config = &uart_rx[UART_SECOND];
    } else {
        return;
    }

    HAL_UART_Receive_DMA(config->huart_ptr, config->buffer, config->full_size);
}
