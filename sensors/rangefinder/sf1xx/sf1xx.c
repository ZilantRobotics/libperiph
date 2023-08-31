/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "sf1xx.h"
#include <string.h>
#include "main.h"
#include "i2c_manager.h"
#include "params.h"

#define I2C_ID                  (0x68 << 1)
#define I2C_RESPONSE_SIZE       2
static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];


int8_t sf1xxInit() {
    return LIBPERIPH_OK;
}

int8_t sf1xxCollectData() {
    memset(i2c_response_buf, 0x00, I2C_RESPONSE_SIZE);
    i2cReceive(I2C_ID, i2c_response_buf, I2C_RESPONSE_SIZE);
}

float sf1xxParseCollectedData() {
    return ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01;
}

bool sf1xxCollectDataPeriodically(int8_t i2c_manager_id, uint32_t measurement_period) {
    static uint32_t next_measurement_time_ms = 0;
    uint32_t crnt_time = HAL_GetTick();

    if (i2c_manager_id < 0 || crnt_time < next_measurement_time_ms) {
        return false;
    }
    next_measurement_time_ms = crnt_time + measurement_period;

    if (i2cManagerPerformRequest(i2c_manager_id, &sf1xxCollectData) == LIBPERIPH_ERROR) {
        return false;
    }

    return true;
}
