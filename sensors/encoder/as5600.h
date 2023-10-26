/*
 * Copyright (C) 2022-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file as5600.h
 * @author sainquake@gmail.com (ko.burdinov)
 * @date 01.09.2022
 */

#ifndef SENSORS_ENCODER_AS5600_H_
#define SENSORS_ENCODER_AS5600_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return LIBPERIPH_OK if there is no error, otherwise < 0
 */
int8_t AS5600Init();

/**
  * Collect data from i2c and Parse them
  *
  * CollectData() is a blocking function. It reads data from I2C and save it to the internal buffer.
  * It returns LIBPERIPH_OK on success and < 0 on failure.
  *
  * Parse() parses the received buffer and returns the distance in meters.
  */
int8_t AS5600CollectData();
float AS5600Parse();

/**
  * @brief  Only for test usage
  */
void AS5600FillBuffer(const uint8_t new_buffer[]);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_ENCODER_AS5600_H_
