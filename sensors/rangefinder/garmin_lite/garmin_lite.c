/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "garmin_lite.h"
#include <string.h>
#include "main.h"
#include "hal_i2c.h"

#define REG_ACQ_COMMAND                     0x00    ///< Device command
#define REG_STATUS                          0x01    ///< System status
#define REG_FULL_DELAY_HIGH                 0x0f    ///< Distance measurement high byte
#define REG_FULL_DELAY_LOW                  0x10    ///< Distance measurement low byte
#define REG_GET_MEASUREMENT                 0x8f    ///< REG_FULL_DELAY_HIGH and REG_FULL_DELAY_LOW
#define REG_UNIT_ID_HIGH                    0x16    ///< Serial number high byte
#define REG_UNIT_ID_LOW                     0x17    ///< Serial measurement low byte

#define ACQ_COMMAND_TAKE_RAW_DISTANCE       0x03    ///< Take distance with without bias correction
#define ACQ_COMMAND_TAKE_BIASED_DISTANCE    0x04    ///< Take distance with bias correction

#define I2C_ID                  (0x62 << 1)
#define I2C_RESPONSE_SIZE       2

static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];
static uint8_t _serial_number[2] = {0};


int8_t garminLiteInit() {
    if (i2cReadRegister(I2C_ID, REG_UNIT_ID_HIGH, &_serial_number[0], 1) < 0) {
        return LIBPERIPH_ERROR;
    }

    if (i2cReadRegister(I2C_ID, REG_UNIT_ID_LOW, &_serial_number[1], 1) < 0) {
        return LIBPERIPH_ERROR;
    }

    return LIBPERIPH_OK;
}

float garminLiteParseCollectedData() {
    return ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01f;
}

int8_t garminLiteCollectData() {
    memset(i2c_response_buf, 0x00, I2C_RESPONSE_SIZE);
    i2cReadRegister(I2C_ID, REG_GET_MEASUREMENT, &i2c_response_buf[0], I2C_RESPONSE_SIZE);

    i2cWriteRegisterOneByte(I2C_ID, REG_ACQ_COMMAND, ACQ_COMMAND_TAKE_RAW_DISTANCE);

    return LIBPERIPH_OK;
}

void garminGetSerialNumber(GarminLiteSerialNumber serial_number) {
    memcpy(serial_number.buffer, _serial_number, 2);
}
