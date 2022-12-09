/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"
#include <string.h>
#include "main.h"
#include "libperiph_common.h"


#define MAX_UART_TX_BUF_SIZE    100

typedef enum {
    NO_FLAGS = 0,
    HALF_RECEIVED_FLAG,
    FULL_RECEIVED_FLAG,
    BOTH_FLAGS,
} UartRxStatus_t;

#ifdef HAL_UART_MODULE_ENABLED

typedef struct {
    UART_HandleTypeDef* huart_ptr;
    uint8_t* buffer;
    uint16_t size;
    UartRxStatus_t status;
    void (*rx_callback)();
} UartRxConfig_t;

typedef struct {
    uint8_t buffer[MAX_UART_TX_BUF_SIZE];
    bool full_transmitted;
    void (*tx_callback)();
} UartTxConfig_t;


extern UART_HandleTypeDef huart1;
#define UART_1_PTR &huart1
static UartRxConfig_t uart_rx[2];
static UartTxConfig_t uart_1_tx = {{}, true};

#if defined(SECOND_UART)
    extern UART_HandleTypeDef huart2;
    #define UART_2_PTR &huart2
#else
    #define UART_2_PTR NULL
#endif


int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size) {
    if (instance == UART_FIRST) {
        uart_rx[instance].huart_ptr = UART_1_PTR;
    } else if (instance == UART_SECOND) {
        uart_rx[instance].huart_ptr = UART_2_PTR;
    } else {
        return STATUS_ERROR;
    }

    uart_rx[instance].buffer = buffer;
    uart_rx[instance].size = size;
    HAL_StatusTypeDef status = HAL_UART_Receive_DMA(uart_rx[instance].huart_ptr, buffer, size);
    return (status == HAL_OK) ? STATUS_OK : STATUS_ERROR;
}

void uartRegisterRxCallback(UartInstance_t instance, void (*rx_callback)()) {
    if (instance > UART_SECOND) {
        return;
    }

    uart_rx[instance].rx_callback = rx_callback;
}

void uartRegisterTxCallback(UartInstance_t instance, void (*tx_callback)()) {
    if (instance == UART_FIRST) {
        uart_1_tx.tx_callback = tx_callback;
    }
}

size_t uartGetLastReceivedIndex(UartInstance_t instance) {
    if (instance > UART_SECOND) {
        return 0;
    }

    const UartRxConfig_t* config = &uart_rx[instance];
    uint16_t dma_counter = __HAL_DMA_GET_COUNTER(config->huart_ptr->hdmarx);
    if (config->size == dma_counter) {
        return config->size - 1;
    }
    return config->size - dma_counter - 1;
}

uint8_t* uartRxDmaPop() {
    UartRxConfig_t* config = &uart_rx[UART_FIRST];

    if (config->status == NO_FLAGS) {
        return NULL;
    }

    uint8_t* buffer_ptr = config->buffer;
    if (config->status == FULL_RECEIVED_FLAG || config->status == BOTH_FLAGS) {
        buffer_ptr += config->size / 2;
    }
    config->status = NO_FLAGS;
    return buffer_ptr;
}


int8_t uartTransmit(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE) {
        return STATUS_ERROR;
    }
    memcpy(uart_1_tx.buffer, buffer, size);
    return HAL_UART_Transmit(uart_rx[UART_FIRST].huart_ptr, uart_1_tx.buffer, size, 500) == HAL_OK ? 0 : -1;
}

int8_t uartTransmitDma(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE || !uart_1_tx.full_transmitted) {
        return STATUS_ERROR;
    }
    memcpy(uart_1_tx.buffer, buffer, size);
    uart_1_tx.full_transmitted = false;
    return HAL_UART_Transmit_DMA(uart_rx[UART_FIRST].huart_ptr, uart_1_tx.buffer, size) == HAL_OK ? 0 : -1;
}

bool uartIsTxReady() {
    HAL_UART_StateTypeDef status = HAL_UART_GetState(uart_rx[UART_FIRST].huart_ptr);
    return (status == HAL_UART_STATE_READY || status == HAL_UART_STATE_BUSY_RX);
}


void uartEnableTx(bool enable) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9;

    if (enable) {
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    } else {
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void UartChangeBaudrate(uint16_t rate) {
    uart_rx[UART_FIRST].huart_ptr->Init.BaudRate = rate;
    HAL_UART_Init(uart_rx[UART_FIRST].huart_ptr);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == uart_rx[UART_FIRST].huart_ptr) {
        uart_rx[UART_FIRST].status |= HALF_RECEIVED_FLAG;
        if (uart_rx[UART_FIRST].rx_callback != NULL) {
            (*uart_rx[UART_FIRST].rx_callback)();
        }
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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == uart_rx[UART_FIRST].huart_ptr) {
        uart_1_tx.full_transmitted = true;
        if (uart_1_tx.tx_callback != NULL) {
            (*uart_1_tx.tx_callback)();
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

    HAL_UART_Receive_DMA(config->huart_ptr, config->buffer, config->size);
}

#endif  // HAL_UART_MODULE_ENABLED
