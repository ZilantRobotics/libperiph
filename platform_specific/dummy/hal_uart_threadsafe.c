/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart_threadsafe.h"
#include <iostream>
#include <chrono>
#include <thread>

// ubx_nav_pvt package example
static uint8_t buffer[] = {
    181, 98, 1, 7, 92, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 100, 193
};

int8_t tsUartInitRx(uint8_t buffer[], uint16_t size) {
    return 0;
}

bool tsUartWaitUntilReceiveIsComplete(uint32_t timeout_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return true;
}

uint8_t* tsUartPopRxDma() {
    return buffer;
}

int8_t tsUartInitTx() {
    return 0;
}

int8_t tsUartTransmit(uint8_t buffer[], size_t size) {
    return 0;
}

void tsUartTxDmaCallback() {
}

void tsUartRxDmaCallback() {
}
