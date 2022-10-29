/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"
#include <string.h>
#include "config.h"
#include "hal_uart_threadsafe.h"
#include "main.h"

#define MAX_UART_TX_BUF_SIZE    100


#ifdef HAL_UART_MODULE_ENABLED

extern UART_HandleTypeDef huart1;
static uint8_t* rx_buffer1;
static uint16_t rx_buffer_size1;
static uint8_t tx_buffer[MAX_UART_TX_BUF_SIZE];
static uint16_t tx_buffer_max_size = 0;
static bool uart_full_transmitted = true;
static bool uart_half_received = false;
static bool uart_full_received = false;

#if defined(SECOND_UART)
    extern UART_HandleTypeDef huart2;
    static uint8_t* rx_buffer2;
    static uint16_t rx_buffer_size2;
#endif


int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size) {
    if (instance == UART_FIRST) {
        rx_buffer1 = buffer;
        rx_buffer_size1 = size;
        HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&huart1, buffer, size);
        return (status == HAL_OK) ? STATUS_OK : STATUS_ERROR;
    } else if (instance == UART_SECOND) {
#if defined(SECOND_UART)
        rx_buffer2 = buffer;
        rx_buffer_size2 = size;
        HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&huart2, buffer, size);
        return (status == HAL_OK) ? STATUS_OK : STATUS_ERROR;
#endif
    }

    return STATUS_ERROR;
}

size_t uartLastRecvIndex(UartInstance_t instance) {
    if (instance == UART_FIRST) {
        if (rx_buffer_size1 == __HAL_DMA_GET_COUNTER(huart1.hdmarx)) {
            return rx_buffer_size1 - 1;
        }
        return rx_buffer_size1 - __HAL_DMA_GET_COUNTER(huart1.hdmarx) - 1;
    } else if (instance == UART_SECOND) {
#if defined(SECOND_UART)
        if (rx_buffer_size2 == __HAL_DMA_GET_COUNTER(huart2.hdmarx)) {
            return rx_buffer_size2 - 1;
        }
        return rx_buffer_size2 - __HAL_DMA_GET_COUNTER(huart2.hdmarx) - 1;
#endif
    }

    return 0;
}

uint8_t* uartRxDmaPop() {
    if (!uart_half_received && !uart_full_received) {
        return NULL;
    }

    uint8_t* buffer_ptr = rx_buffer1;
    if (uart_full_received) {
        buffer_ptr += rx_buffer_size1 / 2;
    }
    uart_half_received = false;
    uart_full_received = false;
    return buffer_ptr;
}


int8_t uartTransmit(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE) {
        return -1;
    }
    memcpy(tx_buffer, buffer, size);
    return HAL_UART_Transmit(&huart1, tx_buffer, size, 500) == HAL_OK ? 0 : -1;
}

int8_t uartTransmitDma(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE || !uart_full_transmitted) {
        return -1;
    }
    memcpy(tx_buffer, buffer, size);
    tx_buffer_max_size = size;
    uart_full_transmitted = false;
    return HAL_UART_Transmit_DMA(&huart1, tx_buffer, size) == HAL_OK ? 0 : -1;
}

bool uartIsTxReady() {
    HAL_UART_StateTypeDef status = HAL_UART_GetState(&huart1);
    if (status == HAL_UART_STATE_READY || status == HAL_UART_STATE_BUSY_RX) {
        return true;
    }
    return false;
}


void uartEnableTx() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void uartDisableTx() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        uart_half_received = true;
        uartRxDmaCallback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        uart_full_received = true;
        uartRxDmaCallback();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        uart_full_transmitted = true;
        uartTxDmaCallback();
    }
}

void UartChangeBaudrate(uint16_t rate) {
    /*  there is no need to override all parameters
    of usart connection, only baudrate should be changed  */
    huart1.Init.BaudRate = rate;
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

/**
 * @note Example of error: baud rate is wrong
 * For example, it may occur when during ESP8266 initialization
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart == &huart1) {
        HAL_UART_Receive_DMA(&huart1, rx_buffer1, rx_buffer_size1);
    }
#ifdef SECOND_UART
    if (huart == &huart2) {
        HAL_UART_Receive_DMA(&huart2, rx_buffer2, rx_buffer_size2);
    }
#endif
}

#endif  // HAL_UART_MODULE_ENABLED
