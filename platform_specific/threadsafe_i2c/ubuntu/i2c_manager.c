/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "i2c_manager.h"
#include "libperiph_common.h"

int8_t i2cManagerRegister(uint8_t priority) {
    return STATUS_OK;
}

int8_t i2cManagerPerformRequest(int8_t device_id, void (*function)()) {
    return STATUS_OK;
}

void i2cManagerSpin() {
}

int8_t i2cWriteRegisterOneByte(uint8_t dev_id, uint8_t reg_addr, uint8_t new_reg_value) {
    return STATUS_OK;
}

int8_t i2cReadRegister(uint8_t dev_id, uint8_t reg_addr, uint8_t* out, uint8_t out_length) {
    return STATUS_OK;
}
