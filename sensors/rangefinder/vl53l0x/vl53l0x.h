/*
 * Copyright (C) 2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_RANGESENSOR_VL53L0X_H_
#define SENSORS_RANGESENSOR_VL53L0X_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int8_t vl53l0xInit(int8_t i2c_manager_id);

/**
  * @brief  Process measurement
  * @note   Blocking operation. It should be performed as fast as possible.
  */
bool vl53l0xCollectData(uint32_t measurement_period);

/**
  * @brief  Parse I2C rx buffer
  */
float vl53l0xParseCollectedData();

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_RANGESENSOR_VL53L0X_H_
