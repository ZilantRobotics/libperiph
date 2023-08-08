/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"
#include <string.h>
#include "libperiph_common.h"

static uint16_t rx_last_indexes[2];
static uint8_t* rx_buffers[2] = {};

// We should have a ring buffer here and a callback here
static uint8_t tx_buffer[256] = {};
static uint16_t tx_buffer_size;

int8_t uartInitRxDma(UartInstance_t instance, uint8_t buffer[], uint16_t size) {
    if (instance >= UART_AMOUNT || buffer == NULL || size == 0) {
        return LIBPERIPH_ERROR;
    }

    rx_buffers[instance] = buffer;

    return LIBPERIPH_OK;
}

void uartSetLastReceivedIndex(UartInstance_t instance, size_t last_recv_idx) {
    if (instance >= UART_AMOUNT) {
        return;
    }

    rx_last_indexes[instance] = last_recv_idx;
}

size_t uartGetLastReceivedIndex(UartInstance_t instance) {
    return (instance >= UART_AMOUNT) ? 0 : rx_last_indexes[instance];
}

uint8_t* uartRxDmaPop() {
    return rx_buffers[UART_FIRST];
}

int8_t uartTransmit(uint8_t buffer[], size_t size) {
    if (buffer == NULL || size == 0) {
        return LIBPERIPH_ERROR;
    }

    memcpy(tx_buffer, buffer, size);
    tx_buffer_size = size;
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
