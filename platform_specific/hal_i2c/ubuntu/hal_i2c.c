/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_i2c.h"
#include <string.h>
#include "libperiph_common.h"

uint8_t ubuntu_i2c_id __attribute__((weak)) = 0;
uint8_t ubuntu_i2c_buffer[256] __attribute__((weak)) = {};


int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len) {
    if (tx == NULL || len == 0) {
        return LIBPERIPH_ERROR;
    }

    ubuntu_i2c_id = id;
    memcpy(ubuntu_i2c_buffer, tx, len);
    return LIBPERIPH_OK;
}

int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len) {
    if (rx == NULL || len == 0) {
        return LIBPERIPH_ERROR;
    }

    memcpy(rx, ubuntu_i2c_buffer, len);
    return LIBPERIPH_OK;
}