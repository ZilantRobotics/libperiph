/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "bmp280.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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

// 4.3.4 Register 0xF4 “ctrl_meas”
#define PRESS_OVERSAMPLING_8    0x4 << 2    // 3.3.1 Pressure measurement
#define TEMP_OVERSAMPLING       0x4 << 5    // 3.3.2 Temperature measurement
#define CTRL_MEAS_NORMAL_MODE   3           // 3.6 Power modes
#define CTRL_MEAS_SETTINGS      PRESS_OVERSAMPLING_8 | TEMP_OVERSAMPLING | CTRL_MEAS_NORMAL_MODE

#define DEVICE_ID               0x58
#define RESET_CMD               0xB6

typedef struct {
    float static_pressure;
    float static_temperature;
} BMP280_t;

typedef struct {
    uint16_t t1;
    int16_t t2;
    int16_t t3;

    uint16_t p1;
    int16_t p2;
    int16_t p3;
    int16_t p4;
    int16_t p5;
    int16_t p6;
    int16_t p7;
    int16_t p8;
    int16_t p9;
} BMP280_stored_calib_param_t;

typedef struct {
    float t1;
    float t2;
    float t3;

    float p1;
    float p2;
    float p3;
    float p4;
    float p5;
    float p6;
    float p7;
    float p8;
    float p9;
} BMP280_processed_calib_param_t;

typedef struct {
    uint8_t p_msb;
    uint8_t p_lsb;
    uint8_t p_xlsb;
    uint8_t t_msb;
    uint8_t t_lsb;
    uint8_t t_xlsb;
} BMP280_measurement_registers_t;


static void bmp280CheckDeviceId();
static void bmp280SetCtrlMeas();
static void bmp280GetCalibration();


static bool dev_id_confirmed = false;
static BMP280_stored_calib_param_t stored_calib;
static BMP280_processed_calib_param_t processed_calib;
static BMP280_t bmp280;


void bmp280Init() {
    bmp280CheckDeviceId();
    bmp280SetCtrlMeas();
    bmp280GetCalibration();
}

bool bmp280IsInitialized() {
    return dev_id_confirmed;
}

void bmp280Calibrate() {
    processed_calib.t1 = stored_calib.t1 * powf(2,  4);
    processed_calib.t2 = stored_calib.t2 * powf(2, -14);
    processed_calib.t3 = stored_calib.t3 * powf(2, -34);

    processed_calib.p1 = stored_calib.p1 * (powf(2, 4) / -100000.0f);
    processed_calib.p2 = stored_calib.p1 * stored_calib.p2 * (powf(2, -31) / -100000.0f);
    processed_calib.p3 = stored_calib.p1 * stored_calib.p3 * (powf(2, -51) / -100000.0f);

    processed_calib.p4 = stored_calib.p4 * powf(2, 4) - powf(2, 20);
    processed_calib.p5 = stored_calib.p5 * powf(2, -14);
    processed_calib.p6 = stored_calib.p6 * powf(2, -31);

    processed_calib.p7 = stored_calib.p7 * powf(2, -4);
    processed_calib.p8 = stored_calib.p8 * powf(2, -19) + 1.0f;
    processed_calib.p9 = stored_calib.p9 * powf(2, -35);
}

void bmp280CollectData() {
    uint8_t tx[1] = {PRESS_REG};
    BMP280_measurement_registers_t data;
    i2cTransmit(I2C_ID, tx, 1);
    i2cReceive(I2C_ID + 1, (uint8_t*)&data, 6);

    bmp280.static_pressure = data.p_msb << 12 | data.p_lsb << 4 | data.p_xlsb >> 4;
    bmp280.static_temperature = data.t_msb << 12 | data.t_lsb << 4 | data.t_xlsb >> 4;
}


void bmp280ParseData() {
    float static_temperature, static_pressure;

    float ofs = (float)bmp280.static_temperature - processed_calib.t1;
    float t_fine = (ofs * processed_calib.t3 + processed_calib.t2) * ofs;
    static_temperature = t_fine * (1.0f / 5120.0f) + 273;

    float tf = t_fine - 128000.0f;
    float x1 = (tf * processed_calib.p6 + processed_calib.p5) * tf + processed_calib.p4;
    float x2 = (tf * processed_calib.p3 + processed_calib.p2) * tf + processed_calib.p1;
    float pf = ((float) bmp280.static_pressure + x1) / x2;
    static_pressure = (pf * processed_calib.p9 + processed_calib.p8) * pf + processed_calib.p7;

    bmp280.static_temperature = static_temperature;
    bmp280.static_pressure = static_pressure;
}

float bmp280GetStaticPressure() {
    return bmp280.static_pressure;
}

float bmp280GetStaticTemperature() {
    return bmp280.static_temperature;
}

void bmp280CheckDeviceId() {
    uint8_t tx[1] = {ID_REG};
    uint8_t rx[1] = {0};

    i2cTransmit(I2C_ID, tx, 1);
    i2cReceive(I2C_ID, rx, 1);

    dev_id_confirmed = (rx[0] == DEVICE_ID) ? true : false;
}


void bmp280SetCtrlMeas() {
    uint8_t tx[2] = {CTRL_MEAS_REG, CTRL_MEAS_SETTINGS};
    uint8_t rx[1] = {0};
    i2cTransmit(I2C_ID, tx, 2);
    i2cReceive(I2C_ID, rx, 1);
}

void bmp280GetCalibration() {
    uint8_t tx[1] = {0x88};
    i2cTransmit(I2C_ID, tx, 1);
    i2cReceive(I2C_ID, (uint8_t*)(&stored_calib), 24);
}
