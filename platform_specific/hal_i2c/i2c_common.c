/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hal_i2c.h"
#include "main.h"
#include "libperiph_common.h"

int8_t i2cWriteRegisterOneByte(uint8_t dev_id, uint8_t reg_addr, uint8_t new_reg_value) {
    uint8_t tx_buffer[2] = {reg_addr, new_reg_value};
    if (i2cTransmit(dev_id, tx_buffer, 2) < 0) {
        return LIBPERIPH_ERROR;
    }

    return LIBPERIPH_OK;
}

int8_t i2cReadRegister(uint8_t dev_id, uint8_t reg_addr, uint8_t* out, uint8_t out_length) {
    if (i2cTransmit(dev_id, &reg_addr, 1) < 0) {
        return LIBPERIPH_ERROR;
    }
    if (i2cReceive(dev_id, out, out_length) < 0) {
        return LIBPERIPH_ERROR;
    }

    return LIBPERIPH_OK;
}
