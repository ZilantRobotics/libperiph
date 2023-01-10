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

extern UART_HandleTypeDef huart1;

void UartChangeBaudrate(uint32_t rate) {
    huart1.Init.BaudRate = rate;
    HAL_UART_Init(&huart1);
}

bool uartIsReady() {
    HAL_UART_StateTypeDef status = HAL_UART_GetState(&huart1);
    return (status == HAL_UART_STATE_READY || status == HAL_UART_STATE_BUSY_RX);
}
