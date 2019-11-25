/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file temperature_sensor.h
 * @author d.ponomarev
 * @date Nov 18, 2018
 */

#ifndef __TEMPERATURE_SENSOR_HPP
#define __TEMPERATURE_SENSOR_HPP

#include "stdint.h"

/**
* @brief Get temperature value
* @return temperature value in degree celsius
*/
uint32_t temperature_sensor_parse(int32_t raw_adc_value);

#endif //__TEMPERATURE_SENSOR_HPP
