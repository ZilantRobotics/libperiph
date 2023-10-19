/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_
#define SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "libperiph_common.h"

typedef struct {
    float temperature;
    float diff_pressure;
} DifferentialPressureData;

/**
  * @brief Run it once. It returns LIBPERIPH_OK on success and < 0 on failure.
  */
int8_t ms4525doInit();


/**
  * Collect data from i2c and parse them
  * CollectData is a blocking function. It reads data from I2C and save it to the internal buffer.
  * It returns LIBPERIPH_OK on success and < 0 on failure.
  * ParseCollectedData parses the received buffer and returns the sensor data.
  */
int8_t ms4525CollectData();
DifferentialPressureData ms4525ParseCollectedData();

/**
  * @brief Only for test usage
  */
void ms4525doFillBuffer(const uint8_t new_buffer[]);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_DIFFERENTIAL_PRESSURE_MS4525DO_H_
