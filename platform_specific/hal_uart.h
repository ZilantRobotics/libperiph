/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef PLATFORM_SPECIFIC_HAL_UART_H_
#define PLATFORM_SPECIFIC_HAL_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/**
 * @brief UART1 RX DMA
 * @note It is expected to poll data with some period of time.
 * Response time is limited by a spin period. Not effective.
 */
int8_t uartInitRxDma(uint8_t buffer[], uint16_t size);
size_t uartLastRecvIndex();
uint8_t* uartRxDmaPop();


/**
 * @brief UART1 TX
 */
int8_t uartTransmit(uint8_t buffer[], size_t size);
int8_t uartTransmitDma(uint8_t buffer[], size_t size);
bool uartIsTxReady();

void uartEnableTx();
void uartDisableTx();
void UartChangeBaudrate(uint16_t rate);


/**
 * @brief UART2 DMA RX
 */
int8_t uartInitRxDmaSecond(uint8_t buffer[], uint16_t size);
size_t uartLastRecvIndexSecond();


#endif  // PLATFORM_SPECIFIC_HAL_UART_H_
