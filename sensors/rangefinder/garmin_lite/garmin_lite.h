/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file garmin_lite.h
 * @author d.ponomarev
 */
#ifndef SENSORS_RANGESENSOR_GARMIN_LITE_H_
#define SENSORS_RANGESENSOR_GARMIN_LITE_H_

#include <stdbool.h>
#include <stdint.h>

/**
  * @brief  Just save i2c_manager_id
  */
int8_t garminLiteInit(int8_t i2c_manager_id);


/**
  * @brief  Process measurement
  * @note   Blocking operation. It should be performed as fast as possible.
  */
bool garminLiteCollectData(uint32_t measurement_period);


/**
  * @brief  Parse I2C rx buffer
  */
float garminLiteParseCollectedData();

/**
  * @brief  Get 2 bytes of serial number
  */
void garminGetSerialNumber(uint8_t buffer[]);

#endif  // SENSORS_RANGESENSOR_GARMIN_LITE_H_
