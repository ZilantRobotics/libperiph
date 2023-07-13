/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"
#include "libperiph_common.h"

uint8_t ubuntu_uart_last_recv[2] __attribute__((weak)) = {};
uint8_t* ubuntu_uart_rx_buffer[2] __attribute__((weak)) = {};

uint8_t ubuntu_uart_tx_buffer[256] __attribute__((weak)) = {};

int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size) {
    if (instance >= UART_AMOUNT || buffer == NULL || size == 0) {
        return LIBPERIPH_ERROR;
    }

    ubuntu_uart_rx_buffer[instance] = buffer;

    return LIBPERIPH_OK;
}

size_t uartGetLastReceivedIndex(UartInstance_t instance) {
    return (instance >= UART_AMOUNT) ? 0 : ubuntu_uart_last_recv[instance];
}

uint8_t* uartRxDmaPop() {
    return NULL;
}

int8_t uartTransmit(uint8_t buffer[], size_t size) {
    if (buffer == NULL || size == 0) {
        return LIBPERIPH_ERROR;
    }

    memcpy(ubuntu_uart_tx_buffer, buffer, size);
    return LIBPERIPH_OK;
}

int8_t uartTransmitDma(uint8_t buffer[], size_t size) {
    return LIBPERIPH_OK;
}
bool uartIsReady() {
    return false;
}

void uartEnableTx(bool enable) {
}

void UartChangeBaudrate(UartInstance_t instance, uint32_t rate) {
}
