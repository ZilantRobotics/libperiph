/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file rangefinder_garmin_lite.c
 * @author d.ponomarev
 */

#include "rangefinder_garmin_lite.h"
#include <string.h>

#define STATUS_ERROR    -1
#define STATUS_OK       0

#define REG_ACQ_COMMAND             0x00    ///< Device command
#define REG_STATUS                  0x01    ///< System status
#define REG_FULL_DELAY_HIGH         0x0f    ///< Distance measurement high byte
#define REG_FULL_DELAY_LOW          0x10    ///< Distance measurement low byte
#define REG_UNIT_ID_HIGH            0x16    ///< Serial number high byte
#define REG_UNIT_ID_LOW             0x17    ///< Serial measurement low byte

#define ACQ_COMMAND_TAKE_DISTANCE   0x04    ///< Take distance with receiver bias correction

#define I2C_ID                  (0x62 << 1)
#define I2C_RESPONSE_SIZE       2
static uint8_t i2c_response_buf[I2C_RESPONSE_SIZE];
static int8_t i2c_manager_id = -1;


static void garminLiteMeasureCallback();

// functions below should be implemented outside
int8_t i2cManagerPerformRequest(int8_t device_id, void (*function)());
int8_t i2cManagerTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cManagerReceive(uint8_t id, uint8_t* rx, uint8_t len);
// functions above should be implemented outside

int8_t i2cWriteRegisterOneByte(uint8_t reg_addr, uint8_t new_reg_value) {
    uint8_t tx_buffer[2] = {reg_addr, new_reg_value};
    if (i2cManagerTransmit(I2C_ID, (const uint8_t*)&tx_buffer, 2) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

int8_t i2cReadRegister(uint8_t reg_addr, uint8_t* out, uint8_t out_length) {
    if (i2cManagerTransmit(I2C_ID, &reg_addr, 1) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    if (i2cManagerReceive(I2C_ID, out, out_length) == STATUS_ERROR) {
        return STATUS_ERROR;
    }
    return STATUS_OK;
}

void garminLiteInit(int8_t new_i2c_manager_id) {
    i2c_manager_id = new_i2c_manager_id;

    uint8_t status;
    if (i2cReadRegister(REG_STATUS, &status, 1) == STATUS_ERROR) {
        return;
    }

    uint8_t serial_number[2] = {};
    if (i2cReadRegister(REG_UNIT_ID_HIGH, &serial_number[0], 1) == STATUS_ERROR ||
            i2cReadRegister(REG_UNIT_ID_LOW, &serial_number[1], 1) == STATUS_ERROR) {
        return;
    }
}


bool garminLiteCollectData(uint32_t crnt_time, uint32_t measurement_period) {
    static uint32_t next_measurement_time_ms = 0;

    if (crnt_time < next_measurement_time_ms) {
        return false;
    }
    next_measurement_time_ms = crnt_time + measurement_period;

    if (i2cManagerPerformRequest(i2c_manager_id, &garminLiteMeasureCallback) == STATUS_ERROR) {
        return false;
    }

    return true;
}

float garminLiteParseCollectedData() {
    return ((i2c_response_buf[0] << 8) + i2c_response_buf[1]) * 0.01;
}

/**
 * @note The simplest method of obtaining measurement results from the I2C interface is as follows:
 * 1. Write 0x04 to register 0x00
 * 2. Read register 0x01. Repeat until bit 0 (LSB) goes low
 * 3. Read two bytes from 0x8f (0x0f and 0x10) to obtain 16-bit measured distance in centimeters.
 */
void garminLiteMeasureCallback() {
    i2cWriteRegisterOneByte(REG_ACQ_COMMAND, ACQ_COMMAND_TAKE_DISTANCE);

    memset(i2c_response_buf, 0x00, I2C_RESPONSE_SIZE);
    i2cReadRegister(0x8f, &i2c_response_buf[0], 2);
}