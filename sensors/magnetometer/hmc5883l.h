/*
 * Copyright (C) 2018-2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef SENSORS_HMC5883L_H_
#define SENSORS_HMC5883L_H_

#include <stdint.h>

#define HMC5883L_MEASUREMENT_FREQUENCY    30

#ifdef __cplusplus
extern "C" {
#endif

int8_t hmc5883Init();

int8_t hmc5883lMeasure();
void hmc5883GetMeasurement(float* x, float* y, float* z);


// For tests only:
void hmc5883lFillRxBuffer(const uint8_t new_buf[6]);

#ifdef __cplusplus
}
#endif

#endif  // SENSORS_HMC5883L_H_
