/*
 * Copyright (C) 2019-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file mpu9250.c
 * @author d.ponomarev
 * @note https://invensense.tdk.com/wp-content/uploads/2015/02/RM-MPU-9250A-00-v1.6.pdf
 */

#include "mpu9250.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>

// functions below should be implemented outside
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
// functions above should be implemented outside

#ifndef STATUS_OK
    #define STATUS_OK       0
#endif
#ifndef STATUS_ERROR
    #define STATUS_ERROR    -1
#endif

// Config
#define MPU6050_USED

#define I2C_SENSOR_ID           0x68
#define I2C_SENSOR_ID_SHIFTED   0x68 << 1
#define I2C_MAG_AK8963_ID       0x0C
#define MAG_CNTL1_REG           0x0A

// Registers map
#define PWR_MGMT_1_REG          0x6B
#define GYRO_CONFIG_REG         0x1B
#define ACCEL_CONFIG_REG        0x1C
#define ACCEL_XOUT_H            0x3B

// Registers values
#define GYRO_FS_SEL_250_DPS     (0 << 3)    // 0x00
#define GYRO_FS_SEL_500_DPS     (1 << 3)    // 0x08
#define GYRO_FS_SEL_1000_DPS    (2 << 3)    // 0x10
#define GYRO_FS_SEL_2000_DPS    (3 << 3)    // 0x18

#define ACCEL_FS_SEL_2_G        (0 << 3)    // 0x00
#define ACCEL_FS_SEL_4_G        (1 << 3)    // 0x08
#define ACCEL_FS_SEL_8_G        (2 << 3)    // 0x10
#define ACCEL_FS_SEL_16_G       (3 << 3)    // 0x18

// Scale
#define GYRO_SCALE_250_DPS      (250.00f / 32768 * 3.14f / 180.0)
#define GYRO_SCALE_500_DPS      (500.00f / 32768 * 3.14f / 180.0)
#define GYRO_SCALE_1000_DPS     (1000.0f / 32768 * 3.14f / 180.0)
#define GYRO_SCALE_2000_DPS     (2000.0f / 32768 * 3.14f / 180.0)

#define ACCEL_SCALE_2_G         (2.00 * 9.81f / 32768)
#define ACCEL_SCALE_4_G         (4.00 * 9.81f / 32768)
#define ACCEL_SCALE_8_G         (8.00 * 9.81f / 32768)
#define ACCEL_SCALE_16_G        (16.0 * 9.81f / 32768)

typedef struct {
    uint8_t accel[6];
    uint8_t temp[2];
    uint8_t gyro[6];
} RxBuffer_t;
static_assert(sizeof(RxBuffer_t) == 14, "Wrong allignment.");

int16_t accel_raw[3];
int16_t gyro_raw[3];


static RxBuffer_t rx_buffer = {0};

static void mpu9250doConvert();


bool mpu9250Init() {
    uint8_t buffer[2];

#ifdef MPU6050_USED
    buffer[0] = PWR_MGMT_1_REG;
    buffer[1] = 0x00;
    i2cTransmit(I2C_SENSOR_ID_SHIFTED, buffer, 2);
#endif

    buffer[0] = GYRO_CONFIG_REG;
    buffer[1] = GYRO_FS_SEL_250_DPS;
    i2cTransmit(I2C_SENSOR_ID_SHIFTED, buffer, 2);

    buffer[0] = ACCEL_CONFIG_REG;
    buffer[1] = ACCEL_FS_SEL_2_G;
    i2cTransmit(I2C_SENSOR_ID_SHIFTED, buffer, 2);

    return STATUS_OK;
}


void mpu9250Measure() {
    uint8_t tx_buf_recv[1] = {ACCEL_XOUT_H};
    i2cTransmit(I2C_SENSOR_ID_SHIFTED, tx_buf_recv, 1);
    i2cReceive(I2C_SENSOR_ID_SHIFTED, (uint8_t*)(&rx_buffer), 14);
}

void mpu9250doConvert() {
    const uint8_t* rx_buf = (const uint8_t*)&rx_buffer;
    accel_raw[0] = ((int16_t)(rx_buf[0]) << 8) | rx_buf[1];
    accel_raw[1] = ((int16_t)(rx_buf[2]) << 8) | rx_buf[3];
    accel_raw[2] = ((int16_t)(rx_buf[4]) << 8) | rx_buf[5];

    gyro_raw[0] = ((int16_t)(rx_buf[8]) << 8) | rx_buf[9];
    gyro_raw[1] = ((int16_t)(rx_buf[10]) << 8) | rx_buf[11];
    gyro_raw[2] = ((int16_t)(rx_buf[12]) << 8) | rx_buf[13];
}

void mpu9250GetMeasurements(float linear_acceleration[3], float angular_velocity[3]) {
    mpu9250doConvert();

    float ACCEL_SCALE = ACCEL_SCALE_2_G;
    float GYRO_SCALE = GYRO_SCALE_250_DPS;

    linear_acceleration[0] = accel_raw[0] * ACCEL_SCALE;
    linear_acceleration[1] = accel_raw[1] * ACCEL_SCALE;
    linear_acceleration[2] = accel_raw[2] * ACCEL_SCALE;

    angular_velocity[0] = gyro_raw[0] * GYRO_SCALE;
    angular_velocity[1] = gyro_raw[1] * GYRO_SCALE;
    angular_velocity[2] = gyro_raw[2] * GYRO_SCALE;
}
