/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_UART_THREADSAFE_H_
#define PLATFORM_SPECIFIC_HAL_UART_THREADSAFE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief UART1 RX DMA with semaphores
 * @note It is expected to to wait until data is received.
 * Response time is as small as possible. More effective than simple DMA approach.
 */
int8_t tsUartInitRx(uint8_t buffer[], uint16_t size);
bool tsUartWaitUntilReceiveIsComplete(uint32_t timeout_ms);
uint8_t* tsUartPopRxDma();

/**
 * @brief UART1 TX
 */
int8_t tsUartInitTx();
int8_t tsUartTransmit(uint8_t buffer[], size_t size);

#endif  // PLATFORM_SPECIFIC_HAL_UART_THREADSAFE_H_
