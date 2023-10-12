/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "hmc5883l.h"
#include <string.h>
#include "libperiph_common.h"


#define I2C_ID                      0x3C
#define I2C_ID_READ                 0x3C
#define REG_CONF_A                  0x00
#define REG_CONF_B                  0x01
#define REG_MODE                    0x02
#define REG_DATA_OUT_X_MSB          0x03
#define REG_DATA_OUT_X_LSB          0x04
#define REG_DATA_OUT_Y_MSB          0x05
#define REG_DATA_OUT_Y_LSB          0x06
#define REG_DATA_OUT_Z_MSB          0x07
#define REG_DATA_OUT_Z_LSB          0x08
#define REG_STATUS                  0x09
#define REG_IDENTIFICATION_A        0x0A
#define REG_IDENTIFICATION_B        0x0B
#define REG_IDENTIFICATION_C        0x0C

#define REG_CONF_A_DEF              (0 << 0)  ///< default
#define REG_CONF_A_RATE_15_HZ       (4 << 2)  ///< default
#define REG_CONF_A_RATE_30_HZ       (5 << 2)
#define REG_CONF_A_RATE_75_HZ       (6 << 2)
#define REG_CONF_A_SAMPLE_1         (0 << 5)  ///< default
#define REG_CONF_A_SAMPLE_2         (1 << 5)
#define REG_CONF_A_SAMPLE_4         (2 << 5)
#define REG_CONF_A_SAMPLE_8         (3 << 5)
#define REG_CONF_A_DEF_30_HZ_8_SAMP REG_CONF_A_DEF + REG_CONF_A_RATE_30_HZ + REG_CONF_A_SAMPLE_8

#define REG_CONF_B_GAIN_LSB_1370    (0 << 5)
#define REG_CONF_B_GAIN_LSB_1090    (1 << 5)  ///< default

#define REG_MODE_CONTINUOUS_MODE    (0 << 0)
#define REG_MODE_SINGLE_MODE        (1 << 0)  ///< default


typedef struct {
    int16_t raw[3];
    float mag[3];
} HMC5883_t;

static void hmc5883lParse();

// functions below should be implemented outside
int8_t i2cTransmit(uint8_t id, const uint8_t tx[], uint8_t len);
int8_t i2cReceive(uint8_t id, uint8_t* rx, uint8_t len);
// functions above should be implemented outside

static uint8_t rx_buf[6] = {0};
static HMC5883_t hmc5883;


int8_t hmc5883Init() {
    const uint8_t TX_BUF_1[2] = {REG_CONF_A, REG_CONF_A_DEF_30_HZ_8_SAMP};
    const uint8_t TX_BUF_2[2] = {REG_CONF_B, REG_CONF_B_GAIN_LSB_1090};
    const uint8_t TX_BUF_3[2] = {REG_MODE, REG_MODE_CONTINUOUS_MODE};

    if (i2cTransmit(I2C_ID_READ, TX_BUF_1, 2) == STATUS_ERROR ||
            i2cTransmit(I2C_ID_READ, TX_BUF_2, 2) == STATUS_ERROR ||
            i2cTransmit(I2C_ID_READ, TX_BUF_3, 2) == STATUS_ERROR ||
            i2cReceive(I2C_ID_READ, rx_buf, 6) == STATUS_ERROR ) {
        return STATUS_ERROR;
    }

    return STATUS_OK;
}

void hmc5883GetMeasurement(float* x, float* y, float* z) {
    hmc5883lParse();
    *x = hmc5883.mag[0];
    *y = hmc5883.mag[1];
    *z = hmc5883.mag[2];
}

int8_t hmc5883lMeasure() {
    memset(rx_buf, 0x00, 6);
    const uint8_t TX_BUF_1[1] = {REG_DATA_OUT_X_MSB};
    if (i2cReceive(I2C_ID_READ, rx_buf, 6) == STATUS_ERROR ||
            i2cTransmit(I2C_ID_READ, TX_BUF_1, 1) == STATUS_ERROR) {
        asm("NOP");
    }

    return LIBPERIPH_OK;
}

void hmc5883lFillRxBuffer(const uint8_t new_buf[6]) {
    memcpy(rx_buf, new_buf, 6);
}

static void hmc5883lParse() {
    hmc5883.raw[0] = (rx_buf[0] << 8) + rx_buf[1];
    hmc5883.raw[1] = (rx_buf[2] << 8) + rx_buf[3];
    hmc5883.raw[2] = (rx_buf[4] << 8) + rx_buf[5];

    hmc5883.mag[0] = hmc5883.raw[0] / 1090.0;
    hmc5883.mag[1] = hmc5883.raw[1] / 1090.0;
    hmc5883.mag[2] = hmc5883.raw[2] / 1090.0;
}
