/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file sf1xx.c
 * @author d.ponomarev
 * @note https://www.mouser.com/datasheet/2/321/28055-LW20-SF20-LiDAR-Manual-Rev-7-1371848.pdf
 */

#include "sf1xx.h"
#include <string.h>
#include "main.h"
#include "i2c_manager.h"
#include "params.h"


#define I2C_ID                  (0x68 << 1)
#define I2C_RESPONSE_SIZE       2
static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];
static int8_t i2c_manager_id = -1;

static void sf1xxMeasureCallback();


void sf1xxInit(int8_t new_i2c_manager_id) {
    i2c_manager_id = new_i2c_manager_id;
}


bool sf1xxCollectData(uint32_t measurement_period) {
    static uint32_t next_measurement_time_ms = 0;
    uint32_t crnt_time = HAL_GetTick();

    if (i2c_manager_id == STATUS_ERROR || crnt_time < next_measurement_time_ms) {
        return false;
    }
    next_measurement_time_ms = crnt_time + measurement_period;

    if (i2cManagerPerformRequest(i2c_manager_id, &sf1xxMeasureCallback) == STATUS_ERROR) {
        return false;
    }

    return true;
}

float sf1xxParseCollectedData() {
    return ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01;
}

void sf1xxMeasureCallback() {
    memset(i2c_response_buf, 0x00, I2C_RESPONSE_SIZE);
    i2cReceive(I2C_ID, i2c_response_buf, I2C_RESPONSE_SIZE);
}
