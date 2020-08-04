/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rangesensor_sf1xx.c
 * @author d.ponomarev
 * @note https://www.mouser.com/datasheet/2/321/28055-LW20-SF20-LiDAR-Manual-Rev-7-1371848.pdf
 */

#include "rangesensor_sf1xx.h"
#include <stdint.h>


#define I2C_ID              (0x66 << 1)
#define I2C_RESPONSE_SIZE   2
#define I2C_REQUEST_SIZE    1

static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];


float sf1xxParseFromI2C(uint8_t i2c_response_buf[2]) {
    float range = ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01;
    return range;
}
