/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_CURRENT_SENSOR_ACS712_H_
#define SENSORS_CURRENT_SENSOR_ACS712_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initialize.
 */
void acs712SetCoefficient(float new_adc_raw_to_dc_dc_out_current);

/**
 * @brief   Convert raw adc value into current. It might be either positive or negative.
 */
float acs712ConvertToCurrent(uint16_t current_raw);

/**
 * @brief   Process calibration of sensor.
 * @return  false during the process, true when the calibration is finished
 */
bool acs712PerformIterationOfCalibration(uint16_t current_raw);

/**
 * @brief   Set calibrated value beside calibration process
 */
void acs712SetCalibratedValue(uint16_t current_raw);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_CURRENT_SENSOR_ACS712_H_
