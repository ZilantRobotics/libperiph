/*
 * Copyright (C) 2020-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "rm3100.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "libperiph_common.h"


#ifndef UNUSED
    #define UNUSED(x) (void)(x)
#endif

#define DEBUG_RM3100    0


#define I2C_ID                              0x20    // The higher 5 bits are predefined (0b01000xx)

// All registers for I2C and SPI
#define REG_POLL                            0x00    // Polls for a Single Measurement
#define REG_CMM                             0x01    // Initiates Continuous Measurement Mode
#define REG_CCX                             0x04    // Cycle Count Register –X Axis
#define REG_CCY                             0x06    // Cycle Count Register –Y Axis
#define REG_CCZ                             0x08    // Cycle Count Register –Z Axis
#define REG_TMRC                            0x0B    // Sets Continuous Measurement Mode Data Rate
#define REG_MX                              0x24    // Measurement Results – X Axis
#define REG_MY                              0x27    // Measurement Results – Y Axis
#define REG_MZ                              0x2A    // Measurement Results – Z Axis
#define REG_BIST                            0x33    // Built-In Self TestRegister
#define REG_STATUS                          0x34    // Status of DRDY
#define REG_HSHAKE                          0x35    // Handshake Register
#define REG_REVID                           0x36    // REVID Register

#define CCX_DEFAULT_MSB                     0
#define CCX_DEFAULT_LSB                     200
#define CCY_DEFAULT_MSB                     CCX_DEFAULT_MSB
#define CCY_DEFAULT_LSB                     CCX_DEFAULT_LSB
#define CCZ_DEFAULT_MSB                     CCX_DEFAULT_MSB
#define CCZ_DEFAULT_LSB                     CCX_DEFAULT_LSB
#define TMRC_DEFAULT                        0x95    ///< 75 hz
#define START_ALL_AXIS                      1 | (1 << 4) | (1 << 5) | (1 << 6);

#define UTESLA_TO_GAUSS                     100.0f
#define RM3100_GAIN_OF_CYCLE_COUNTS_200     75.0f
#define SCALE                               1.0 / UTESLA_TO_GAUSS / RM3100_GAIN_OF_CYCLE_COUNTS_200

#define READ_MASK                           0x80    // REG | MASK

static int32_t rm3100ConvertSigned(int32_t n);

// functions below should be implemented outside
int8_t spiPerformTransaction(uint8_t* tx, uint8_t* rx, uint8_t size);
// functions above should be implemented outside

typedef struct {
    float mag[3];
} RM3100_t;
static RM3100_t rm3100;


int8_t rm3100Init() {
    uint8_t tx_buf[8];
    uint8_t rx_buf[8];
    int8_t result = STATUS_OK;

    // Set up registers
    tx_buf[0] = REG_CCX;
    tx_buf[1] = CCX_DEFAULT_MSB;
    tx_buf[2] = CCX_DEFAULT_LSB;
    tx_buf[3] = CCY_DEFAULT_MSB;
    tx_buf[4] = CCY_DEFAULT_LSB;
    tx_buf[5] = CCZ_DEFAULT_MSB;
    tx_buf[6] = CCZ_DEFAULT_LSB;
    result |= spiPerformTransaction(tx_buf, rx_buf, 7);

    tx_buf[0] = REG_TMRC;
    tx_buf[1] = TMRC_DEFAULT;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    // Check registers
    tx_buf[0] = READ_MASK | REG_CCX;
    tx_buf[1] = 0x00;
    memset(tx_buf + 2, 0x00, 5);
    result |= spiPerformTransaction(tx_buf, rx_buf, 7);
    if (rx_buf[1] != CCX_DEFAULT_MSB || rx_buf[2] != CCX_DEFAULT_LSB ||
            rx_buf[3] != CCY_DEFAULT_MSB || rx_buf[4] != CCY_DEFAULT_LSB ||
            rx_buf[5] != CCZ_DEFAULT_MSB || rx_buf[6] != CCZ_DEFAULT_LSB) {
        result = STATUS_ERROR;
    }

    tx_buf[0] = READ_MASK | REG_TMRC;
    tx_buf[1] = 0x00;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);
    if (rx_buf[1] != TMRC_DEFAULT) {
        result = STATUS_ERROR;
    }

    tx_buf[0] = READ_MASK | REG_BIST;
    tx_buf[1] = 0x00;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    tx_buf[0] = READ_MASK | REG_STATUS;
    tx_buf[1] = 0x00;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    tx_buf[0] = READ_MASK | REG_HSHAKE;
    tx_buf[1] = 0x00;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    tx_buf[0] = READ_MASK | REG_REVID;
    tx_buf[1] = 0;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    // Set Continuous Measurement Mode
    // Important note: Certain  commands,  such  as  reading  from  the  CMM
    // register or  writing  to  the  TMRC  register, will terminate CMM
    // So, it is necessary to run this command in the end on initialization
    tx_buf[0] = REG_CMM;
    tx_buf[1] = START_ALL_AXIS;
    result |= spiPerformTransaction(tx_buf, rx_buf, 2);

    return result;
}

int8_t rm3100Measure() {
    uint8_t tx_buf[10] = {0};
    uint8_t rx_buf[10] = {0};
    int8_t status = STATUS_OK;

    tx_buf[0] = READ_MASK | REG_STATUS;
    tx_buf[1] = 0x00;
    status = spiPerformTransaction(tx_buf, rx_buf, 2);
    uint8_t rdy = rx_buf[1] >= 0x80;
    if (status != STATUS_OK || rdy != true) {
        return STATUS_ERROR;
    }

    tx_buf[0] = READ_MASK | REG_MX;
    memset(tx_buf + 1, 0x00, 9);
    if (spiPerformTransaction(tx_buf, rx_buf, 10) != STATUS_OK) {
        return STATUS_ERROR;
    }

    // uavcan uses Gauss, but rm3100 is utesla
    rm3100.mag[0] = rm3100ConvertSigned((rx_buf[1] << 16) | (rx_buf[2] << 8) | rx_buf[3]) * SCALE;
    rm3100.mag[1] = rm3100ConvertSigned((rx_buf[4] << 16) | (rx_buf[5] << 8) | rx_buf[6]) * SCALE;
    rm3100.mag[2] = rm3100ConvertSigned((rx_buf[7] << 16) | (rx_buf[8] << 8) | rx_buf[9]) * SCALE;

#if DEBUG_RM3100 == 1
    for (uint8_t idx = 0; idx < 9; idx++) {
        if (rx_buf[idx + 1] != 0) {
            asm("NOP");
            break;
        }
    }
#endif

    return STATUS_OK;
}

void rm310GetMeasurement(float* x, float* y, float* z) {
    *x = rm3100.mag[0];
    *y = rm3100.mag[1];
    *z = rm3100.mag[2];
}


/**
 * @note Sensor returns values as 24 bit signed values,
 * so we need to manually convert to 32 bit signed values
 */
int32_t rm3100ConvertSigned(int32_t n) {
    if ((n & (1 << 23)) == (1 << 23)) {
        n |= 0xFF000000;
    }
    return n;
}
