/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_uart.h"

int8_t uartInitRxDma(uint8_t buffer[], uint16_t size) {
    return 0;
}

size_t uartLastRecvIndex() {
    return 0;
}

uint8_t* uartRxDmaPop() {
    return NULL;
}

int8_t uartTransmit(uint8_t buffer[], size_t size) {
    return 0;
}

int8_t uartTransmitDma(uint8_t buffer[], size_t size) {
    return 0;
}
bool uartIsTxReady() {
    return false;
}

void uartEnableTx() {
}

void uartDisableTx() {
}

void UartChangeBaudrate(uint16_t rate) {
}

int8_t uartInitRxDmaSecond(uint8_t buffer[], uint16_t size) {
    return 0;
}

size_t uartLastRecvIndexSecond() {
    return 0;
}