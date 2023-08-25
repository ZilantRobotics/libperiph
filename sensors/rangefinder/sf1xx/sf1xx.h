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

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Just save i2c_manager_id
  */
void sf1xxInit(int8_t i2c_manager_id);

/**
  * @brief  Process measurement
  * @note   Blocking operation. It should be performed as fast as possible.
  */
bool sf1xxCollectData(uint32_t measurement_period);

/**
  * @brief  Parse I2C rx buffer
  */
float sf1xxParseCollectedData();

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_RANGESENSOR_SF1XX_H_
