/*
 * Copyright (C) 2020 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
/**
 * @file rangefinder/garmin_lite.h
 * @author d.ponomarev
 */
#ifndef RANGESENSOR_GARMIN_LIGHT_H_
#define RANGESENSOR_GARMIN_LIGHT_H_

#include <stdbool.h>
#include <stdint.h>

/**
  * @brief  Just save i2c_manager_id
  */
void garminLiteInit(int8_t i2c_manager_id);


/**
  * @brief  Process measurement
  * @note   Blocking operation. It should be performed as fast as possible.
  */
bool garminLiteCollectData(uint32_t crnt_time, uint32_t measurement_period);


/**
  * @brief  Parse I2C rx buffer
  */
float garminLiteParseCollectedData();

#endif  // RANGESENSOR_GARMIN_LIGHT_H_