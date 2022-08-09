/*
 * Copyright (C) 2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file mpu9250.c
 * @author d.ponomarev
 */

#include "imu/mpu9250.h"
#include <string.h>
#include <stdint.h>


#define I2C_SENSOR_ID           0x68
#define I2C_SENSOR_ID_SHIFTED   0x68 << 1
#define I2C_MAG_AK8963_ID       0x0C
#define MAG_CNTL1_REG           0x0A


static int16_t accel_raw[3];
static int16_t gyro_raw[3];


void mpu9250doConvert(const uint8_t* rx_buf) {
    accel_raw[0] = ((int16_t)(rx_buf[0]) << 8) | rx_buf[1];
    accel_raw[1] = ((int16_t)(rx_buf[2]) << 8) | rx_buf[3];
    accel_raw[2] = ((int16_t)(rx_buf[4]) << 8) | rx_buf[5];

    gyro_raw[0] = ((int16_t)(rx_buf[8]) << 8) | rx_buf[9];
    gyro_raw[1] = ((int16_t)(rx_buf[10]) << 8) | rx_buf[11];
    gyro_raw[2] = ((int16_t)(rx_buf[12]) << 8) | rx_buf[13];
}

void mpu9250doParse(float linear_acceleration[3], float angular_velocity[3]) {
    float ACCEL_SCALE = 0.0006103515;   // +-20m/sec^2 (+-2g) divided by 2^15 (int16_t)
    float GYRO_SCALE = 0.00013315795;   // +-4.36332 rad/sec (+-250 degrees/sec) divided by 2^15

    linear_acceleration[0] = accel_raw[0] * ACCEL_SCALE;
    linear_acceleration[1] = accel_raw[1] * ACCEL_SCALE;
    linear_acceleration[2] = accel_raw[2] * ACCEL_SCALE;

    angular_velocity[0] = gyro_raw[0] * GYRO_SCALE;
    angular_velocity[1] = gyro_raw[1] * GYRO_SCALE;
    angular_velocity[2] = gyro_raw[2] * GYRO_SCALE;
}
