/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_RANGESENSOR_SF1XX_H_
#define SENSORS_RANGESENSOR_SF1XX_H_

#include <stdbool.h>
#include <stdint.h>
#include "libperiph_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LW_SF10_A,
    LW_SF10_B,
    LW_SF10_C,
    LW_SF11_C,
    LW_SF_LW20_B,
    LW_SF_LW20_C,
    LW_SF_LW30_D,
    LW_KNOWN_SENSOR_TYPES_AMOUNT,
} LightwareSensor;

/**
 * @return LIBPERIPH_OK if there is no error, otherwise < 0
 */
int8_t sf1xxInit();

/**
  * Collect data from i2c and parse them
  * CollectData is a blocking function. It reads data from I2C and save it to the internal buffer.
  * It returns LIBPERIPH_OK on success and < 0 on failure.
  * ParseCollectedData parses the received buffer and returns the distance in meters.
  */
int8_t sf1xxCollectData();
float sf1xxParseCollectedData();

/**
 * @note Additional info about the sensor
 */
float lightwareGetMinDistance(LightwareSensor sensor_type);
float lightwareGetMaxDistance(LightwareSensor sensor_type);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_RANGESENSOR_SF1XX_H_
