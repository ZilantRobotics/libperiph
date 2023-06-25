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


#define MAX_UART_TX_BUF_SIZE    100

typedef struct {
    uint8_t buffer[MAX_UART_TX_BUF_SIZE];
    bool full_transmitted;
    void (*tx_callback)();
} UartTxConfig_t;


static UartTxConfig_t uart_1_tx = {{}, true};

void uartRegisterTxCallback(UartInstance_t instance, void (*tx_callback)()) {
    if (instance == UART_FIRST) {
        uart_1_tx.tx_callback = tx_callback;
    }
}

int8_t uartTransmit(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE) {
        return STATUS_ERROR;
    }
    memcpy(uart_1_tx.buffer, buffer, size);
    HAL_StatusTypeDef res = HAL_UART_Transmit(UART_1_PTR, uart_1_tx.buffer, size, 500);
    return -res;
}

int8_t uartTransmitDma(uint8_t buffer[], size_t size) {
    if (size > MAX_UART_TX_BUF_SIZE || !uart_1_tx.full_transmitted) {
        return STATUS_ERROR;
    }
    memcpy(uart_1_tx.buffer, buffer, size);
    uart_1_tx.full_transmitted = false;
    return HAL_UART_Transmit_DMA(UART_1_PTR, uart_1_tx.buffer, size) == HAL_OK ? 0 : -1;
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

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == UART_1_PTR) {
        uart_1_tx.full_transmitted = true;
        if (uart_1_tx.tx_callback != NULL) {
            (*uart_1_tx.tx_callback)();
        }
    }
}
