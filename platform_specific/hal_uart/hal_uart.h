/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
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
    UART_AMOUNT,
} UartInstance_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Common
 */
void UartChangeBaudrate(UartInstance_t instance, uint32_t rate);
uint32_t UartGetBaudrate(UartInstance_t instance);
bool uartIsReady();

/**
 * @brief UART RX
 */
int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size);
void uartRegisterRxCallback(UartInstance_t instance, void (*rx_callback)());
size_t uartGetLastReceivedIndex(UartInstance_t instance);
uint8_t* uartRxDmaPop();
uint32_t uartRxGetStats(UartInstance_t instance);
void uartRxResetStats();


/**
 * @brief UART1 TX
 */
int8_t uartTransmit(const uint8_t buffer[], size_t size);
int8_t uartTransmitDma(uint8_t buffer[], size_t size);
void uartRegisterTxCallback(UartInstance_t instance, void (*tx_callback)());
void uartEnableTx(bool enable);

/**
 * @brief For SITL and unit tests only
 */
void uartSetLastReceivedIndex(UartInstance_t instance, size_t last_recv_idx);

#ifdef __cplusplus
}
#endif

#endif  // PLATFORM_SPECIFIC_HAL_UART_H_
