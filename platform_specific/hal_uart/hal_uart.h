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

typedef enum {
    UART_FIRST,
    UART_SECOND,
} UartInstance_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief UART RX
 */
int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size);
void uartRegisterRxCallback(UartInstance_t instance, void (*rx_callback)());
void uartRegisterTxCallback(UartInstance_t instance, void (*tx_callback)());
size_t uartGetLastReceivedIndex(UartInstance_t instance);
uint8_t* uartRxDmaPop();


/**
 * @brief UART1 TX
 */
int8_t uartTransmit(uint8_t buffer[], size_t size);
int8_t uartTransmitDma(uint8_t buffer[], size_t size);
bool uartIsTxReady();

void uartEnableTx(bool enable);
void UartChangeBaudrate(uint16_t rate);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_SPECIFIC_HAL_UART_H_
