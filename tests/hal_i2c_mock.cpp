/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file hal_i2c_mock.cpp
 * @author d.ponomarev
 */

#include <stdint.h>

int8_t i2cManagerPerformRequest(int8_t device_id, void (*function)()) {
    function();
    return 0;
}
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len) {
    return 0;
}
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len) {
    return len;
}
