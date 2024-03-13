/*
 * Copyright (C) 2020-2024 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "bmp280.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include "libperiph_common.h"
#include "hal_i2c.h"


#define I2C_ID                  0xEE
#define ID_REG                  0xD0        // 4.3.1 Register 0xD0 “id”
#define RESET_REG               0xE0        // 4.3.2 Register 0xE0 “reset”
#define STATUS_REG              0xF3        // 4.3.3 Register 0xF3 “status”
#define CTRL_MEAS_REG           0xF4        // 4.3.4 Register 0xF4 “ctrl_meas”
#define CONFIG_REG              0xF5        // 4.3.5 Register 0xF5 “config”
#define PRESS_REG               0xF7        // 4.3.6 Register 0xF7…0xF9 “press”
#define TEMP_REG                0xFA        // 4.3.7 Register 0xFA…0xFC “temp”

#define PRESS_OVERSAMPLING_8    0x4 << 2    // 3.3.1 Pressure measurement
#define TEMP_OVERSAMPLING       0x4 << 5    // 3.3.2 Temperature measurement
#define CTRL_MEAS_NORMAL_MODE   3           // 3.6 Power modes
#define CTRL_MEAS_SETTINGS      PRESS_OVERSAMPLING_8 | TEMP_OVERSAMPLING | CTRL_MEAS_NORMAL_MODE

#define DEVICE_ID               0x58
#define RESET_CMD               0xB6

/**
 * @note The “id” register contains the chip identification number chip_id[7:0], which is 0x58.
 * This number can be read as soon as the device finished the power-on-reset.
 */
typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;

    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
} TrimmingParameters_t;
static_assert(sizeof(TrimmingParameters_t) == 24);

typedef struct {
    float t[3];
    float p[9];
} ProcessedCalibParam_t;
static_assert(sizeof(ProcessedCalibParam_t) == 48);

typedef struct {
    uint8_t p_msb;
    uint8_t p_lsb;
    uint8_t p_xlsb;
    uint8_t t_msb;
    uint8_t t_lsb;
    uint8_t t_xlsb;
} BMP280_measurement_registers_t;
static_assert(sizeof(BMP280_measurement_registers_t) == 6);

typedef struct {
    ProcessedCalibParam_t calib;
    BarometerMeasurements raw;
    uint32_t stale_counter;
    bool dev_id_confirmed;
} BarometerBmp280Instance;
static_assert(sizeof(BarometerBmp280Instance) == 64);


static int8_t _checkDeviceId();
static int8_t _setCtrlMeas();
static int8_t _calibrate();


static BarometerBmp280Instance bmp280;


int8_t bmp280Init() {
    bmp280.stale_counter = 0;
    bmp280.dev_id_confirmed = false;

    if (_checkDeviceId() < 0) {
        return -LIBPERIPH_BPM280_INIT_CHECK_DEV_ID_ERROR;
    }

    if (_setCtrlMeas() < 0) {
        return -LIBPERIPH_BPM280_INIT_SET_CTRL_MEAS_ERROR;
    }

    if (_calibrate() < 0) {
        return -LIBPERIPH_BPM280_INIT_CALIBRATION_ERROR;
    }

    return (bmp280IsInitialized()) ? LIBPERIPH_BPM280_OK : -LIBPERIPH_BPM280_INIT_WRONG_DEV_ID;
}

bool bmp280IsInitialized() {
    return bmp280.dev_id_confirmed;
}

int8_t bmp280GetData(BarometerMeasurements* out_data) {
    int8_t res = bmp280CollectData();
    if (res < 0) {
        return res;
    }

    res = bmp280ParseCollectedData(out_data);
    if (res < 0) {
        return res;
    }

    return bmp280VerifyData(out_data);
}

