/*
 * Copyright (C) 2018-2019 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file temperature_sensor.c
 * @author d.ponomarev
 * @brief Implementation of TemperatureSensor
 * @date Nov 18, 2018
 */

#include "temperature_sensor.h"

uint32_t temperature_sensor_parse(int32_t raw_adc_value)
{
	const uint8_t temp_25 = 25;
	const float voltage_at_25 = 1.43;
	const float slope = 232.558;
	const float adc_koef = 0.00073242187;
	float voltage_sense = raw_adc_value * adc_koef;
	uint32_t value = (voltage_at_25 - voltage_sense)*slope + temp_25;
	return value;
}
