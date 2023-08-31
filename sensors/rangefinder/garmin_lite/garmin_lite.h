/*
 * Copyright (C) 2020-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_RANGESENSOR_GARMIN_LITE_H_
#define SENSORS_RANGESENSOR_GARMIN_LITE_H_

#include <stdbool.h>
#include <stdint.h>
#include "libperiph_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return LIBPERIPH_OK if there is no error, otherwise < 0
 */
int8_t garminLiteInit();

/**
  * Collect data from i2c and parse them
  * CollectData is a blocking function. It reads data from I2C and save it to the internal buffer.
  * It returns LIBPERIPH_OK on success and < 0 on failure.
  * ParseCollectedData parses the received buffer and returns the distance in meters.
  */
int8_t garminLiteCollectData();
float garminLiteParseCollectedData();


/**
  * @brief  Get 2 bytes of serial number
  */
void garminGetSerialNumber(uint8_t buffer[]);

/**
  * @brief  Process measurement
  * @note   Blocking operation. It should be performed as fast as possible.
  */
bool garminLiteCollectDataPeriodically(int8_t i2c_manager_id, uint32_t measurement_period);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_RANGESENSOR_GARMIN_LITE_H_