int8_t bmp280CollectData() {
    uint8_t tx[1] = {PRESS_REG};
    if (i2cTransmit(I2C_ID, tx, 1) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    BMP280_measurement_registers_t data = {};
    if (i2cReceive(I2C_ID + 1, (uint8_t*)&data, 6) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    BarometerMeasurements temp = {
        .pressure = (float)(data.p_msb << 12 | data.p_lsb << 4 | data.p_xlsb >> 4),
        .temperature = (float)(data.t_msb << 12 | data.t_lsb << 4 | data.t_xlsb >> 4),
    };

    if (temp.pressure == bmp280.raw.pressure && temp.temperature == bmp280.raw.temperature) {
        bmp280.stale_counter++;
    } else {
        bmp280.stale_counter = 0;
    }

    bmp280.raw = temp;

    return LIBPERIPH_BPM280_OK;
}


int8_t bmp280ParseCollectedData(BarometerMeasurements* out_data) {
    if (out_data == NULL) {
        return -LIBPERIPH_BPM280_INTERNAL_ERROR;
    }

    float ofs = bmp280.raw.temperature - bmp280.calib.t[0];
    float t_fine = (ofs * bmp280.calib.t[2] + bmp280.calib.t[1]) * ofs;
    out_data->temperature = t_fine * (1.0f / 5120.0f) + 273;

    float tf = t_fine - 128000.0f;
    float x1 = (tf * bmp280.calib.p[5] + bmp280.calib.p[4]) * tf + bmp280.calib.p[3];
    float x2 = (tf * bmp280.calib.p[2] + bmp280.calib.p[1]) * tf + bmp280.calib.p[0];
    if (x2 == 0) {
        return -LIBPERIPH_BPM280_INTERNAL_ERROR;
    }
    float pf = (bmp280.raw.pressure + x1) / x2;
    out_data->pressure = (pf * bmp280.calib.p[8] + bmp280.calib.p[7]) * pf + bmp280.calib.p[6];

    return LIBPERIPH_BPM280_OK;
}


int8_t bmp280VerifyData(const BarometerMeasurements* data) {
    if (data->pressure < 30000 || data->pressure > 110000) {
        return -LIBPERIPH_BPM280_VERIFICATION_PRESSURE_OUT_OF_BOUND;
    }

    if (data->temperature < 233 || data->temperature > 368) {
        return -LIBPERIPH_BPM280_VERIFICATION_TEMPERATURE_OUT_OF_BOUND;
    }

    if (bmp280.stale_counter >= 10) {
        return -LIBPERIPH_BPM280_VERIFICATION_STALE;
    }

    return LIBPERIPH_BPM280_OK;
}


/**
 * @note The “id” register contains the chip identification number chip_id[7:0], which is 0x58.
 * This number can be read as soon as the device finished the power-on-reset.
 */
static int8_t _checkDeviceId() {
    uint8_t tx[1] = {ID_REG};
    if (i2cTransmit(I2C_ID, tx, 1) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    uint8_t rx[1] = {0};
    if (i2cReceive(I2C_ID, rx, 1) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    bmp280.dev_id_confirmed = (rx[0] == DEVICE_ID) ? true : false;

    return LIBPERIPH_BPM280_OK;
}

/**
 * @note The “ctrl_meas” register sets the data acquisition options of the device.
 */
static int8_t _setCtrlMeas() {
    uint8_t tx[2] = {CTRL_MEAS_REG, CTRL_MEAS_SETTINGS};
    if (i2cTransmit(I2C_ID, tx, 2) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    uint8_t rx[1] = {0};
    if (i2cReceive(I2C_ID, rx, 1) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    return LIBPERIPH_BPM280_OK;
}

/**
 * @note Trimming parameter readout
 */
static int8_t _calibrate() {
    uint8_t tx[1] = {0x88};
    if (i2cTransmit(I2C_ID, tx, 1) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    TrimmingParameters_t params;
    if (i2cReceive(I2C_ID, (uint8_t*)(&params), 24) < 0) {
        return -LIBPERIPH_BPM280_I2C_NO_RESPONSE;
    }

    bmp280.calib.t[0] = params.dig_T1 * powf(2,  4);
    bmp280.calib.t[1] = params.dig_T2 * powf(2, -14);
    bmp280.calib.t[2] = params.dig_T3 * powf(2, -34);

    bmp280.calib.p[0] = params.dig_P1 * (powf(2, 4) / -100000.0f);
    bmp280.calib.p[1] = params.dig_P1 * params.dig_P2 * (powf(2, -31) / -100000.0f);
    bmp280.calib.p[2] = params.dig_P1 * params.dig_P3 * (powf(2, -51) / -100000.0f);

    bmp280.calib.p[3] = params.dig_P4 * powf(2, 4) - powf(2, 20);
    bmp280.calib.p[4] = params.dig_P5 * powf(2, -14);
    bmp280.calib.p[5] = params.dig_P6 * powf(2, -31);

    bmp280.calib.p[6] = params.dig_P7 * powf(2, -4);
    bmp280.calib.p[7] = params.dig_P8 * powf(2, -19) + 1.0f;
    bmp280.calib.p[8] = params.dig_P9 * powf(2, -35);

    return LIBPERIPH_BPM280_OK;
}
