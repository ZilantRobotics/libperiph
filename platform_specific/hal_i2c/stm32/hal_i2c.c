/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <hal_i2c.h>
#include "main.h"
#include "libperiph_common.h"


#ifdef HAL_I2C_MODULE_ENABLED
    extern I2C_HandleTypeDef hi2c1;
#endif

#define I2C_TIMEOUT_MS          10


int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len) {
#ifdef HAL_I2C_MODULE_ENABLED
    HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(&hi2c1, id, (uint8_t*)tx, len, I2C_TIMEOUT_MS);
    if (res == HAL_OK) {
        return LIBPERIPH_OK;
    }
#else
    UNUSED(id);
    UNUSED(tx);
    UNUSED(len);
#endif
    return LIBPERIPH_ERROR;
}

int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len) {
#ifdef HAL_I2C_MODULE_ENABLED
    HAL_StatusTypeDef res = HAL_I2C_Master_Receive(&hi2c1, id, rx, len, I2C_TIMEOUT_MS);
    if (res == HAL_OK) {
        return LIBPERIPH_OK;
    }
#else
    UNUSED(id);
    UNUSED(rx);
    UNUSED(len);
#endif
    return LIBPERIPH_ERROR;
}

int8_t i2cWriteRegisterOneByte(uint8_t dev_id, uint8_t reg_addr, uint8_t new_reg_value) {
    uint8_t tx_buffer[2] = {reg_addr, new_reg_value};
    if (i2cTransmit(dev_id, tx_buffer, 2) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

int8_t i2cReadRegister(uint8_t dev_id, uint8_t reg_addr, uint8_t* out, uint8_t out_length) {
    if (i2cTransmit(dev_id, &reg_addr, 1) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    if (i2cReceive(dev_id, out, out_length) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}
