/*
 * Copyright (C) 2018-2022 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/**
 * @file hmc5883l.h
 * @author d.ponomarev
 * @date Nov 18, 2018
 */

#ifndef HMC5883L_H_
#define HMC5883L_H_

#include <stdint.h>

#define HMC5883L_MEASUREMENT_FREQUENCY    30


int8_t hmc5883Init();

void hmc5883lMeasure();
void hmc5883GetMeasurement(float* x, float* y, float* z);


// For tests only:
void hmc5883lFillRxBuffer(uint8_t new_buf[6]);

#endif  // HMC5883L_H_
