/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart_threadsafe.h"

int8_t uartInitRxDmaWithSem(uint8_t buffer[], uint16_t size) {
    return 0;
}

bool uartWaitUntilReceiveIsComplete(uint32_t timeout_ms) {
    return false;
}

int8_t uartInitTxDmaThreadSafe() {
    return 0;
}

int8_t uartTransmitDmaThreadSafe(uint8_t buffer[], size_t size) {
    return 0;
}

void uartTxDmaCallback() {
}

void uartRxDmaCallback() {
}
