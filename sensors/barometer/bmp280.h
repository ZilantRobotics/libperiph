/*
 * Copyright (C) 2020-2024 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef BAROMETER_BMP280_H_
#define BAROMETER_BMP280_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BMP280_MAX_MEASUREMENT_FREQUENCY            50

typedef enum {
    LIBPERIPH_BPM280_OK                                     = 0,
    LIBPERIPH_BPM280_UNKNOWN_ERRROR                         = 1,

    LIBPERIPH_BPM280_I2C_NO_RESPONSE                        = 2,

    LIBPERIPH_BPM280_INIT_CHECK_DEV_ID_ERROR                = 10,
    LIBPERIPH_BPM280_INIT_SET_CTRL_MEAS_ERROR               = 11,
    LIBPERIPH_BPM280_INIT_CALIBRATION_ERROR                 = 12,
    LIBPERIPH_BPM280_INIT_WRONG_DEV_ID                      = 13,

    LIBPERIPH_BPM280_VERIFICATION_PRESSURE_OUT_OF_BOUND     = 20,
    LIBPERIPH_BPM280_VERIFICATION_TEMPERATURE_OUT_OF_BOUND  = 21,
    LIBPERIPH_BPM280_VERIFICATION_STALE                     = 22,

    LIBPERIPH_BPM280_INTERNAL_ERROR                         = 127,
} Bpm280ErrorCode;


typedef struct {
    float pressure;
    float temperature;
} BarometerMeasurements;


/**
 * @brief Initialize the device including:
 * 1. Check device ID
 * 2. Set the data acquisition options of the device with `ctrl_meas` register
 * 3. Readout trimming parameter and calculate the calibration coefficients
 * @return LIBPERIPH_BPM280_OK on success, otherwise error code < 0
 * @note The operation is time consuming: it performs the communication and
 * a lot of calculations.
 */
int8_t bmp280Init();
bool bmp280IsInitialized();


/**
 * @brief Collect data from i2c, parse it and verify
 * @param[out] out_data - pressure in Pascal and temperature in Kelvin
 * @return LIBPERIPH_BPM280_OK on success, otherwise error code < 0
 * @note Sequentially call bmp280CollectData, bmp280ParseCollectedData and bmp280VerifyData.
 */
int8_t bmp280GetData(BarometerMeasurements* out_data);


/**
 * @brief Alternatively, you can collect, parse and verify the measurements separately
 * @param[in] data - barometer measurements
 * @return LIBPERIPH_BPM280_OK on success, otherwise error code < 0
 * @note The verification consist of:
 * 1. Verify that the Pressure in range within [30.000, 110.000] Pascal
 * (equiv. to +9000 .. -500 m above/below sea level)
 *
 * 2. Verify that the Temperature in range within [233, 368] Kelvin
 * (-40 ... +85 °C)
 *
 * 3. Verify that the measurements are not stale
 */
int8_t bmp280CollectData();
int8_t bmp280ParseCollectedData(BarometerMeasurements* out_data);
int8_t bmp280VerifyData(const BarometerMeasurements* data);

#ifdef __cplusplus
}
#endif

#endif  // BAROMETER_BMP280_H_
